﻿// [14-1] This is MT3620Blink native code translated by IL2C, do not edit.

#include <MT3620Blink.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////
// [14-2] Type pre definitions:

////////////////////////////////////////////////////////////
// [2-1-1] .NET types:

/* internal sealed class */ typedef struct MT3620Blink_GpioOutput MT3620Blink_GpioOutput;

////////////////////////////////////////////////////////////
// [2-1-2] VTable types:

typedef MT3620Blink_Descriptor_VTABLE_DECL__ MT3620Blink_GpioOutput_VTABLE_DECL__;

///////////////////////////////////////////////////////////////////////////
// [14-3] Type body definitions:

#ifdef MT3620Blink_DECL_TYPE_BODY__

////////////////////////////////////////////////////////////
// [1] MT3620Blink.GpioOutput

// [1-1-2] Class layout
/* internal sealed class */ struct MT3620Blink_GpioOutput
{
    MT3620Blink_GpioOutput_VTABLE_DECL__* vptr0__;
    System_IDisposable_VTABLE_DECL__* vptr_System_IDisposable__;
    int32_t baseField1__;
};

// [1-5-1] VTable (Same as MT3620Blink.Descriptor)
#define MT3620Blink_GpioOutput_VTABLE__ MT3620Blink_Descriptor_VTABLE__

// [1-4] Runtime type information
IL2C_DECLARE_RUNTIME_TYPE(MT3620Blink_GpioOutput);

//////////////////////////////////////////////////////////////////////////////////
// [2-3] Methods: MT3620Blink.GpioOutput

extern /* public sealed */ void MT3620Blink_GpioOutput__ctor(MT3620Blink_GpioOutput* this__, int32_t gpioId, MT3620Blink_GPIO_OutputMode_Type type, bool initialValue);
extern /* public sealed */ void MT3620Blink_GpioOutput_SetValue(MT3620Blink_GpioOutput* this__, bool value);

#endif

#ifdef __cplusplus
}
#endif
