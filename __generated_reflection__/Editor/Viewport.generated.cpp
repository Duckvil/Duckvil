#include "Editor/Viewport.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
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
_namespaces.push_back("viewport");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::viewport>(_data, "viewport");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_shaderID), "m_shaderID");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_fbo), "m_fbo");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_fboTextureObject), "m_fboTextureObject");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_textureID), "m_textureID");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_transformID), "m_transformID");
_property = record_property<glm::mat4>(_data, _type, offsetof(Duckvil::Editor::viewport, m_projection), "m_projection");
_property = record_property<glm::mat4>(_data, _type, offsetof(Duckvil::Editor::viewport, m_view), "m_view");
_property = record_property<glm::quat>(_data, _type, offsetof(Duckvil::Editor::viewport, m_rotation), "m_rotation");
_property = record_property<glm::vec3>(_data, _type, offsetof(Duckvil::Editor::viewport, m_position), "m_position");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_uiWidth), "m_uiWidth");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Editor::viewport, m_uiHeight), "m_uiHeight");
_property = record_property<flecs::world*>(_data, _type, offsetof(Duckvil::Editor::viewport, m_ecs), "m_ecs");
_property = record_property<flecs::query<Graphics::MeshComponent,Graphics::TransformComponent>>(_data, _type, offsetof(Duckvil::Editor::viewport, m_rendererQuery), "m_rendererQuery");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Viewport.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
