#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__ast_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_protection", m_protection); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_sKey", m_sKey); \
_pSerializer->SerializeProperty("m_sValue", m_sValue); \
_pSerializer->SerializeProperty("m_sType", m_sType); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_aElements", m_aElements); \
_pSerializer->SerializeProperty("m_scopeType", m_scopeType); \
_pSerializer->SerializeProperty("m_aScopes", m_aScopes); \
_pSerializer->SerializeProperty("m_pParentScope", m_pParentScope); \
_pSerializer->SerializeProperty("m_aMeta", m_aMeta); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_sType", m_sType); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_accessLevel", m_accessLevel); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_structureType", m_structureType); \
_pSerializer->SerializeProperty("m_aInheritance", m_aInheritance); \
_pSerializer->SerializeProperty("m_aTemplates", m_aTemplates); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_aElements", m_aElements); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_aArguments", m_aArguments); \
_pSerializer->SerializeProperty("m_accessLevel", m_accessLevel); \
_pSerializer->SerializeProperty("m_sType", m_sType); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_aArguments", m_aArguments); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_sReturnType", m_sReturnType); \
_pSerializer->SerializeProperty("m_sMemberType", m_sMemberType); \
_pSerializer->SerializeProperty("m_aTemplates", m_aTemplates); \
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_sReturnType", m_sReturnType); \
_pSerializer->SerializeProperty("m_aTemplates", m_aTemplates); \
_pSerializer->SerializeProperty("m_flags", m_flags); \
_pSerializer->SerializeProperty("m_sUserDefine", m_sUserDefine); \
_pSerializer->SerializeProperty("m_main", m_main); \
_pSerializer->SerializeProperty("m_pCurrentScope", m_pCurrentScope); \
_pSerializer->SerializeProperty("m_pPendingScope", m_pPendingScope); \
_pSerializer->SerializeProperty("m_currentAccess", m_currentAccess); \
_pSerializer->SerializeProperty("m_aUserDefines", m_aUserDefines); \
}
