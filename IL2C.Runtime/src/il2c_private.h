// It uses for internal purpose only.

#ifndef __IL2C_PRIVATE_H__
#define __IL2C_PRIVATE_H__

#pragma once

// TODO:
#define IL2C_USE_RUNTIME_GIANT_LOCK

#if defined(_DEBUG)
#define IL2C_USE_LINE_INFORMATION
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile long interlock_t;

///////////////////////////////////////////////////
// Internal depended definitions

#include "Private/msvc_win32.h"
#include "Private/msvc_uefi.h"
#include "Private/msvc_wdm.h"
#include "Private/gcc_win32.h"
#include "Private/gcc_linux.h"
#include "Private/arduino_all.h"

#if defined(IL2C_USE_RUNTIME_DEBUG_LOG)
extern void il2c_runtime_debug_log(const wchar_t* message);
extern void il2c_runtime_debug_log_format(const wchar_t* format, ...);
#else
#define il2c_runtime_debug_log(message)
#define il2c_runtime_debug_log_format(format, ...)
#endif

extern void il2c_initialize__(void);
extern void il2c_shutdown__(void);

#if defined(IL2C_USE_LINE_INFORMATION)
extern void il2c_collect__(const char* pFile, int line);
#define il2c_collect() il2c_collect__(__FILE__, __LINE__)
#else
extern void il2c_collect__(void);
#define il2c_collect() il2c_collect__()
#endif

///////////////////////////////////////////////////
// il2c.h

#include <il2c.h>

///////////////////////////////////////////////////
// Internal runtime definitions

typedef volatile struct IL2C_GC_TRACKING_INFORMATION_DECL
{
    IL2C_GC_TRACKING_INFORMATION* pNext__;
    const uint16_t objRefCount__;
    const uint16_t valueCount__;
    volatile void* pReferences__[1];     // objRefCount__
    // IL2C_VALUE_DESCRIPTOR valueDescriptors__[];  // valueCount__
} IL2C_GC_TRACKING_INFORMATION;

typedef IL2C_GC_TRACKING_INFORMATION IL2C_EXECUTION_FRAME;
typedef IL2C_GC_TRACKING_INFORMATION IL2C_STATIC_FIELDS;

typedef const struct IL2C_MARK_TARGET_DECL
{
    const IL2C_RUNTIME_TYPE valueType;
    const uintptr_t offset;
} IL2C_MARK_TARGET;

typedef const struct IL2C_IMPLEMENTED_INTERFACE_DECL
{
    const IL2C_RUNTIME_TYPE type;
    const void* vptr0;
} IL2C_IMPLEMENTED_INTERFACE;

struct IL2C_RUNTIME_TYPE_DECL
{
    const char* pTypeName;
    const uintptr_t flags;
    const uintptr_t bodySize;       // uint32_t
    const IL2C_RUNTIME_TYPE baseType;
    const void* vptr0;
    const uintptr_t markTarget;     // mark target count / custom mark handler (only variable type)
    const uintptr_t interfaceCount;
    //IL2C_MARK_TARGET markTargets[markTarget];
    //IL2C_IMPLEMENTED_INTERFACE interfaces[interfaceCount];
};

// TODO: shrink for interface types
//struct IL2C_RUNTIME_TYPE_DECL
//{
//    const char* pTypeName;
//    const uintptr_t flags;
//    const uintptr_t interfaceCount;
// ---------------------------------------
//    const uintptr_t bodySize;       // uint32_t
//    const IL2C_RUNTIME_TYPE baseType;
//    const void* vptr0;
//    const uintptr_t markTarget;     // mark target count / custom mark handler (only variable type)
//    //IL2C_IMPLEMENTED_INTERFACE interfaces[interfaceCount];
//    //const void* markTargets[markTarget];
//};

// IL2C_REF_HEADER_DECL.characteristic
#define IL2C_CHARACTERISTIC_ACQUIRED_MONITOR_LOCK ((interlock_t)0x08000000UL)
#define IL2C_CHARACTERISTIC_SUPPRESS_FINALIZE ((interlock_t)0x10000000UL)
#define IL2C_CHARACTERISTIC_MARK_INDEX ((interlock_t)0x20000000UL)      // Mark index is only 0 or 1.
#define IL2C_CHARACTERISTIC_INITIALIZED ((interlock_t)0x40000000UL)     // GC will ignore if not initialized
#define IL2C_CHARACTERISTIC_CONST ((interlock_t)0x80000000UL)

#define il2c_get_header__(pReference) \
    ((IL2C_REF_HEADER*)(((uint8_t*)(pReference)) - sizeof(IL2C_REF_HEADER)))

// Generator macro for the trampoline virtual function using the value type.
// These are using the unsafe_unbox. Because we can understand what type the this__ pointer,
// these function only invoke from the (known value type) trampoline vtable.
#define IL2C_DECLARE_TRAMPOLINE_VFUNC_FOR_VALUE_TYPE(typeName) \
static System_String* typeName##_ToString_Trampoline_VFunc__(System_ValueType* this__) \
{ \
    il2c_assert(this__ != NULL); \
 \
    typeName* pValue = il2c_unsafe_unbox__(this__, typeName); \
    return typeName##_ToString(pValue); \
} \
 \
