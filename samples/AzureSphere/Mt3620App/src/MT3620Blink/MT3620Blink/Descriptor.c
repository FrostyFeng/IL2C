﻿// [15-2] This is MT3620Blink native code translated by IL2C, do not edit.

#include <MT3620Blink.h>
#include <MT3620Blink_internal.h>

#include <mscorlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////
// [9-1] Type: MT3620Blink.Descriptor

//////////////////////////////////////////////////////////////////////////////////
// [9-2] File scope prototypes:

//////////////////////////////////////////////////////////////////////////////////
// [2-3] Methods: MT3620Blink.Descriptor

extern /* private */ void MT3620Blink_Descriptor_set_Identity(MT3620Blink_Descriptor* this__, int32_t value);

//////////////////////////////////////////////////////////////////////////////////
// [9-4] Type: MT3620Blink.Descriptor

///////////////////////////////////////
// [3] MT3620Blink.Descriptor..ctor(MT3620Blink.Descriptor this__, System.Int32 fd)

//-------------------
// [3-7] Declare execution frame:

typedef struct MT3620Blink_Descriptor__ctor_EXECUTION_FRAME_DECL
{
    const IL2C_EXECUTION_FRAME* pNext__;
    const uint16_t objRefCount__;
    const uint16_t valueCount__;
    //-------------------- objref
    MT3620Blink_Descriptor* stack0_0__;
    System_String* stack0_2__;
    System_Exception* stack0_3__;
    il2c_boxedtype(System_Int32)* stack1_1__;
} MT3620Blink_Descriptor__ctor_EXECUTION_FRAME__;

//-------------------
// [3-2] Function body:

void MT3620Blink_Descriptor__ctor(MT3620Blink_Descriptor* this__, int32_t fd)
{
    il2c_assert(this__ != NULL);

    //-------------------
    // [3-4] Evaluation stacks (!objref):

    int32_t stack0_1__;
    int32_t stack1_0__;

    //-------------------
    // [3-5] Setup execution frame:

    MT3620Blink_Descriptor__ctor_EXECUTION_FRAME__ frame__ =
        { NULL, 4 };
    il2c_link_execution_frame(&frame__);

    //-------------------
    // [3-6] IL body:

    /* IL_0000: ldarg.0                         : Descriptor.cs(7) */
    frame__.stack0_0__ = this__;
    /* IL_0001: call System.Object..ctor        : Descriptor.cs(7) */
    System_Object__ctor((System_Object*)frame__.stack0_0__);
    /* IL_0006: ldarg.1                         : Descriptor.cs(9) */
    stack0_1__ = fd;
    /* IL_0007: ldc.i4.0                        : Descriptor.cs(9) */
    stack1_0__ = 0;
    /* IL_0008: bge.s IL_0020                   : Descriptor.cs(9) */
    if (stack0_1__ >= stack1_0__) goto IL_0020;
    /* IL_000a: ldstr "Invalid descriptor: "    : Descriptor.cs(11) */
    frame__.stack0_2__ = MT3620Blink_string0__;
    /* IL_000f: ldarg.1                         : Descriptor.cs(11) */
    stack1_0__ = fd;
    /* IL_0010: box System.Int32                : Descriptor.cs(11) */
    frame__.stack1_1__ = il2c_box(&stack1_0__, System_Int32);
    /* IL_0015: call System.String.Concat       : Descriptor.cs(11) */
    frame__.stack0_2__ = System_String_Concat_4((System_Object*)frame__.stack0_2__, (System_Object*)frame__.stack1_1__);
    /* IL_001a: newobj System.Exception..ctor   : Descriptor.cs(11) */
    frame__.stack0_3__ = il2c_get_uninitialized_object(System_Exception);
    System_Exception__ctor_1(frame__.stack0_3__, frame__.stack0_2__);
    /* IL_001f: throw                           : Descriptor.cs(11) */
    il2c_throw(frame__.stack0_3__);
IL_0020:
    /* IL_0020: ldarg.0                         : Descriptor.cs(13) */
    frame__.stack0_0__ = this__;
    /* IL_0021: ldarg.1                         : Descriptor.cs(13) */
    stack1_0__ = fd;
    /* IL_0022: call MT3620Blink.Descriptor.set_Identity : Descriptor.cs(13) */
    MT3620Blink_Descriptor_set_Identity(frame__.stack0_0__, stack1_0__);
    /* IL_0027: ret                             : Descriptor.cs(14) */
    il2c_unlink_execution_frame(&frame__);
    return;
}

