using System.Runtime.CompilerServices;

namespace Duckvil
{
    public abstract class Component
    {
        public Entity? Entity { get; internal set; }
    };

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity?.ID ?? throw new Exception(), out var translation);

                return translation;
            }

            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity?.ID ?? throw new Exception(), ref value);
            }
        }

        public Quaternion Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity?.ID ?? throw new Exception(), out var rotation);

                return rotation;
            }

            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity?.ID ?? throw new Exception(), ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity?.ID ?? throw new Exception(), out var scale);

                return scale;
            }

            set
            {
                InternalCalls.TransformComponent_SetScale(Entity?.ID ?? throw new Exception(), ref value);
            }
        }
    };

}