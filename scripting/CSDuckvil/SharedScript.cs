using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Duckvil
{
    public interface SharedScript
    {
        public ulong GetSharedScriptID();
        public void SetSharedScriptID(ulong id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Register(out ulong id, object type);
    }

    [System.AttributeUsage(System.AttributeTargets.Method | AttributeTargets.Field | AttributeTargets.Property)]
    public class CppSharedAttribute : System.Attribute
    {
        public CppSharedAttribute()
        {

        }

        public CppSharedAttribute(string name)
        {

        }
    }
}
