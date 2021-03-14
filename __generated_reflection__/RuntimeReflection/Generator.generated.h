#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__generator_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_aMeta", m_aMeta); \
_pSerializer->SerializeProperty("m_aNamespaces", m_aNamespaces); \
_pSerializer->SerializeProperty("m_bWasNamespaces", m_bWasNamespaces); \
_pSerializer->SerializeProperty("m_sInclude", m_sInclude); \
_pSerializer->SerializeProperty("m_uiRecorderIndex", m_uiRecorderIndex); \
}
