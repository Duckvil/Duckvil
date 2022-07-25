#include "HotReloader/Network/HotObjectSync.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_namespaces.push_back("HotReloader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Network");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("HotObjectSync");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::HotReloader::Network;
_type = record_type<Duckvil::HotReloader::Network::HotObjectSync>(_data, "HotObjectSync");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "HotReloader");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_meta(_data, _type, ReflectionFlags_AutoEventsAdding, true);
record_inheritance<ISystem>(_data, _type, __protection::__protection_public);
record_inheritance<Duckvil::Network::NetworkSystem>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::HotReloader::Network::HotObjectSync, const Memory::FreeList&, Duckvil::Network::IClient*, Duckvil::Network::IServer*, HotReloader::RuntimeCompilerSystem*>(_data, _type);
_destructor = record_destructor<Duckvil::HotReloader::Network::HotObjectSync>(_data, _type);
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, bool>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::Init, "Init");
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, void, double>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::Update, "Update");
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, void, const HotReloader::SwapEvent&>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::OnEvent, "OnEvent");
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, void, Duckvil::Network::IConnection::Owner>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::SetOwner, "SetOwner");
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, bool, const Duckvil::Network::Message&, std::shared_ptr<Duckvil::Network::IConnection>>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::OnMessage, "OnMessage");
_function = record_function<void*, void*>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::HotReloader::Network::HotObjectSync, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::HotReloader::Network::HotObjectSync::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_enum = record_enum<Duckvil::HotReloader::Network::HotObjectSync::NetworkCommands>(_data, _ntype, "NetworkCommands");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::HotReloader::Network::HotObjectSync::NetworkCommands::SendHotObject, "SendHotObject");
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "HotReloader/Network/HotObjectSync.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
