#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<viewport*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_shaderID", m_shaderID); \
_pSerializer->SerializeProperty("m_fbo", m_fbo); \
_pSerializer->SerializeProperty("m_fboTextureObject", m_fboTextureObject); \
_pSerializer->SerializeProperty("m_textureID", m_textureID); \
_pSerializer->SerializeProperty("m_transformID", m_transformID); \
_pSerializer->SerializeProperty("m_projection", m_projection); \
_pSerializer->SerializeProperty("m_view", m_view); \
_pSerializer->SerializeProperty("m_rotation", m_rotation); \
_pSerializer->SerializeProperty("m_position", m_position); \
_pSerializer->SerializeProperty("m_ecs", m_ecs); \
_pSerializer->SerializeProperty("m_rendererQuery", m_rendererQuery); \
}
