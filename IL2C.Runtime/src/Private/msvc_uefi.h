// It uses for internal purpose only.

#ifndef __MSVC_UEFI_H__
#define __MSVC_UEFI_H__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////
// Visual C++ (UEFI)

#if defined(_MSC_VER) && defined(UEFI)

#include <intrin.h>
#include <stdint.h>
#include <wchar.h>

// Compatibility symbols (required platform depended functions)
extern wchar_t* il2c_itow(int32_t value, wchar_t* d, int radix);
#define il2c_ultow _ultow
#define il2c_i64tow _i64tow
#define il2c_ui64tow _ui64tow
#define il2c_snwprintf _snwprintf
extern long il2c_wcstol(const wchar_t *nptr, wchar_t **endptr, int base);
#define il2c_wcstoul wcstoul
#define il2c_wcstoll wcstoll
#define il2c_wcstoull wcstoull
#define il2c_wcstof wcstof
#define il2c_wcstod wcstod
#define il2c_wcscmp wcscmp
#define il2c_wcsicmp wcsicmp
#define il2c_wcslen wcslen
#define il2c_initialize_heap()
#define il2c_check_heap()
#define il2c_shutdown_heap()

extern void* il2c_malloc(size_t size);
extern void il2c_free(void* p);
#define il2c_mcalloc il2c_malloc
#define il2c_mcfree il2c_free

#define il2c_ixchg(pDest, newValue) _InterlockedExchange((interlock_t*)(pDest), (interlock_t)(newValue))
#define il2c_ixchgptr(ppDest, pNewValue) _InterlockedExchangePointer((void**)(ppDest), (void*)(pNewValue))
#define il2c_icmpxchg(pDest, newValue, comperandValue) _InterlockedCompareExchange((interlock_t*)(pDest), (interlock_t)(newValue), (interlock_t)(comperandValue))
#define il2c_icmpxchgptr(ppDest, pNewValue, pComperandValue) _InterlockedCompareExchangePointer((void**)(ppDest), (void*)(pNewValue), (void*)(pComperandValue))
extern void il2c_sleep(uint32_t milliseconds);

#define il2c_longjmp longjmp

#endif

#ifdef __cplusplus
}
#endif

#endif