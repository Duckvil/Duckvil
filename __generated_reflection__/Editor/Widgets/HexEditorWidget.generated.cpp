#include "Editor/Widgets/HexEditorWidget.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::HexEditorWidget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "HexEditorWidget");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
record_inheritance<Widget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Editor::HexEditorWidget, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::Editor::HexEditorWidget, void, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::HexEditorWidget::InitEditor, "InitEditor");
record_function<Duckvil::Editor::HexEditorWidget, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::HexEditorWidget::OnDraw, "OnDraw");
record_function<Duckvil::Editor::HexEditorWidget, void, const HexEditorWidgetInitEvent&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::HexEditorWidget::OnEvent, "OnEvent");
record_function<void*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::HexEditorWidget::Cast, "Cast");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Editor/Widgets/HexEditorWidget.h", 3 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
