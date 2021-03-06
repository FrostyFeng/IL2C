﻿using System;
using System.Linq;

using Mono.Cecil.Cil;

using IL2C.Translators;
using IL2C.Metadata;

namespace IL2C.ILConverters
{
    internal sealed class LdftnConverter : InlineMethodConverter
    {
        public override OpCode OpCode => OpCodes.Ldftn;

        public override Func<IExtractContext, string[]> Apply(
            IMethodInformation operand, DecodeContext decodeContext)
        {
            var symbol = decodeContext.PushStack(decodeContext.PrepareContext.MetadataContext.IntPtrType);

            // Register callee method declaring type (at the file scope).
            decodeContext.PrepareContext.RegisterType(operand.DeclaringType, decodeContext.Method);

            return extractContext => new[] { string.Format(
                "{0} = (intptr_t){1}",
                extractContext.GetSymbolName(symbol),
                operand.CLanguageFunctionName) };
        }
    }

    internal sealed class LdvirtftnConverter : InlineMethodConverter
    {
        public override OpCode OpCode => OpCodes.Ldvirtftn;

        public override Func<IExtractContext, string[]> Apply(
            IMethodInformation method, DecodeContext decodeContext)
        {
            // ECMA-335 III.4.18: ldvirtfn - load a virtual method pointer

            if (method.IsStatic)
            {
                throw new InvalidProgramSequenceException(
                    "Invalid method token (static): Location={0}, Method={1}",
                    decodeContext.CurrentCode.RawLocation,
                    method.FriendlyName);
            }

            var si = decodeContext.PopStack();
            if (!method.DeclaringType.IsAssignableFrom(si.TargetType))
            {
                throw new InvalidProgramSequenceException(
                    "Invalid method token (not a member): Location={0}, Method={1}",
                    decodeContext.CurrentCode.RawLocation,
                    method.FriendlyName);
            }

            var symbol = decodeContext.PushStack(
                decodeContext.PrepareContext.MetadataContext.IntPtrType);

            // Register callee method declaring type (at the file scope).
            decodeContext.PrepareContext.RegisterType(method.DeclaringType, decodeContext.Method);

            if (method.IsVirtual && !method.IsSealed)
            {
                // TODO: interface member vptr
                var virtualMethods =
                    method.DeclaringType.CalculatedVirtualMethods;
                var (m, overloadIndex) =
                    virtualMethods.First(entry => entry.method.Equals(method));

                var vptrName = m.GetCLanguageDeclarationName(overloadIndex);

                return extractContext => new[] { string.Format(
                    "{0} = (intptr_t){1}->vptr0__->{2}",
                    extractContext.GetSymbolName(symbol),
                    extractContext.GetSymbolName(si),
                    vptrName) };
            }
            else
            {
                return extractContext => new[] { string.Format(
                    "{0} = (intptr_t){1}",
                    extractContext.GetSymbolName(symbol),
                    method.CLanguageFunctionName) };
            }
        }
    }
}
