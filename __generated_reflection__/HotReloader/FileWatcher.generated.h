#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<FileWatcher*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_aPathsToWatch", m_aPathsToWatch); \
_pSerializer->SerializeProperty("m_fnAction", m_fnAction); \
_pSerializer->SerializeProperty("m_aPaths", m_aPaths); \
_pSerializer->SerializeProperty("m_pUserData", m_pUserData); \
}
