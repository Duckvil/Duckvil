#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<LoadProjectWidget*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_iSelectedProject", m_iSelectedProject); \
_pSerializer->SerializeProperty("m_aProjects", m_aProjects); \
}
