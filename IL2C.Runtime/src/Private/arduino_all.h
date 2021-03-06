// It uses for internal purpose only.

#ifndef __ARDUINO_ALL_H__
#define __ARDUINO_ALL_H__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////
// Arduino

#if defined(__GNUC__) && defined(ARDUINO)

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <alloca.h>

// Compatibility symbols (required platform depended functions)
extern wchar_t* il2c_i32tow(int32_t value, wchar_t* buffer, int radix);
extern wchar_t* il2c_u32tow(uint32_t value, wchar_t* buffer, int radix);
extern wchar_t* il2c_i64tow(int64_t value, wchar_t* buffer, int radix);
extern wchar_t* il2c_u64tow(uint64_t value, wchar_t* buffer, int radix);
#define il2c_snwprintf swprintf
extern long il2c_wtoi32(const wchar_t *nptr, wchar_t **endptr, int base);
#define il2c_wtou32 wcstoul
#define il2c_wtoi64 wcstoll
#define il2c_wtou64 wcstoull
#define il2c_wcstof wcstof
#define il2c_wcstod wcstod
#define il2c_wcscmp wcscmp
#define il2c_wcsicmp wcsicmp
#define il2c_wcslen wcslen
#define il2c_check_heap()

#define il2c_malloc malloc

#if defined(_DEBUG)
extern void il2c_free(void* p);
#else
#define il2c_free free
#endif

#define il2c_mcalloc(elementType, name, size) \
    const uint32_t name_csize__ = (uint32_t)(size); \
    const bool is_name_heaped__ = name_csize__ >= 32U; \
    elementType* name = is_name_heaped__ ? il2c_malloc(name_csize__) : alloca(name_csize__)
#define il2c_mcfree(name) \
    do { if (is_name_heaped__) il2c_free(name); } while (0)

#define il2c_iand(pDest, newValue) __sync_fetch_and_and((interlock_t*)(pDest), (interlock_t)(newValue))
#define il2c_ior(pDest, newValue) __sync_fetch_and_or((interlock_t*)(pDest), (interlock_t)(newValue))
#define il2c_ixor(pDest, newValue) __sync_fetch_and_xor((interlock_t*)(pDest), (interlock_t)(newValue))
#define il2c_iinc(pDest) __sync_add_and_fetch((interlock_t*)(pDest), 1)
#define il2c_idec(pDest) __sync_sub_and_fetch((interlock_t*)(pDest), 1)
#define il2c_ixchg(pDest, newValue) __sync_lock_test_and_set((interlock_t*)(pDest), (interlock_t)(newValue))
#define il2c_ixchgptr(ppDest, pNewValue) __sync_lock_test_and_set((void**)(ppDest), (void*)(pNewValue))
#define il2c_icmpxchg(pDest, newValue, comperandValue) __sync_val_compare_and_swap((interlock_t*)(pDest), (interlock_t)(comperandValue), (interlock_t)(newValue))
#define il2c_icmpxchgptr(ppDest, pNewValue, pComperandValue) __sync_val_compare_and_swap((void**)(ppDest), (void*)(pComperandValue), (void*)(pNewValue))
#define il2c_memory_barrier() __sync_synchronize()

#define il2c_longjmp longjmp

// FreeRTOS
#if defined(portNUM_PROCESSORS)

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern void il2c_sleep(uint32_t milliseconds);

typedef BaseType_t IL2C_TLS_INDEX;
#define il2c_tls_alloc() ((IL2C_TLS_INDEX)((configNUM_THREAD_LOCAL_STORAGE_POINTERS) - 1))
#define il2c_tls_free(tlsIndex) ((void)0)
#define il2c_get_tls_value(tlsIndex) pvTaskGetThreadLocalStoragePointer(NULL, tlsIndex)
#define il2c_set_tls_value(tlsIndex, value) vTaskSetThreadLocalStoragePointer(NULL, tlsIndex, value)

#define IL2C_THREAD_ENTRY_POINT_RESULT_TYPE void
#define IL2C_THREAD_ENTRY_POINT_RETURN(value) ((void)0)
#define IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE void*
typedef IL2C_THREAD_ENTRY_POINT_RESULT_TYPE (*IL2C_THREAD_ENTRY_POINT_TYPE)(IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE);

