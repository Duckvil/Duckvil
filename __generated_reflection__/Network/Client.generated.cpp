#include "Network/Client.h"
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
_namespaces.push_back("Network");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Client");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::Client>(_data, "Client");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_inheritance<IClient>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Network::Client, Memory::FreeList const&>(_data, _type);
_destructor = record_destructor<Duckvil::Network::Client>(_data, _type);
_function = record_function<Duckvil::Network::Client, bool, std::string const&, uint16_t const>(_data, _type, &Duckvil::Network::Client::Connect, "Connect");
_function = record_function<Duckvil::Network::Client, void>(_data, _type, &Duckvil::Network::Client::Disconnect, "Disconnect");
_function = record_function<Duckvil::Network::Client, bool>(_data, _type, &Duckvil::Network::Client::IsConnected, "IsConnected");
_function = record_function<Duckvil::Network::Client, void>(_data, _type, &Duckvil::Network::Client::Update, "Update");
_function = record_function<Duckvil::Network::Client, void, Duckvil::Network::NetworkSystem*>(_data, _type, &Duckvil::Network::Client::AddSystem, "AddSystem");
_function = record_function<Duckvil::Network::Client, tsqueue<Duckvil::Network::OwnedMessage>&>(_data, _type, &Duckvil::Network::Client::Incoming, "Incoming");
_function = record_function<Duckvil::Network::Client, void, Duckvil::Network::Message const&>(_data, _type, &Duckvil::Network::Client::Send, "Send");
_function = record_function<Duckvil::Network::Client, uint32_t>(_data, _type, &Duckvil::Network::Client::GetID, "GetID");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/Client.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