///////////////////////////////////////
// [3] Virtual: MT3620Blink.Descriptor.Dispose(MT3620Blink.Descriptor this__)

//-------------------
// [3-7] Declare execution frame:

typedef struct MT3620Blink_Descriptor_Dispose_EXECUTION_FRAME_DECL
{
    const IL2C_EXECUTION_FRAME* pNext__;
    const uint16_t objRefCount__;
    const uint16_t valueCount__;
    //-------------------- objref
    MT3620Blink_Descriptor* stack0_0__;
} MT3620Blink_Descriptor_Dispose_EXECUTION_FRAME__;

//-------------------
// [3-2] Function body:

void MT3620Blink_Descriptor_Dispose(MT3620Blink_Descriptor* this__)
{
    il2c_assert(this__ != NULL);

    //-------------------
    // [3-4] Evaluation stacks (!objref):

    int32_t stack0_1__;
    int32_t stack1_0__;

    //-------------------
    // [3-5] Setup execution frame:

    MT3620Blink_Descriptor_Dispose_EXECUTION_FRAME__ frame__ =
        { NULL, 1 };
    il2c_link_execution_frame(&frame__);

    //-------------------
    // [3-6] IL body:

    /* IL_0000: ldarg.0                         : Descriptor.cs(18) */
    frame__.stack0_0__ = this__;
    /* IL_0001: call MT3620Blink.Descriptor.get_Identity : Descriptor.cs(18) */
    stack0_1__ = MT3620Blink_Descriptor_get_Identity(frame__.stack0_0__);
    /* IL_0006: ldc.i4.0                        : Descriptor.cs(18) */
    stack1_0__ = 0;
    /* IL_0007: blt.s IL_001c                   : Descriptor.cs(18) */
    if (stack0_1__ < stack1_0__) goto IL_001c;
    /* IL_0009: ldarg.0                         : Descriptor.cs(20) */
    frame__.stack0_0__ = this__;
    /* IL_000a: call MT3620Blink.Descriptor.get_Identity : Descriptor.cs(20) */
    stack0_1__ = MT3620Blink_Descriptor_get_Identity(frame__.stack0_0__);
    /* IL_000f: call MT3620Blink.Interops.close : Descriptor.cs(20) */
    stack0_1__ = MT3620Blink_Interops_close(stack0_1__);
    /* IL_0014: pop                             : Descriptor.cs(20) */
    /* IL_0015: ldarg.0  */
    frame__.stack0_0__ = this__;
    /* IL_0016: ldc.i4.m1                       : Descriptor.cs(21) */
    stack1_0__ = -1;
    /* IL_0017: call MT3620Blink.Descriptor.set_Identity : Descriptor.cs(21) */
    MT3620Blink_Descriptor_set_Identity(frame__.stack0_0__, stack1_0__);
IL_001c:
    /* IL_001c: ret                             : Descriptor.cs(23) */
    il2c_unlink_execution_frame(&frame__);
    return;
}

///////////////////////////////////////
// [3] MT3620Blink.Descriptor.get_Identity(MT3620Blink.Descriptor this__)

//-------------------
// [3-7] Declare execution frame:

typedef struct MT3620Blink_Descriptor_get_Identity_EXECUTION_FRAME_DECL
{
    const IL2C_EXECUTION_FRAME* pNext__;
    const uint16_t objRefCount__;
    const uint16_t valueCount__;
    //-------------------- objref
    MT3620Blink_Descriptor* stack0_0__;
} MT3620Blink_Descriptor_get_Identity_EXECUTION_FRAME__;

//-------------------
// [3-2] Function body:

