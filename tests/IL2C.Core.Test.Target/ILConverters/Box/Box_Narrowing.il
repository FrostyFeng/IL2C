﻿.class public IL2C.ILConverters.Box_Narrowing
{
    .method public static object Box_Int32ToByte(int32 v) cil managed
    {
        .maxstack 1
        ldarg.0
        box [mscorlib]System.Byte
        ret
    }

    .method public static object Box_Int32ToInt16(int32 v) cil managed
    {
        .maxstack 1
        ldarg.0
        box [mscorlib]System.Int16
        ret
    }

    .method public static object Box_Int32ToBool(int32 v) cil managed
    {
        .maxstack 1
        ldarg.0
        box [mscorlib]System.Boolean
        ret
    }

    .method public static object Box_Int32ToChar(int32 v) cil managed
    {
        .maxstack 1
        ldarg.0
        box [mscorlib]System.Char
        ret
    }

    .method public static object Box_Int64ToUInt32(int64 v) cil managed
    {
        .maxstack 1
        ldarg.0
        box [mscorlib]System.UInt32
        ret
    }
}
