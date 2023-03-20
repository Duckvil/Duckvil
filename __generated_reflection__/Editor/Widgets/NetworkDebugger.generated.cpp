#include "Editor/Widgets/NetworkDebugger.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(15)
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
_namespaces.push_back("NetworkDebugger");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::NetworkDebugger>(_data, "NetworkDebugger");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 15);
record_meta(_data, _type, Duckvil::ReflectionFlags::ReflectionFlags_UserSystem, true);
record_inheritance<Widget>(_data, _type, __protection::__protection_public);
record_inheritance<ISystem>(_data, _type, __protection::__protection_public);
record_inheritance<Network::NetworkSystem>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Editor::NetworkDebugger, Memory::FreeList const&, Network::IClient*, Network::IServer*>(_data, _type);
_destructor = record_destructor<Duckvil::Editor::NetworkDebugger>(_data, _type);
_function = record_function<Duckvil::Editor::NetworkDebugger, bool>(_data, _type, &Duckvil::Editor::NetworkDebugger::Init, "Init");
_function = record_function<Duckvil::Editor::NetworkDebugger, void, double>(_data, _type, &Duckvil::Editor::NetworkDebugger::Update, "Update");
_function = record_function<Duckvil::Editor::NetworkDebugger, void, void*>(_data, _type, &Duckvil::Editor::NetworkDebugger::InitEditor, "InitEditor");
_function = record_function<Duckvil::Editor::NetworkDebugger, void>(_data, _type, &Duckvil::Editor::NetworkDebugger::OnDraw, "OnDraw");
_function = record_function<Duckvil::Editor::NetworkDebugger, void, Network::IConnection::Owner>(_data, _type, &Duckvil::Editor::NetworkDebugger::SetOwner, "SetOwner");
_function = record_function<Duckvil::Editor::NetworkDebugger, bool, Duckvil::Network::Message const&, std::shared_ptr<Duckvil::Network::IConnection>>(_data, _type, &Duckvil::Editor::NetworkDebugger::OnMessage, "OnMessage");
_function = record_function<void*, void*>(_data, _type, &Duckvil::Editor::NetworkDebugger::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Widgets/NetworkDebugger.h", 15);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 15;
}
#endif
