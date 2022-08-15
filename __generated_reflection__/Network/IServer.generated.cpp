#include "Network/IServer.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
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
_enum = record_enum<Duckvil::Network::BasicCommands>(_data, _ntype, "BasicCommands");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::BasicCommands::Ping, "Ping");
_enum = record_enum<Duckvil::Network::CommonCommands>(_data, _ntype, "CommonCommands");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::CommonCommands::Client_Accepted, "Client_Accepted");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::CommonCommands::Client_AssignID, "Client_AssignID");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::CommonCommands::Client_RegisterWithServer, "Client_RegisterWithServer");
_namespaces.push_back("IServer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::IServer>(_data, "IServer");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 4);
_destructor = record_destructor<Duckvil::Network::IServer>(_data, _type);
_function = record_function<Duckvil::Network::IServer, bool>(_data, _type, &Duckvil::Network::IServer::Start, "Start");
_function = record_function<Duckvil::Network::IServer, void>(_data, _type, &Duckvil::Network::IServer::Stop, "Stop");
_function = record_function<Duckvil::Network::IServer, void, size_t, bool>(_data, _type, &Duckvil::Network::IServer::Update, "Update");
_function = record_function<Duckvil::Network::IServer, void, std::shared_ptr<IConnection>, const Message&>(_data, _type, &Duckvil::Network::IServer::MessageClient, "MessageClient");
_function = record_function<Duckvil::Network::IServer, void, const Message&, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::IServer::MessageAllClients, "MessageAllClients");
_function = record_function<Duckvil::Network::IServer, void, std::shared_ptr<IConnection>>(_data, _type, &Duckvil::Network::IServer::OnClientValidated, "OnClientValidated");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/IServer.h", 4);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
