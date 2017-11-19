﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace IL2C
{
    public static class Converter
    {
        internal static IEnumerable<ILData> DecodeAndEnumerateOpCodes(DecodeContext decodeContext)
        {
            while (true)
            {
                var label = decodeContext.MakeLabel();
                if (decodeContext.TryDecode(out var ilc) == false)
                {
                    break;
                }

                var operand = ilc.DecodeOperand(decodeContext);
                yield return new ILData(label, ilc, operand);

                if (ilc.IsEndOfPath)
                {
                    yield break;
                }
            }
        }

        public static void ConvertToHeader(
            TextWriter twHeader,
            Assembly assembly,
            TranslateContext translateContext,
            string indent)
        {
            var assemblyName = assembly.GetName().Name;

            twHeader.WriteLine("#ifndef __MODULE_{0}__", assemblyName);
            twHeader.WriteLine("#define __MODULE_{0}__", assemblyName);
            twHeader.WriteLine();

            foreach (var fileName in translateContext.EnumerateRequiredIncludeFileNames())
            {
                twHeader.WriteLine("#include <{0}>", fileName);
            }

            var types = assembly.GetTypes()
                .Where(type => (type.IsValueType || type.IsClass)
                && (type.IsPublic || type.IsNestedPublic || type.IsNestedFamily || type.IsNestedFamORAssem))
                .ToArray();

            InternalConvertToPrototypes(
                twHeader,
                types,
                translateContext,
                indent);

            twHeader.WriteLine();
            twHeader.WriteLine("#endif");
        }

        private static void InternalConvertToPrototypes(
            TextWriter tw,
            Type[] types,
            TranslateContext translateContext,
            string indent)
        {
            tw.WriteLine();
            tw.WriteLine("#ifdef __cplusplus");
            tw.WriteLine("extern \"C\" {");
            tw.WriteLine("#endif");

            tw.WriteLine();
            tw.WriteLine("//////////////////////////////////////////////////////////////////////////////////");
            tw.WriteLine("// Types:");
            tw.WriteLine();

            // Output prototypes.
            foreach (var type in types)
            {
                var typeName = translateContext.GetCLanguageTypeName(type, TypeNameFlags.Dereferenced)
                    .ManglingSymbolName();

                tw.WriteLine(
                    "typedef struct {0} {0};",
                    typeName);
            }

            // Output value type and object reference type.
            foreach (var type in types)
            {
                tw.WriteLine();
                ConvertType(
                    translateContext,
                    tw,
                    type,
                    indent);
            }

            tw.WriteLine();
            tw.WriteLine("//////////////////////////////////////////////////////////////////////////////////");
            tw.WriteLine("// Public static fields:");

            foreach (var type in types)
            {
                tw.WriteLine();

                foreach (var field in
                    from field in type.GetFields(
                        BindingFlags.Public |
                        BindingFlags.Static | BindingFlags.DeclaredOnly)
                    select field)
                {
                    tw.WriteLine(
                        "extern {0};",
                        Utilities.GetStaticFieldPrototypeString(field, false, translateContext));
                }
            }

            tw.WriteLine();
            tw.WriteLine("//////////////////////////////////////////////////////////////////////////////////");
            tw.WriteLine("// Methods:");

            foreach (var type in types)
            {
                tw.WriteLine();

                foreach (var method in
                    from method in type.GetMembers(
                            BindingFlags.Public | BindingFlags.NonPublic
                            | BindingFlags.Static | BindingFlags.Instance
                            | BindingFlags.DeclaredOnly)
                        .OfType<MethodBase>()
                    where (method is MethodInfo) || !method.IsStatic
                    select method)
                {
                    var methodName = Utilities.GetFullMemberName(method);

                    var mi = method as MethodInfo;
                    var functionPrototype = Utilities.GetFunctionPrototypeString(
                        methodName,
                        mi?.ReturnType ?? typeof(void),
                        method.GetSafeParameters(),
                        translateContext);

                    tw.WriteLine("extern {0};", functionPrototype);

                    // Is this instance constructor?
                    // TODO: Type initializer's handlers
                    if (method.IsConstructor && (method.IsStatic == false))
                    {
                        var typeName = translateContext.GetCLanguageTypeName(type, TypeNameFlags.Dereferenced);

                        // Write mark handler:
                        var makrHandlerPrototype = string.Format(
                            "extern void __{0}_MARK_HANDLER__(void* pReference);",
                            typeName);
                        tw.WriteLine(makrHandlerPrototype);

                        // Write new:
                        var newPrototype = string.Format(
                            "extern void __{0}_NEW__({0}** ppReference);",
                            typeName);

                        tw.WriteLine(newPrototype);
                    }
                }
            }

            tw.WriteLine();
            tw.WriteLine("#ifdef __cplusplus");
            tw.WriteLine("}");
            tw.WriteLine("#endif");
        }

        public static void ConvertType(
            TranslateContext translateContext,
            TextWriter tw,
            Type declaredType,
            string indent)
        {
            if (declaredType.IsPrimitive
                || !(declaredType.IsValueType || declaredType.IsClass))
            {
                return;
            }

            var structName = translateContext.GetCLanguageTypeName(declaredType, TypeNameFlags.Dereferenced)
                .ManglingSymbolName();

            tw.WriteLine("////////////////////////////////////////////////////////////");
            tw.WriteLine(
                "// {0}: {1}",
                declaredType.IsValueType ? "Struct" : "Class",
                Utilities.GetFullMemberName(declaredType));
            tw.WriteLine();

            var stopType = declaredType.IsValueType
                ? typeof(ValueType)
                : typeof(object);

            var fields = declaredType
                .Traverse(type => type.BaseType)
                .TakeWhile(type => type != stopType)
                .Reverse()
                .SelectMany(type => type.GetFields(
                    BindingFlags.Public | BindingFlags.NonPublic
                    | BindingFlags.Instance | BindingFlags.DeclaredOnly))
                .ToArray();
            if (fields.Length >= 1)
            {
                tw.WriteLine(
                    "struct {0}",
                    structName);
                tw.WriteLine("{");

                foreach (var field in fields)
                {
                    tw.WriteLine(
                        "{0}{1} {2};",
                        indent,
                        translateContext.GetCLanguageTypeName(field.FieldType),
                        field.Name);
                }

                tw.WriteLine("};");

                // Write sizeof:
                tw.WriteLine();
                tw.WriteLine(
                    "#define __{0}_SIZEOF__() (sizeof({0}))",
                    structName);
            }
            else
            {
                // Write sizeof:
                tw.WriteLine();
                tw.WriteLine(
                    "#define __{0}_SIZEOF__() (0)",
                    structName);
            }
        }

        public static void ConvertToSourceCode(
            TextWriter twSource,
            Assembly assembly,
            TranslateContext translateContext,
            string indent)
        {
            foreach (var fileName in translateContext.EnumerateRequiredPrivateIncludeFileNames())
            {
                twSource.WriteLine("#include <{0}>", fileName);
            }

            var assemblyName = assembly.GetName().Name;
            twSource.WriteLine("#include \"{0}.h\"", assemblyName);

            var allTypes = assembly.GetTypes()
                .Where(type => type.IsValueType || type.IsClass)
                .ToArray();
            var types = allTypes
                .Where(type => !(type.IsPublic || type.IsNestedPublic || type.IsNestedFamily || type.IsNestedFamORAssem))
                .ToArray();

            InternalConvertToPrototypes(
                twSource,
                types,
                translateContext,
                indent);

            twSource.WriteLine();
            twSource.WriteLine("//////////////////////////////////////////////////////////////////////////////////");
            twSource.WriteLine("// Static fields:");

            foreach (var type in allTypes)
            {
                twSource.WriteLine();

                foreach (var field in
                    from field in type.GetFields(
                        BindingFlags.Public | BindingFlags.NonPublic |
                        BindingFlags.Static | BindingFlags.DeclaredOnly)
                    select field)
                {
                    twSource.WriteLine(
                        "{0};",
                        Utilities.GetStaticFieldPrototypeString(field, true, translateContext));
                }
            }

            twSource.WriteLine();
            twSource.WriteLine("//////////////////////////////////////////////////////////////////////////////////");
            twSource.WriteLine("// Methods:");

            foreach (var type in allTypes)
            {
                twSource.WriteLine();
                twSource.WriteLine("////////////////////////////////////////////////////////////");
                twSource.WriteLine("// Type: {0}", Utilities.GetFullMemberName(type));

                foreach (var method in
                    from method in type.GetMembers(
                            BindingFlags.Public | BindingFlags.NonPublic
                            | BindingFlags.Static | BindingFlags.Instance
                            | BindingFlags.DeclaredOnly)
                        .OfType<MethodBase>()
                    where (method is MethodInfo) || !method.IsStatic
                    select method)
                {
                    ConvertMethod(
                        translateContext,
                        twSource,
                        method,
                        indent);
                }
            }
        }

        public static void ConvertMethod(
            TranslateContext translateContext,
            TextWriter tw,
            MethodBase method,
            string indent)
        {
            var methodName = Utilities.GetFullMemberName(method);
            var mi = method as MethodInfo;

            // Write method body:
            var body = method.GetMethodBody();
            if (body != null)
            {
                InternalConvert(
                    translateContext,
                    tw,
                    method.Module,
                    methodName,
                    mi?.ReturnType ?? typeof(void),
                    method.GetSafeParameters(),
                    body,
                    indent);
            }
            else if ((method.Attributes & MethodAttributes.PinvokeImpl) == MethodAttributes.PinvokeImpl)
            {
                var dllImportAttribute = method.GetCustomAttribute<DllImportAttribute>();
                if (dllImportAttribute == null)
                {
                    throw new InvalidProgramSequenceException(
                        "Missing DllImport attribute at P/Invoke entry: Method={0}",
                        methodName);
                }

                InternalConvert(
                    translateContext,
                    tw,
                    methodName,
                    mi?.ReturnType ?? typeof(void),
                    method.GetSafeParameters(),
                    method.Name,
                    dllImportAttribute,
                    indent);
            }

            // Is this instance constructor?
            // TODO: Type initializer's handlers
            if (method.IsConstructor && (method.IsStatic == false))
            {
                tw.WriteLine();
                tw.WriteLine("//////////////////////");
                tw.WriteLine("// Runtime helpers:");

                var type = method.DeclaringType;
                var typeName = translateContext.GetCLanguageTypeName(type, TypeNameFlags.Dereferenced);
                var baseTypeName = translateContext.GetCLanguageTypeName(type.BaseType, TypeNameFlags.Dereferenced);

                // Write mark handler:
                var makrHandlerPrototype = string.Format(
                    "void __{0}_MARK_HANDLER__(void* pReference)",
                    typeName);

                tw.WriteLine();
                tw.WriteLine(makrHandlerPrototype);
                tw.WriteLine("{");

                foreach (var field in type.GetFields(
                    BindingFlags.Public | BindingFlags.NonPublic
                    | BindingFlags.Instance | BindingFlags.DeclaredOnly)
                    .Where(field => field.FieldType.IsValueType == false))
                {
                    tw.WriteLine(
                        "{0}__TRY_MARK_FROM_HANDLER__((({1}*)pReference)->{2});",
                        indent,
                        typeName,
                        field.Name);
                }

                tw.WriteLine(
                    "{0}__{1}_MARK_HANDLER__(pReference);",
                    indent,
                    baseTypeName);
                tw.WriteLine("}");

                // Write new:
                var newPrototype = string.Format(
                    "void __{0}_NEW__({0}** ppReference)",
                    typeName);

                tw.WriteLine();
                tw.WriteLine(newPrototype);
                tw.WriteLine("{");
                tw.WriteLine("{0}__gc_get_uninitialized_object__(", indent);
                tw.WriteLine("{0}{0}(void**)ppReference,", indent);
                tw.WriteLine("{0}{0}__{1}_SIZEOF__(),", indent, typeName);
                tw.WriteLine("{0}{0}__{1}_MARK_HANDLER__);", indent, typeName);
                tw.WriteLine("{0}{1}__ctor(*ppReference);", indent, typeName);
                tw.WriteLine("}");
            }
        }

        private struct GeneratedSourceCode
        {
            public readonly Label Label;
            public readonly string[] SourceCode;

            public GeneratedSourceCode(Label label, string[] sourceCode)
            {
                this.Label = label;
                this.SourceCode = sourceCode;
            }
        }

        private static void InternalConvert(
            TranslateContext translateContext,
            TextWriter tw,
            Module module,
            string methodName,
            Type returnType,
            Parameter[] parameters,
            MethodBody body,
            string indent)
        {
            var locals = body.LocalVariables;

            var decodeContext = new DecodeContext(
                module,
                methodName,
                returnType,
                parameters,
                locals,
                body.GetILAsByteArray(),
                translateContext);

            var bodySourceCode = new List<GeneratedSourceCode>();
            while (decodeContext.TryDequeueNextPath())
            {
                bodySourceCode.AddRange(
                    from ilData in DecodeAndEnumerateOpCodes(decodeContext)
                    let sourceCode = ilData.ILConverter.Apply(ilData.Operand, decodeContext)
                    select new GeneratedSourceCode(ilData.Label, sourceCode));
            }

            var functionPrototype = Utilities.GetFunctionPrototypeString(
                methodName,
                returnType,
                parameters,
                translateContext);

            tw.WriteLine();
            tw.WriteLine("///////////////////////////////////////");
            tw.WriteLine("// {0}", methodName);
            tw.WriteLine();

            tw.WriteLine(functionPrototype);
            tw.WriteLine("{");

            tw.WriteLine("{0}//-------------------", indent);
            tw.WriteLine("{0}// Local variables:", indent);
            tw.WriteLine();

            // Important NULL assigner (p = NULL):
            //   Because these variables are pointer (of object reference).
            //   So GC will traverse these variables just setup the stack frame.

            foreach (var local in locals)
            {
                tw.WriteLine(
                    "{0}{1} local{2}{3};",
                    indent,
                    translateContext.GetCLanguageTypeName(local.LocalType),
                    local.LocalIndex,
                    local.LocalType.IsValueType ? string.Empty : " = NULL");
            }

            tw.WriteLine();
            tw.WriteLine("{0}//-------------------", indent);
            tw.WriteLine("{0}// Evaluation stacks:", indent);
            tw.WriteLine();

            var stacks = decodeContext.ExtractStacks().ToArray();
            foreach (var si in stacks)
            {
                tw.WriteLine(
                    "{0}{1} {2}{3};",
                    indent,
                    translateContext.GetCLanguageTypeName(si.TargetType),
                    si.SymbolName,
                    si.TargetType.IsValueType ? string.Empty : " = NULL");
            }

            var frameEntries = locals
                .Where(local => local.LocalType.IsValueType == false)
                .Select(local => new { Type = local.LocalType, Name = "local" + local.LocalIndex })
                .Concat(stacks
                    .Where(stack => stack.TargetType.IsValueType == false)
                    .Select(stack => new { Type = stack.TargetType, Name = stack.SymbolName }))
                .ToArray();

            if (frameEntries.Length >= 1)
            {
                tw.WriteLine();
                tw.WriteLine("{0}//-------------------", indent);
                tw.WriteLine("{0}// Setup stack frame:", indent);
                tw.WriteLine();

                tw.WriteLine("{0}struct /* __EXECUTION_FRAME__ */", indent);
                tw.WriteLine("{0}{{", indent);
                tw.WriteLine("{0}{0}__EXECUTION_FRAME__* pNext;", indent);
                tw.WriteLine("{0}{0}uint8_t targetCount;", indent);

                foreach (var frameEntry in frameEntries)
                {
                    tw.WriteLine(
                        "{0}{0}{1}* p{2};",
                        indent,
                        translateContext.GetCLanguageTypeName(frameEntry.Type),
                        frameEntry.Name);
                }

                tw.WriteLine("{0}}} __executionFrame__;", indent);
                tw.WriteLine();
                tw.WriteLine("{0}__executionFrame__.targetCount = {1};", indent, frameEntries.Length);

                foreach (var frameEntry in frameEntries)
                {
                    tw.WriteLine(
                        "{0}__executionFrame__.p{1} = &{1};",
                        indent,
                        frameEntry.Name);
                }

                tw.WriteLine("{0}__gc_link_execution_frame__(&__executionFrame__);", indent);
            }

            tw.WriteLine();
            tw.WriteLine("{0}//-------------------", indent);
            tw.WriteLine("{0}// IL body:", indent);
            tw.WriteLine();

            foreach (var entry in bodySourceCode)
            {
                if (decodeContext.TryGetLabelName(
                    entry.Label, out var labelName))
                {
                    tw.WriteLine("{0}:", labelName);
                }

                foreach (var sourceCode in entry.SourceCode)
                {
                    // Dirty hack:
                    if (sourceCode.StartsWith("return")
                        && (frameEntries.Length >= 1))
                    {
                        tw.WriteLine(
                            "{0}__gc_unlink_execution_frame__(&__executionFrame__);",
                            indent);
                    }

                    tw.WriteLine(
                        "{0}{1};",
                        indent,
                        sourceCode);
                }
            }

            tw.WriteLine("}");
        }

        private static void InternalConvert(
            TranslateContext translateContext,
            TextWriter tw,
            string methodName,
            Type returnType,
            Parameter[] parameters,
            string targetName,
            DllImportAttribute dllImportAttribute,
            string indent)
        {
            // TODO: Switch DllImport.Value include direction to library direction.
            translateContext.RegisterPrivateIncludeFile(dllImportAttribute.Value);

            var functionPrototype = Utilities.GetFunctionPrototypeString(
                methodName,
                returnType,
                parameters,
                translateContext);

            tw.WriteLine();
            tw.WriteLine("///////////////////////////////////////");
            tw.WriteLine("// P/Invoke: {0}", methodName);
            tw.WriteLine();

            tw.WriteLine(functionPrototype);
            tw.WriteLine("{");

            var realTargetName = string.IsNullOrWhiteSpace(dllImportAttribute.EntryPoint)
                ? targetName
                : dllImportAttribute.EntryPoint;
            var arguments = string.Join(
                ", ",
                parameters.Select(parameter => parameter.Name));

            if (returnType != typeof(void))
            {
                tw.WriteLine("{0}return {1}({2});", indent, realTargetName, arguments);
            }
            else
            {
                tw.WriteLine("{0}{1}({2});", indent, realTargetName, arguments);
            }

            tw.WriteLine("}");
        }
    }
}
