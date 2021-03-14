#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<WindowSDL*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pWindow", m_pWindow); \
_pSerializer->SerializeProperty("m_pContext", m_pContext); \
_pSerializer->SerializeProperty("m_pEvents", m_pEvents); \
}
