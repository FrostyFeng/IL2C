﻿using System;

using Mono.Cecil.Cil;

using IL2C.Metadata;
using IL2C.Translators;

namespace IL2C.ILConverters
{
    internal static class StlocConverterUtilities
    {
        private static Func<IExtractContext, string[]> Apply(
            ILocalVariableInformation target,
            ITypeInformation targetType,
            DecodeContext decodeContext)
        {
            var si = decodeContext.PopStack();

            var codeInformation = decodeContext.CurrentCode;

            return extractContext =>
            {
                var rightExpression = extractContext.GetRightExpression(targetType, si);
                if (rightExpression == null)
                {
                    throw new InvalidProgramSequenceException(
                        "Invalid store operation: Location={0}, StackType={1}, LocalType={2}, SymbolName={3}",
                        codeInformation.RawLocation,
                        si.TargetType.FriendlyName,
                        targetType.FriendlyName,
                        target);
                }

                return new[] { string.Format(
                    "{0} = {1}",
                    extractContext.GetSymbolName(target),
                    rightExpression) };
            };
        }

        public static Func<IExtractContext, string[]> Apply(
            int localIndex,
            DecodeContext decodeContext)
        {
            var local = decodeContext.Method.LocalVariables[localIndex];
            return Apply(local, local.TargetType, decodeContext);
        }

        public static Func<IExtractContext, string[]> Apply(
            VariableInformation localVariable,
            DecodeContext decodeContext)
        {
            var local = decodeContext.Method.LocalVariables[localVariable.Index];
            return Apply(local, local.TargetType, decodeContext);
        }
    }

    internal sealed class Stloc_0Converter : InlineNoneConverter
    {
        public override OpCode OpCode => OpCodes.Stloc_0;

        public override Func<IExtractContext, string[]> Apply(DecodeContext decodeContext)
        {
            return StlocConverterUtilities.Apply(0, decodeContext);
        }
    }

    internal sealed class Stloc_1Converter : InlineNoneConverter
    {
        public override OpCode OpCode => OpCodes.Stloc_1;

        public override Func<IExtractContext, string[]> Apply(DecodeContext decodeContext)
        {
            return StlocConverterUtilities.Apply(1, decodeContext);
        }
    }

    internal sealed class Stloc_2Converter : InlineNoneConverter
    {
        public override OpCode OpCode => OpCodes.Stloc_2;

        public override Func<IExtractContext, string[]> Apply(DecodeContext decodeContext)
        {
            return StlocConverterUtilities.Apply(2, decodeContext);
        }
    }

    internal sealed class Stloc_3Converter : InlineNoneConverter
    {
        public override OpCode OpCode => OpCodes.Stloc_3;

        public override Func<IExtractContext, string[]> Apply(DecodeContext decodeContext)
        {
            return StlocConverterUtilities.Apply(3, decodeContext);
        }
    }

    internal sealed class Stloc_SConverter : ShortInlineVarConverter
    {
        public override OpCode OpCode => OpCodes.Stloc_S;

        public override Func<IExtractContext, string[]> Apply(
            VariableInformation operand, DecodeContext decodeContext)
        {
            return StlocConverterUtilities.Apply(operand, decodeContext);
        }
    }
}
