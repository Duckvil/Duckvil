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
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::ImGuiEditorData>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ImGuiEditorData");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
#ifdef DUCKVIL_HOT_RELOADING
_property = record_property<Memory::Vector<Editor::HotDraw>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aHotDraws), "m_aHotDraws");
#endif
_property = record_property<Memory::Vector<Editor::Draw>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aDraws), "m_aDraws");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, _ctx), "_ctx");
_property = record_property<Event::Pool<Event::mode::immediate>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pEditorEvents), "m_pEditorEvents");
_property = record_property<Memory::FreeList>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_heap), "m_heap");
_property = record_property<Memory::Vector<uint32_t>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aTextures), "m_aTextures");
_property = record_property<Graphics::Renderer::renderer_ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pRenderer), "m_pRenderer");
_property = record_property<Graphics::Renderer::renderer_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pRendererData), "m_pRendererData");
_property = record_property<Event::Pool<Event::mode::buffered>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pWindowEventPool), "m_pWindowEventPool");
_property = record_property<flecs::world*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pECS), "m_pECS");
_type = record_type<Duckvil::Editor::EditorFTable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "EditorFTable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<void*(*)(Duckvil::Memory::ftable*, Duckvil::Memory::free_list_allocator*, Window::IWindow*, Graphics::Renderer::renderer_ftable*, Graphics::Renderer::renderer_data*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnInit), "m_fnInit");
_property = record_property<void(*)(void*, Window::IWindow*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnRender), "m_fnRender");
_property = record_property<void(*)(void*, HotDraw)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnAddHotDraw), "m_fnAddHotDraw");
_property = record_property<void(*)(void*, Draw)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnAddDraw), "m_fnAddDraw");
_property = record_property<void(*)(void*, void*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnSetWindowEventPool), "m_fnSetWindowEventPool");
_property = record_property<void(*)(const Memory::FreeList&, void*, EditorFTable*, Event::Pool<Event::mode::immediate>*, HotReloader::RuntimeCompilerSystem*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnPostInit), "m_fnPostInit");
_property = record_property<void(*)(ImGuiEditorData*, const HotReloader::SwapEvent&)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnHotReloadInit), "m_fnHotReloadInit");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Editor/Editor.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
