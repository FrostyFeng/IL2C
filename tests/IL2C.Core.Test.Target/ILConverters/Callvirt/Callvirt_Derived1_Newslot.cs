using System;
using System.Runtime.CompilerServices;

namespace IL2C.ILConverters
{
    [TestId("Callvirt")]
    [TestCase("IL2C.ILConverters.Callvirt_Derived1_Newslot", new[] { "Derived1_Newslot_ToString_System_Object", "ToString" })]
    [TestCase("CallvirtTest", new[] { "Derived1_Newslot_ToString_IL2C_ILConverters_Callvirt", "ToString" })]
    public sealed class Callvirt_Derived1_Newslot
    {
        public new string ToString()
        {
            return "CallvirtTest";
        }

        [MethodImpl(MethodImplOptions.ForwardRef)]
        public static extern string Derived1_Newslot_ToString_System_Object();

        [MethodImpl(MethodImplOptions.ForwardRef)]
        public static extern string Derived1_Newslot_ToString_IL2C_ILConverters_Callvirt();
    }
}
