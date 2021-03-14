#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<HexEditorWidget*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("a1", a1); \
_pSerializer->SerializeProperty("a2", a2); \
_pSerializer->SerializeProperty("m_hexEditor", m_hexEditor); \
}
