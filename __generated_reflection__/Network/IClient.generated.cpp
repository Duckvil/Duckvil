#include "Network/IClient.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
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
_namespaces.push_back("IClient");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::IClient>(_data, "IClient");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_destructor = record_destructor<Duckvil::Network::IClient>(_data, _type);
_function = record_function<Duckvil::Network::IClient, bool, const std::string&, const uint16_t>(_data, _type, &Duckvil::Network::IClient::Connect, "Connect");
_function = record_function<Duckvil::Network::IClient, void>(_data, _type, &Duckvil::Network::IClient::Disconnect, "Disconnect");
_function = record_function<Duckvil::Network::IClient, bool>(_data, _type, &Duckvil::Network::IClient::IsConnected, "IsConnected");
_function = record_function<Duckvil::Network::IClient, void>(_data, _type, &Duckvil::Network::IClient::Update, "Update");
_function = record_function<Duckvil::Network::IClient, void, NetworkSystem*>(_data, _type, &Duckvil::Network::IClient::AddSystem, "AddSystem");
_function = record_function<Duckvil::Network::IClient, tsqueue<OwnedMessage>&>(_data, _type, &Duckvil::Network::IClient::Incoming, "Incoming");
_function = record_function<Duckvil::Network::IClient, void, const Message&>(_data, _type, &Duckvil::Network::IClient::Send, "Send");
_function = record_function<Duckvil::Network::IClient, uint32_t>(_data, _type, &Duckvil::Network::IClient::GetID, "GetID");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/IClient.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
