#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<Descriptor*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_iArgc", m_iArgc); \
_pSerializer->SerializeProperty("m_spArgv", m_spArgv); \
_pSerializer->SerializeProperty("m_pDescriptors", m_pDescriptors); \
_pSerializer->SerializeProperty("m_uiIndex", m_uiIndex); \
_pSerializer->SerializeProperty("m_bIsSet", m_bIsSet); \
}
