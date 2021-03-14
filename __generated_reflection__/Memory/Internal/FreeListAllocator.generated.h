#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__free_list_node*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_ucPadding", m_ucPadding); \
_pSerializer->SerializeProperty("m_ullSize", m_ullSize); \
_pSerializer->SerializeProperty("m_pNext", m_pNext); \
_pSerializer->SerializeProperty("m_ullSize", m_ullSize); \
}
