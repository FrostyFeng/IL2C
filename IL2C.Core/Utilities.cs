﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Cecil.Rocks;

using IL2C.ILConveters;
using IL2C.Translators;

namespace IL2C
{
    internal static class Utilities
    {
        private static readonly Dictionary<OpCode, ILConverter> ilConverters;

        static Utilities()
        {
            ilConverters = typeof(ILConverter)
                .Assembly
                .GetTypes()
                .Where(type => type.IsSealed && typeof(ILConverter).IsAssignableFrom(type))
                .Select(type => (ILConverter)Activator.CreateInstance(type))
                .ToDictionary(ilc => ilc.OpCode);
        }

        public static bool TryGetILConverter(OpCode opCode, out ILConverter ilc)
        {
            return ilConverters.TryGetValue(opCode, out ilc);
        }

        public static TypeReference GetStackableType(TypeReference type)
        {
            if (type.MemberEquals(CecilHelper.ByteType)
                || type.MemberEquals(CecilHelper.SByteType)
                || type.MemberEquals(CecilHelper.Int16Type)
                || type.MemberEquals(CecilHelper.UInt16Type)
                || type.MemberEquals(CecilHelper.BooleanType))
            {
                return CecilHelper.Int32Type;
            }

            return type;
        }

        public static TypeDefinition GetStackableType(TypeDefinition type)
        {
            if (type.MemberEquals(CecilHelper.ByteType)
                || type.MemberEquals(CecilHelper.SByteType)
                || type.MemberEquals(CecilHelper.Int16Type)
                || type.MemberEquals(CecilHelper.UInt16Type))
            {
                return CecilHelper.Int32Type;
            }

            return type;
        }

        public static Parameter[] GetSafeParameters(this MethodReference method)
        {
            var parameters = method.Parameters
                .Select(parameter => new Parameter(parameter.Name, parameter.ParameterType));
            if (method.HasThis)
            {
                TypeReference type = method.DeclaringType;
                var thisType = type.IsValueType ? type.MakeByReferenceType() : type;
                parameters = new[]
                    {
                        new Parameter("__this", thisType)
                    }
                    .Concat(parameters);
            }

            return parameters.ToArray();
        }

        public static string ManglingSymbolName(this string rawSymbolName)
        {
            return rawSymbolName
                .Replace('.', '_')
                .Replace("*", "_reference");
        }

        public static string GetFunctionPrototypeString(
            string methodName,
            TypeReference returnType,
            Parameter[] parameters,
            IExtractContext extractContext)
        {
            var parametersString = String.Join(
                ", ",
                parameters.Select(parameter => String.Format(
                    "{0} {1}",
                    extractContext.GetCLanguageTypeName(parameter.ParameterType),
                    parameter.Name)));

            var returnTypeName =
                extractContext.GetCLanguageTypeName(returnType);

            return String.Format(
                "{0} {1}({2})",
                returnTypeName,
                methodName.ManglingSymbolName(),
                (parametersString.Length >= 1) ? parametersString : "void");
        }

        public static string GetStaticFieldPrototypeString(
            FieldReference field,
            bool requireInitializerExpression,
            IExtractContext extractContext)
        {
            var initializer = String.Empty;
            if (requireInitializerExpression)
            {
                if (field.FieldType.IsNumericPrimitive())
                {
                    // TODO: numericPrimitive and (literal or readonly static) ?
                    var fieldDefinition = field.Resolve();
                    Debug.Assert(fieldDefinition.IsStatic);
                    var value = fieldDefinition.HasConstant ? fieldDefinition.Constant : 0;

                    Debug.Assert(value != null);

                    initializer = (fieldDefinition.MemberEquals(CecilHelper.Int64Type))
                        ? String.Format(" = {0}LL", value)
                        : String.Format(" = {0}", value);
                }
                else if (field.FieldType.IsValueType == false)
                {
                    initializer = " = NULL";
                }
            }

            return string.Format(
                "{0} {1}{2}",
                extractContext.GetCLanguageTypeName(field.FieldType),
                field.GetFullMemberName().ManglingSymbolName(),
                initializer);
        }

        public struct RightExpressionGivenParameter
        {
            public readonly TypeReference TargetType;
            public readonly SymbolInformation SymbolInformation;

            public RightExpressionGivenParameter(
                TypeReference targetType, SymbolInformation symbolinformation)
            {
                this.TargetType = targetType;
                this.SymbolInformation = symbolinformation;
            }
        }

        public static string GetGivenParameterDeclaration(
            RightExpressionGivenParameter[] parameters,
            IExtractContext extractContext,
            int offset)
        {
            return string.Join(", ", parameters.Select(entry =>
            {
                var rightExpression = extractContext.GetRightExpression(
                    entry.TargetType, entry.SymbolInformation);
                if (rightExpression == null)
                {
                    throw new InvalidProgramSequenceException(
                        "Invalid parameter type: Offset={0}, StackType={1}, ParameterType={2}",
                        offset,
                        entry.SymbolInformation.TargetType.FullName,
                        entry.TargetType.FullName);
                }
                return rightExpression;
            }));
        }

        public static IEnumerable<T> Traverse<T>(this T first, Func<T, T> next, bool invokeNextFirst = false)
            where T : class
        {
            T current = first;
            if (invokeNextFirst)
            {
                if (current != null)
                {
                    while (true)
                    {
                        current = next(current);
                        if (current == null)
                        {
                            break;
                        }
                        yield return current;
                    }
                }
            }
            else
            {
                while (current != null)
                {
                    yield return current;
                    current = next(current);
                }
            }
        }

        public static void ForEach<T>(this IEnumerable<T> enumerable, Action<T> action)
        {
            foreach (var value in enumerable)
            {
                action(value);
            }
        }
    }
}
