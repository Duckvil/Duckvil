#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<ICompiler*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_aFlags", m_aFlags); \
_pSerializer->SerializeProperty("m_aDefines", m_aDefines); \
_pSerializer->SerializeProperty("m_aIncludes", m_aIncludes); \
_pSerializer->SerializeProperty("m_aLibrariesPaths", m_aLibrariesPaths); \
_pSerializer->SerializeProperty("m_aLibraries", m_aLibraries); \
}
