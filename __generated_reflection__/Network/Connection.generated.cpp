#include "Network/Connection.h"
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
_namespaces.push_back("Network");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Connection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::Connection>(_data, "Connection");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_inheritance<IConnection>(_data, _type, __protection::__protection_public);
record_inheritance<std::enable_shared_from_this<Connection>>(_data, _type, __protection::__protection_public);
_destructor = record_destructor<Duckvil::Network::Connection>(_data, _type);
_function = record_function<Duckvil::Network::Connection, void, asio::ip::tcp::resolver::results_type&>(_data, _type, &Duckvil::Network::Connection::ConnectToServer, "ConnectToServer");
_function = record_function<Duckvil::Network::Connection, void, IServer*, uint32_t>(_data, _type, &Duckvil::Network::Connection::ConnectToClient, "ConnectToClient");
_function = record_function<Duckvil::Network::Connection, void>(_data, _type, &Duckvil::Network::Connection::Disconnect, "Disconnect");
_function = record_function<Duckvil::Network::Connection, bool>(_data, _type, &Duckvil::Network::Connection::IsConnected, "IsConnected");
_function = record_function<Duckvil::Network::Connection, void, const Message&>(_data, _type, &Duckvil::Network::Connection::Send, "Send");
_function = record_function<Duckvil::Network::Connection, uint32_t>(_data, _type, &Duckvil::Network::Connection::GetID, "GetID");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/Connection.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
