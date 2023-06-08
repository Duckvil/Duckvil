#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_DUMMY 
#define Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<HexEditorWidget*>(_pObject); }
#define Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("a1", a1); \
_pSerializer->SerializeProperty("a2", a2); \
_pSerializer->SerializeProperty("m_hexEditor", m_hexEditor); \
}
#define Editor_Widgets_HexEditorWidget_h_21_GENERATED_BODY \
Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_DUMMY \
Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_CASTER \
Editor_Widgets_HexEditorWidget_h_21_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_HexEditorWidget_h

