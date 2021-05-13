#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<EditorFTable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_aDraws", m_aDraws); \
_pSerializer->SerializeProperty("_ctx", _ctx); \
_pSerializer->SerializeProperty("m_pEditorEvents", m_pEditorEvents); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
}