int32_t MT3620Blink_Descriptor_get_Identity(MT3620Blink_Descriptor* this__)
{
    il2c_assert(this__ != NULL);

    //-------------------
    // [3-4] Evaluation stacks (!objref):

    int32_t stack0_1__;

    //-------------------
    // [3-5] Setup execution frame:

    MT3620Blink_Descriptor_get_Identity_EXECUTION_FRAME__ frame__ =
        { NULL, 1 };
    il2c_link_execution_frame(&frame__);

    //-------------------
    // [3-6] IL body:

    /* IL_0000: ldarg.0                         : Descriptor.cs(25) */
    frame__.stack0_0__ = this__;
    /* IL_0001: ldfld MT3620Blink.Descriptor.<Identity>k__BackingField : Descriptor.cs(25) */
    stack0_1__ = frame__.stack0_0__->_Identity_k__BackingField;
    /* IL_0006: ret                             : Descriptor.cs(25) */
    il2c_unlink_execution_frame(&frame__);
    return stack0_1__;
}

///////////////////////////////////////
// [3] MT3620Blink.Descriptor.set_Identity(MT3620Blink.Descriptor this__, System.Int32 value)

//-------------------
// [3-7] Declare execution frame:

typedef struct MT3620Blink_Descriptor_set_Identity_EXECUTION_FRAME_DECL
{
    const IL2C_EXECUTION_FRAME* pNext__;
    const uint16_t objRefCount__;
    const uint16_t valueCount__;
    //-------------------- objref
    MT3620Blink_Descriptor* stack0_0__;
} MT3620Blink_Descriptor_set_Identity_EXECUTION_FRAME__;

//-------------------
// [3-2] Function body:

void MT3620Blink_Descriptor_set_Identity(MT3620Blink_Descriptor* this__, int32_t value)
{
    il2c_assert(this__ != NULL);

    //-------------------
    // [3-4] Evaluation stacks (!objref):

    int32_t stack1_0__;

    //-------------------
    // [3-5] Setup execution frame:

    MT3620Blink_Descriptor_set_Identity_EXECUTION_FRAME__ frame__ =
        { NULL, 1 };
    il2c_link_execution_frame(&frame__);

    //-------------------
    // [3-6] IL body:

    /* IL_0000: ldarg.0                         : Descriptor.cs(25) */
    frame__.stack0_0__ = this__;
    /* IL_0001: ldarg.1                         : Descriptor.cs(25) */
    stack1_0__ = value;
    /* IL_0002: stfld MT3620Blink.Descriptor.<Identity>k__BackingField : Descriptor.cs(25) */
    frame__.stack0_0__->_Identity_k__BackingField = stack1_0__;
    /* IL_0007: ret                             : Descriptor.cs(25) */
    il2c_unlink_execution_frame(&frame__);
    return;
}

//////////////////////
// [7] Runtime helpers:

// [7-10-2] VTable
MT3620Blink_Descriptor_VTABLE_DECL__ MT3620Blink_Descriptor_VTABLE__ = {
    0, // Adjustor offset
    (bool (*)(void*, System_Object*))System_Object_Equals,
    (void (*)(void*))System_Object_Finalize,
    (int32_t (*)(void*))System_Object_GetHashCode,
    (System_String* (*)(void*))System_Object_ToString,
    (void (*)(void*))MT3620Blink_Descriptor_Dispose,
};

// [7-13] VTable for System.IDisposable
System_IDisposable_VTABLE_DECL__ MT3620Blink_Descriptor_System_IDisposable_VTABLE__ = {
    il2c_adjustor_offset(MT3620Blink_Descriptor, System_IDisposable),
    (void (*)(void*))MT3620Blink_Descriptor_Dispose,
};

// [7-8] Runtime type information
IL2C_RUNTIME_TYPE_BEGIN(MT3620Blink_Descriptor, "MT3620Blink.Descriptor", IL2C_TYPE_REFERENCE, sizeof(MT3620Blink_Descriptor), System_Object, 0, 1)
    IL2C_RUNTIME_TYPE_INTERFACE(MT3620Blink_Descriptor, System_IDisposable)
IL2C_RUNTIME_TYPE_END();

#ifdef __cplusplus
}
#endif
