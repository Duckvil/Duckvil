using System.Runtime.CompilerServices;

namespace Duckvil
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool Entity_AddComponent(ulong entityID, Type componentType, Component? data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetRotation(ulong entityID, out Quaternion rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetRotation(ulong entityID, ref Quaternion rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 scale);
    };
}