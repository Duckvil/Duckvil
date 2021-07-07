#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<ViewportWidget*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_pRenderer", m_pRenderer); \
_pSerializer->SerializeProperty("m_pRendererData", m_pRendererData); \
_pSerializer->SerializeProperty("m_oldSize", m_oldSize); \
_pSerializer->SerializeProperty("m_pWindowEventPool", m_pWindowEventPool); \
_pSerializer->SerializeProperty("m_viewport", m_viewport); \
_pSerializer->SerializeProperty("m_bWrapCamera", m_bWrapCamera); \
_pSerializer->SerializeProperty("m_aKeys", m_aKeys); \
_pSerializer->SerializeProperty("m_bIsWindowFocused", m_bIsWindowFocused); \
_pSerializer->SerializeProperty("m_setMousePosition", m_setMousePosition); \
_pSerializer->SerializeProperty("m_bSkip", m_bSkip); \
_pSerializer->SerializeProperty("m_pECS", m_pECS); \
ISystem::Serialize(_pSerializer); \
}
