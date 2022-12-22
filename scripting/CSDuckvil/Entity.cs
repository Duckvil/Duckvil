using System.Runtime.InteropServices;

namespace Duckvil
{
    public class Entity
    {
        public readonly ulong ID;

        protected Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            this.ID = id;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            return InternalCalls.Entity_HasComponent(ID, typeof(T));
        }

        //public bool AddComponent<T>() where T : Component, new()
        //{
        //    return InternalCalls.Entity_AddComponent(ID, typeof(T), null);
        //}

        public bool AddComponent<T>(T? data = null) where T : Component, new()
        {
            return InternalCalls.Entity_AddComponent(ID, typeof(T), data);
        }

        public T? GetComponent<T>() where T : Component, new()
        {
            return HasComponent<T>() ? new T() { Entity = this } : null;
        }
    };

}