static int32_t typeName##_GetHashCode_Trampoline_VFunc__(System_ValueType* this__) \
{ \
    il2c_assert(this__ != NULL); \
 \
    typeName* pValue = il2c_unsafe_unbox__(this__, typeName); \
    return typeName##_GetHashCode(pValue); \
} \
 \
static bool typeName##_Equals_1_Trampoline_VFunc__(System_ValueType* this__, System_Object* obj) \
{ \
    il2c_assert(this__ != NULL); \
 \
    typeName* pValue = il2c_unsafe_unbox__(this__, typeName); \
    return typeName##_Equals_1(pValue, obj); \
}

// Generator macro for the trampoline virtual function table using the value type.
#define IL2C_DECLARE_TRAMPOLINE_VTABLE_FOR_VALUE_TYPE(typeName) \
typeName##_VTABLE_DECL__ typeName##_VTABLE__ = { \
    0, \
    (bool(*)(void*, System_Object*))typeName##_Equals_1_Trampoline_VFunc__, \
    (void(*)(void*))System_Object_Finalize, \
    (int32_t(*)(void*))typeName##_GetHashCode_Trampoline_VFunc__, \
    (System_String* (*)(void*))typeName##_ToString_Trampoline_VFunc__ \
}

///////////////////////////////////////////////////
// Internal implements required additional headers

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

///////////////////////////////////////////////////
// Internal runtime functions

#if defined(IL2C_USE_LINE_INFORMATION)
extern IL2C_REF_HEADER* il2c_get_uninitialized_object_internal__(
    IL2C_RUNTIME_TYPE type, uintptr_t bodySize, IL2C_MONITOR_LOCK* pLock, const char* pFile, int line);
#else
extern IL2C_REF_HEADER* il2c_get_uninitialized_object_internal__(
    IL2C_RUNTIME_TYPE type, uintptr_t bodySize, IL2C_MONITOR_LOCK* pLock);
#endif

extern void il2c_register_root_reference__(void* pReference, bool isFixed);
extern void il2c_unregister_root_reference__(void* pReference, bool isFixed);

extern void il2c_default_mark_handler_for_objref__(void* pReference);
extern void il2c_default_mark_handler_for_value_type__(void* pValue, IL2C_RUNTIME_TYPE valueType);
extern void il2c_default_mark_handler_for_tracking_information__(IL2C_GC_TRACKING_INFORMATION* pTrackingInformation);

typedef volatile struct IL2C_RUNTIME_THREAD_BOTTOM_EXECUTION_FRAME /* IL2C_EXECUTION_FRAME */
{
    IL2C_EXECUTION_FRAME* pNext__;
    uint16_t objRefCount__;
    uint16_t valueCount__;
    System_Exception* exception__;
} IL2C_RUNTIME_THREAD_BOTTOM_EXECUTION_FRAME;

typedef volatile struct IL2C_THREAD_CONTEXT_DECL
{
    IL2C_EXECUTION_FRAME* pFrame;
    IL2C_EXCEPTION_FRAME* pUnwindTarget;
    intptr_t rawHandle;
    IL2C_MONITOR_LOCK lockForCollect;
    int32_t id;
} IL2C_THREAD_CONTEXT;

// The real thread structure.
typedef volatile struct IL2C_RUNTIME_THREAD
{
    System_Threading_Thread thread;
    IL2C_THREAD_CONTEXT context;
} IL2C_RUNTIME_THREAD;

typedef volatile struct IL2C_RUNTIME_CREATED_THREAD
{
    System_Threading_Thread thread;
    IL2C_THREAD_CONTEXT context;
    System_Object* parameter;
    IL2C_RUNTIME_THREAD_BOTTOM_EXECUTION_FRAME bottomFrame;
} IL2C_RUNTIME_CREATED_THREAD;

#if defined(IL2C_USE_RUNTIME_GIANT_LOCK)
extern IL2C_MONITOR_LOCK g_GlobalLockForCollect__;
#define IL2C_THREAD_LOCK_TARGET(pThreadContext) (((void)pThreadContext->lockForCollect), (&g_GlobalLockForCollect__))
#else
#define IL2C_THREAD_LOCK_TARGET(pThreadContext) (&pThreadContext->lockForCollect)
#endif

#if defined(IL2C_USE_LINE_INFORMATION)
IL2C_THREAD_CONTEXT* il2c_acquire_thread_context__(const char* pFile, int line);
#else
IL2C_THREAD_CONTEXT* il2c_acquire_thread_context__(void);
#endif

IL2C_MONITOR_LOCK* il2c_acquire_monitor_lock_from_objref__(void* pReference, bool allocateIfRequired);

///////////////////////////////////////////////////////////////////
// TODO: move defs

extern void il2c_write(const wchar_t* s);
extern void il2c_writeline(const wchar_t* s);
extern bool il2c_readline(wchar_t* buffer, int32_t length);

#ifdef __cplusplus
}
#endif

#endif