#define il2c_get_current_thread__() ((intptr_t)xTaskGetCurrentTaskHandle())
#define il2c_get_current_thread_id__() ((int32_t)xTaskGetCurrentTaskHandle())
extern intptr_t il2c_create_thread__(IL2C_THREAD_ENTRY_POINT_TYPE entryPoint, IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE parameter);
#define il2c_resume_thread__(handle)
extern void il2c_join_thread__(intptr_t handle);
#define il2c_close_thread_handle__(handle) vTaskDelete((TaskHandle_t)(handle))

typedef xSemaphoreHandle IL2C_MONITOR_LOCK;
#define il2c_initialize_monitor_lock__(pLock) ((*(pLock)) = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
#define il2c_enter_monitor_lock__(pLock) pthread_mutex_lock(pLock)
#define il2c_try_enter_monitor_lock__(pLock) (pthread_mutex_trylock(pLock) != EBUSY)
#define il2c_exit_monitor_lock__(pLock) pthread_mutex_unlock(pLock)
#define il2c_destroy_monitor_lock__(pLock) pthread_mutex_destroy(pLock)

// POSIX threads (pthread)
#elif defined(_POSIX_THREADS)

#include <pthread.h>

typedef pthread_key_t IL2C_TLS_INDEX;
extern IL2C_TLS_INDEX il2c_tls_alloc(void);
#define il2c_tls_free(tlsIndex) pthread_key_delete(tlsIndex)
extern void* il2c_get_tls_value(IL2C_TLS_INDEX tlsIndex);
extern void il2c_set_tls_value(IL2C_TLS_INDEX tlsIndex, void* value);

#define IL2C_THREAD_ENTRY_POINT_RESULT_TYPE void*
#define IL2C_THREAD_ENTRY_POINT_RETURN(value) pthread_exit(NULL); return NULL
#define IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE void*
typedef IL2C_THREAD_ENTRY_POINT_RESULT_TYPE (*IL2C_THREAD_ENTRY_POINT_TYPE)(IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE);

#define il2c_get_current_thread__() ((intptr_t)pthread_self())
#define il2c_get_current_thread_id__() ((int32_t)pthread_self())
extern intptr_t il2c_create_thread__(IL2C_THREAD_ENTRY_POINT_TYPE entryPoint, void* parameter);
#define il2c_resume_thread__(handle)
extern void il2c_join_thread__(intptr_t handle);
#define il2c_close_thread_handle__(handle)

typedef pthread_mutex_t IL2C_MONITOR_LOCK;
extern void il2c_initialize_monitor_lock__(IL2C_MONITOR_LOCK* pLock);
#define il2c_enter_monitor_lock__(pLock) pthread_mutex_lock(pLock)
#define il2c_try_enter_monitor_lock__(pLock) (pthread_mutex_trylock(pLock) != EBUSY)
#define il2c_exit_monitor_lock__(pLock) pthread_mutex_unlock(pLock)
#define il2c_destroy_monitor_lock__(pLock) pthread_mutex_destroy(pLock)

// Lack for anything scheduler
#else

typedef intptr_t IL2C_TLS_INDEX;
#define il2c_tls_alloc() ((IL2C_TLS_INDEX)0)
#define il2c_tls_free(tlsIndex) (tlsIndex = 0)
#define il2c_get_tls_value(tlsIndex) ((void*)(tlsIndex))
#define il2c_set_tls_value(tlsIndex, value) (tlsIndex = (intptr_t)(value))

#define IL2C_THREAD_ENTRY_POINT_RESULT_TYPE void
#define IL2C_THREAD_ENTRY_POINT_RETURN(value) ((void)0)
#define IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE void*
typedef IL2C_THREAD_ENTRY_POINT_RESULT_TYPE (*IL2C_THREAD_ENTRY_POINT_TYPE)(IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE);

#define il2c_get_current_thread__() ((intptr_t)0)
#define il2c_get_current_thread_id__() ((int32_t)0)
#define il2c_create_thread__(entryPoint, parameter) ((intptr_t)-1)
#define il2c_resume_thread__(handle) ((void)0)
#define il2c_join_thread__(handle) ((void)0)

typedef uint8_t IL2C_MONITOR_LOCK;
#define il2c_initialize_monitor_lock__(pLock) ((void)0)
#define il2c_enter_monitor_lock__(pLock) ((void)0)
#define il2c_try_enter_monitor_lock__(pLock) (true)
#define il2c_exit_monitor_lock__(pLock) ((void)0)
#define il2c_destroy_monitor_lock__(pLock) ((void)0)

#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
