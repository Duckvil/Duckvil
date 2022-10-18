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
_namespaces.push_back("ImGuiEditorData");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::ImGuiEditorData>(_data, "ImGuiEditorData");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
#ifdef DUCKVIL_HOT_RELOADING
_property = record_property<Memory::Vector<Editor::HotDraw>>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aHotDraws), "m_aHotDraws");
#endif
_property = record_property<Memory::Vector<Editor::Draw>>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aDraws), "m_aDraws");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, _ctx), "_ctx");
_property = record_property<Event::Pool<Event::mode::immediate>>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pEditorEvents), "m_pEditorEvents");
_property = record_property<Event::Pool<Event::mode::immediate>*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pRuntimeReflectionEventPool), "m_pRuntimeReflectionEventPool");
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_heap), "m_heap");
_property = record_property<Memory::Vector<uint32_t>>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_aTextures), "m_aTextures");
_property = record_property<Graphics::Renderer::renderer_ftable*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pRenderer), "m_pRenderer");
_property = record_property<Graphics::Renderer::renderer_data*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pRendererData), "m_pRendererData");
_property = record_property<Event::Pool<Event::mode::buffered>*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pWindowEventPool), "m_pWindowEventPool");
_property = record_property<flecs::world*>(_data, _type, offsetof(Duckvil::Editor::ImGuiEditorData, m_pECS), "m_pECS");
_namespaces.pop_back();
_namespaces.push_back("EditorFTable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Editor::EditorFTable>(_data, "EditorFTable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
_property = record_property<void*(*)(EditorFTable*, const Memory::FreeList&, Window::IWindow*, Graphics::Renderer::renderer_ftable*, Graphics::Renderer::renderer_data*, Event::Pool<Event::mode::immediate>*, Event::Pool<Event::mode::immediate>*)>(_data, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnInit), "m_fnInit");
_property = record_property<void(*)(void*, Window::IWindow*)>(_data, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnRender), "m_fnRender");
_property = record_property<void(*)(void*, void*)>(_data, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnSetWindowEventPool), "m_fnSetWindowEventPool");
_property = record_property<void(*)(void*, EditorFTable*, Event::Pool<Event::mode::immediate>*, HotReloader::RuntimeCompilerSystem*)>(_data, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnPostInit), "m_fnPostInit");
_property = record_property<void(*)(ImGuiEditorData*, const HotReloader::SwapEvent&)>(_data, _type, offsetof(Duckvil::Editor::EditorFTable, m_fnHotReloadInit), "m_fnHotReloadInit");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Editor.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
