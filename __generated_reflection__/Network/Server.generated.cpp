#include "Network/Server.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(7)
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
_namespaces.push_back("Server");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::Server>(_data, "Server");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 7);
record_inheritance<IServer>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Network::Server, const Memory::FreeList&, uint16_t>(_data, _type);
_destructor = record_destructor<Duckvil::Network::Server>(_data, _type);
_function = record_function<Duckvil::Network::Server, bool>(_data, _type, &Duckvil::Network::Server::Start, "Start");
_function = record_function<Duckvil::Network::Server, void>(_data, _type, &Duckvil::Network::Server::Stop, "Stop");
_function = record_function<Duckvil::Network::Server, void, size_t, bool>(_data, _type, &Duckvil::Network::Server::Update, "Update");
_function = record_function<Duckvil::Network::Server, void>(_data, _type, &Duckvil::Network::Server::WaitForConnection, "WaitForConnection");
_function = record_function<Duckvil::Network::Server, void, std::shared_ptr<IConnection>, const Message&>(_data, _type, &Duckvil::Network::Server::MessageClient, "MessageClient");
_function = record_function<Duckvil::Network::Server, void, const Message&, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::Server::MessageAllClients, "MessageAllClients");
_function = record_function<Duckvil::Network::Server, void, NetworkSystem*>(_data, _type, &Duckvil::Network::Server::AddSystem, "AddSystem");
_function = record_function<Duckvil::Network::Server, bool, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::Server::OnClientConnect, "OnClientConnect");
_function = record_function<Duckvil::Network::Server, void, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::Server::OnClientDisconnect, "OnClientDisconnect");
_function = record_function<Duckvil::Network::Server, void, std::shared_ptr<IConnection>, Message&>(_data, _type, &Duckvil::Network::Server::OnMessage, "OnMessage");
_function = record_function<Duckvil::Network::Server, void, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::Server::OnClientValidated, "OnClientValidated");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/Server.h", 7);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 7;
}
#endif
