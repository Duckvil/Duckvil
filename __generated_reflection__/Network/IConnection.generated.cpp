#include "Network/IConnection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
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
_namespaces.push_back("IConnection");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::IConnection>(_data, "IConnection");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_destructor = record_destructor<Duckvil::Network::IConnection>(_data, _type);
_function = record_function<Duckvil::Network::IConnection, void, IServer*, uint32_t>(_data, _type, &Duckvil::Network::IConnection::ConnectToClient, "ConnectToClient");
_function = record_function<Duckvil::Network::IConnection, void>(_data, _type, &Duckvil::Network::IConnection::Disconnect, "Disconnect");
_function = record_function<Duckvil::Network::IConnection, bool>(_data, _type, &Duckvil::Network::IConnection::IsConnected, "IsConnected");
_function = record_function<Duckvil::Network::IConnection, void, const Message&>(_data, _type, &Duckvil::Network::IConnection::Send, "Send");
_function = record_function<Duckvil::Network::IConnection, uint32_t>(_data, _type, &Duckvil::Network::IConnection::GetID, "GetID");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Network::IConnection::Owner>(_data, _ntype, "Owner");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::IConnection::Owner::SERVER, "SERVER");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Network::IConnection::Owner::CLIENT, "CLIENT");
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/IConnection.h", 3);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
