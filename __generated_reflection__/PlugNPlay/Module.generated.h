#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__module*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_sName", m_sName); \
_pSerializer->SerializeProperty("m_pModule", m_pModule); \
_pSerializer->SerializeProperty("m_sPath", m_sPath); \
}
