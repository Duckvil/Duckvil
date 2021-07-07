#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__plugin_details*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pAllocator", m_pAllocator); \
_pSerializer->SerializeProperty("m_pInstance", m_pInstance); \
_pSerializer->SerializeProperty("m_pDeallocator", m_pDeallocator); \
_pSerializer->SerializeProperty("m_instantiation", m_instantiation); \
}
