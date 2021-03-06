﻿// [14-1] This is MT3620Blink native code translated by IL2C, do not edit.

#include <MT3620Blink.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////
// [14-2] Type pre definitions:

////////////////////////////////////////////////////////////
// [2-1-1] .NET types:

/* internal static class */ typedef struct MT3620Blink_Interops MT3620Blink_Interops;

////////////////////////////////////////////////////////////
// [2-1-2] VTable types:

typedef System_Object_VTABLE_DECL__ MT3620Blink_Interops_VTABLE_DECL__;

///////////////////////////////////////////////////////////////////////////
// [14-3] Type body definitions:

#ifdef MT3620Blink_DECL_TYPE_BODY__

////////////////////////////////////////////////////////////
// [1] MT3620Blink.Interops

// [1-1-2] Class layout
/* internal static class */ struct MT3620Blink_Interops
{
    MT3620Blink_Interops_VTABLE_DECL__* vptr0__;
};

// [1-5-1] VTable (Same as System.Object)
#define MT3620Blink_Interops_VTABLE__ System_Object_VTABLE__

// [1-4] Runtime type information
IL2C_DECLARE_RUNTIME_TYPE(MT3620Blink_Interops);

//////////////////////////////////////////////////////////////////////////////////
// [2-2] Static fields: MT3620Blink.Interops

/* public static readonly */ #define MT3620Blink_Interops_MT3620_RDB_LED1_RED MT3620_RDB_LED1_RED

/* public static readonly */ #define MT3620Blink_Interops_MT3620_RDB_BUTTON_A MT3620_RDB_BUTTON_A

/* public static readonly */ #define MT3620Blink_Interops_MT3620_RDB_BUTTON_B MT3620_RDB_BUTTON_B

/* public static readonly */ #define MT3620Blink_Interops_EPOLL_CTL_ADD EPOLL_CTL_ADD

/* public static readonly */ #define MT3620Blink_Interops_EPOLL_CTL_DEL EPOLL_CTL_DEL

/* public static readonly */ #define MT3620Blink_Interops_EPOLLIN EPOLLIN

//////////////////////////////////////////////////////////////////////////////////
// [2-3] Methods: MT3620Blink.Interops

extern /* public static sealed */ int32_t MT3620Blink_Interops_close(int32_t fd);
extern /* public static sealed */ void MT3620Blink_Interops_nanosleep(MT3620Blink_timespec* req, MT3620Blink_timespec* rem);
extern /* public static sealed */ int32_t MT3620Blink_Interops_timerfd_create(int32_t clockid, int32_t flags);
extern /* public static sealed */ int32_t MT3620Blink_Interops_timerfd_settime(int32_t fd, int32_t flags, MT3620Blink_itimerspec* new_value, MT3620Blink_itimerspec* old_value);
extern /* public static sealed */ int32_t MT3620Blink_Interops_timerfd_read(int32_t fd, uint64_t* timerData, uintptr_t size);
extern /* public static sealed */ int32_t MT3620Blink_Interops_eventfd(uint32_t initval, int32_t flags);
extern /* public static sealed */ int32_t MT3620Blink_Interops_eventfd_write(int32_t fd, uint64_t value);
extern /* public static sealed */ int32_t MT3620Blink_Interops_eventfd_read(int32_t fd, uint64_t* value);
extern /* public static sealed */ int32_t MT3620Blink_Interops_GPIO_OpenAsOutput(int32_t gpioId, MT3620Blink_GPIO_OutputMode_Type outputMode, MT3620Blink_GPIO_Value_Type initialValue);
extern /* public static sealed */ int32_t MT3620Blink_Interops_GPIO_SetValue(int32_t gpioFd, MT3620Blink_GPIO_Value_Type value);
extern /* public static sealed */ int32_t MT3620Blink_Interops_GPIO_OpenAsInput(int32_t gpioId);
extern /* public static sealed */ int32_t MT3620Blink_Interops_GPIO_GetValue(int32_t gpioFd, MT3620Blink_GPIO_Value_Type* value);
extern /* public static sealed */ int32_t MT3620Blink_Interops_epoll_create1(int32_t flags);
extern /* public static sealed */ int32_t MT3620Blink_Interops_epoll_ctl(int32_t epollfd, int32_t op, int32_t fd, MT3620Blink_epoll_event* ev);
extern /* public static sealed */ int32_t MT3620Blink_Interops_epoll_wait(int32_t epollfd, MT3620Blink_epoll_event* ev, int32_t maxevents, int32_t timeout);

#endif

#ifdef __cplusplus
}
#endif
