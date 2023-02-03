#include "Editor/Draw.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
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
_namespaces.push_back("HotDraw");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::HotDraw>(_data, "HotDraw");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<void(Duckvil::Editor::Widget::*)()>(_data, _type, offsetof(Duckvil::Editor::HotDraw, m_fnDraw), "m_fnDraw");
_property = record_property<void(Duckvil::Editor::Widget::*)()>(_data, _type, offsetof(Duckvil::Editor::HotDraw, m_fnDrawMainMenu), "m_fnDrawMainMenu");
_property = record_property<void(Duckvil::Editor::Widget::*)(void*)>(_data, _type, offsetof(Duckvil::Editor::HotDraw, m_fnInit), "m_fnInit");
_property = record_property<Duckvil::Editor::Widget*>(_data, _type, offsetof(Duckvil::Editor::HotDraw, m_pTrackKeeper), "m_pTrackKeeper");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::Editor::HotDraw, m_typeHandle), "m_typeHandle");
_namespaces.pop_back();
_namespaces.push_back("Draw");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Editor::Draw>(_data, "Draw");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<void(Duckvil::Editor::Widget::*)()>(_data, _type, offsetof(Duckvil::Editor::Draw, m_fnDraw), "m_fnDraw");
_property = record_property<void(Duckvil::Editor::Widget::*)()>(_data, _type, offsetof(Duckvil::Editor::Draw, m_fnDrawMainMenu), "m_fnDrawMainMenu");
_property = record_property<void(Duckvil::Editor::Widget::*)(void*)>(_data, _type, offsetof(Duckvil::Editor::Draw, m_fnInit), "m_fnInit");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Editor::Draw, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(_data, _type, offsetof(Duckvil::Editor::Draw, m_typeHandle), "m_typeHandle");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Draw.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
