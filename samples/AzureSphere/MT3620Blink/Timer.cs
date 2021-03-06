﻿using System;
using System.Runtime.InteropServices;

namespace MT3620Blink
{
    internal abstract class Timer : Descriptor, IEPollListener
    {
        [NativeValue("time.h")]
        private static readonly int CLOCK_MONOTONIC;
        [NativeValue("time.h")]
        private static readonly int TFD_NONBLOCK;

        protected Timer()
            : base(Interops.timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK))
        {
        }

        public void SetInterval(long nsec)
        {
            var tm = new timespec
            {
                tv_sec = (int)(nsec / 1_000_000_000L),
                tv_nsec = (int)(nsec % 1_000_000_000L)
            };
            var newValue = new itimerspec
            {
                it_value = tm,
                it_interval = tm
            };

            Interops.timerfd_settime(this.Identity, 0, ref newValue, out var dummy);
        }

        int IEPollListener.Identity => this.Identity;

        void IEPollListener.OnRaised()
        {
            Interops.timerfd_read(this.Identity, out var timerData, (UIntPtr)(sizeof(ulong)));
            Raised();
        }

        protected abstract void Raised();
    }
}
