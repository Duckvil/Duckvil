#include "Editor/Editor.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::EditorFTable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "EditorFTable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<void*(*)(Duckvil::Memory::IMemory*, Duckvil::Memory::__free_list_allocator*, Window::IWindow*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnInit), "m_fnInit");
_property = record_property<void(*)(void*, Window::IWindow*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnRender), "m_fnRender");
_property = record_property<void(*)(void*, Draw)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnAddDraw), "m_fnAddDraw");
_property = record_property<void(*)(void*, void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnRemoveDraw), "m_fnRemoveDraw");
_property = record_property<void(*)(RuntimeReflection::__data*, const Memory::FreeList&, void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnPostInit), "m_fnPostInit");
_type = record_type<Duckvil::Editor::ImGuiEditorData>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ImGuiEditorData");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<Memory::Vector<Editor::Draw>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aDraws), "m_aDraws");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, _ctx), "_ctx");
_property = record_property<Event::Pool<Event::mode::immediate>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pEditorEvents), "m_pEditorEvents");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); ++i) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Editor/Editor.h", 1 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
