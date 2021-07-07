#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<renderer_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_target", m_target); \
_pSerializer->SerializeProperty("m_filter", m_filter); \
_pSerializer->SerializeProperty("m_width", m_width); \
_pSerializer->SerializeProperty("m_height", m_height); \
_pSerializer->SerializeProperty("m_pData", m_pData); \
_pSerializer->SerializeProperty("m_target", m_target); \
_pSerializer->SerializeProperty("m_filter", m_filter); \
_pSerializer->SerializeProperty("m_width", m_width); \
_pSerializer->SerializeProperty("m_height", m_height); \
_pSerializer->SerializeProperty("m_pData", m_pData); \
_pSerializer->SerializeProperty("m_uiCount", m_uiCount); \
_pSerializer->SerializeProperty("m_target", m_target); \
_pSerializer->SerializeProperty("m_aAttachments", m_aAttachments); \
_pSerializer->SerializeProperty("m_uiCount", m_uiCount); \
_pSerializer->SerializeProperty("m_aTextures", m_aTextures); \
_pSerializer->SerializeProperty("m_textureTarget", m_textureTarget); \
_pSerializer->SerializeProperty("m_uiTypeSize", m_uiTypeSize); \
_pSerializer->SerializeProperty("m_pData", m_pData); \
_pSerializer->SerializeProperty("m_usNumber", m_usNumber); \
_pSerializer->SerializeProperty("m_target", m_target); \
_pSerializer->SerializeProperty("m_uiVBO_Count", m_uiVBO_Count); \
_pSerializer->SerializeProperty("m_aVBO", m_aVBO); \
_pSerializer->SerializeProperty("m_uiCount", m_uiCount); \
_pSerializer->SerializeProperty("m_pTexture", m_pTexture); \
_pSerializer->SerializeProperty("m_uiTextureCount", m_uiTextureCount); \
_pSerializer->SerializeProperty("m_FBO", m_FBO); \
_pSerializer->SerializeProperty("m_vao", m_vao); \
_pSerializer->SerializeProperty("m_uiDrawCount", m_uiDrawCount); \
_pSerializer->SerializeProperty("m_shader", m_shader); \
_pSerializer->SerializeProperty("m_texture", m_texture); \
_pSerializer->SerializeProperty("m_textureObject", m_textureObject); \
_pSerializer->SerializeProperty("m_fbo", m_fbo); \
_pSerializer->SerializeProperty("m_vao", m_vao); \
_pSerializer->SerializeProperty("m_uniform", m_uniform); \
_pSerializer->SerializeProperty("m_pCommandBuffer", m_pCommandBuffer); \
_pSerializer->SerializeProperty("m_pAllocator", m_pAllocator); \
_pSerializer->SerializeProperty("m_ecs", m_ecs); \
}
