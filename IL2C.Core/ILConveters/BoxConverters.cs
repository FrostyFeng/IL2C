﻿using System;

using Mono.Cecil.Cil;

using IL2C.Metadata;
using IL2C.Translators;

namespace IL2C.ILConverters
{
    internal sealed class BoxConverter : InlineTypeConverter
    {
        public override OpCode OpCode => OpCodes.Box;

        public override Func<IExtractContext, string[]> Apply(
            ITypeInformation operand, DecodeContext decodeContext)
        {
            var si = decodeContext.PopStack();
            if (!(si.TargetType.IsValueType &&
                ((operand.IsInt32StackFriendlyType && si.TargetType.IsInt32StackFriendlyType) ||
                (operand.IsInt64StackFriendlyType && si.TargetType.IsInt64StackFriendlyType) ||
                (operand.IsIntPtrStackFriendlyType && si.TargetType.IsIntPtrStackFriendlyType))))
            {
                throw new InvalidProgramSequenceException(
                    "Invalid type at stack: Location={0}, TokenType={1}, StackType={2}",
                    decodeContext.CurrentCode.RawLocation,
                    operand.FriendlyName,
                    si.TargetType.FriendlyName);
            }

            var symbolName = decodeContext.PushStack(
                decodeContext.PrepareContext.MetadataContext.ObjectType);

            if (operand.Equals(si.TargetType))
            {
                return _ =>
                {
                    return new[] { string.Format(
                    "{0} = il2c_box1(&{1}, {2})",
                    symbolName,
                    si.SymbolName,
                    operand.MangledName) };
                };
            }
            else
            {
                return _ =>
                {
                    return new[] { string.Format(
                    "{0} = il2c_box2(&{1}, {2}, {3})",
                    symbolName,
                    si.SymbolName,
                    operand.MangledName,
                    si.TargetType.MangledName) };
                };
            }
        }
    }

    internal sealed class Unbox_AnyConverter : InlineTypeConverter
    {
        public override OpCode OpCode => OpCodes.Unbox_Any;

        public override Func<IExtractContext, string[]> Apply(
            ITypeInformation operand, DecodeContext decodeContext)
        {
            var si = decodeContext.PopStack();

            if (!si.TargetType.IsObjectType)
            {
                throw new InvalidProgramSequenceException(
                    "Invalid type at stack: Location={0}, TokenType={1}, StackType={2}",
                    decodeContext.CurrentCode.RawLocation,
                    operand.FriendlyName,
                    si.TargetType.FriendlyName);
            }

            var symbolName = decodeContext.PushStack(operand);

            return extractContext =>
            {
                var rhs = extractContext.GetRightExpression(
                    decodeContext.PrepareContext.MetadataContext.ObjectType, si);

                return new[] { string.Format(
                    "{0} = *(({1}*)il2c_unbox({2}, {3}))",
                    symbolName,
                    operand.CLanguageTypeName,
                    rhs,
                    operand.MangledName) };
            };
        }
    }
}
