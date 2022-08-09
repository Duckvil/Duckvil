#include "Editor/Widgets/HexEditorWidget.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(12)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Editor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("HexEditorWidget");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::HexEditorWidget>(_data, "HexEditorWidget");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 12);
record_inheritance<Widget>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Editor::HexEditorWidget, const Memory::FreeList&>(_data, _type);
_destructor = record_destructor<Duckvil::Editor::HexEditorWidget>(_data, _type);
_property = record_property<MemoryEditor>(_data, _type, offsetof(Duckvil::Editor::HexEditorWidget, m_hexEditor), "m_hexEditor");
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_debug_info*>(_data, _type, offsetof(Duckvil::Editor::HexEditorWidget, m_pSelected), "m_pSelected");
#endif
_function = record_function<Duckvil::Editor::HexEditorWidget, void, void*>(_data, _type, &Duckvil::Editor::HexEditorWidget::InitEditor, "InitEditor");
_function = record_function<Duckvil::Editor::HexEditorWidget, void>(_data, _type, &Duckvil::Editor::HexEditorWidget::OnDraw, "OnDraw");
_function = record_function<Duckvil::Editor::HexEditorWidget, void, const HexEditorWidgetInitEvent&>(_data, _type, &Duckvil::Editor::HexEditorWidget::OnEvent, "OnEvent");
_function = record_function<void*, void*>(_data, _type, &Duckvil::Editor::HexEditorWidget::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::Editor::HexEditorWidget, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::Editor::HexEditorWidget::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Widgets/HexEditorWidget.h", 12);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 12;
}
#endif
