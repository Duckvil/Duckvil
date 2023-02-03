#include "Network/Message.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(5)
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
_namespaces.push_back("MessageType");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Network;
_type = record_type<Duckvil::Network::MessageType>(_data, "MessageType");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 5);
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::Network::MessageType, m_ullTypeID), "m_ullTypeID");
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::Network::MessageType, m_ullValue), "m_ullValue");
_constructor = record_constructor<Duckvil::Network::MessageType>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("MessageHeader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Network::MessageHeader>(_data, "MessageHeader");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 5);
_property = record_property<Duckvil::Network::MessageType>(_data, _type, offsetof(Duckvil::Network::MessageHeader, m_id), "m_id");
_property = record_property<size_t>(_data, _type, offsetof(Duckvil::Network::MessageHeader, m_ullSize), "m_ullSize");
_constructor = record_constructor<Duckvil::Network::MessageHeader>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("Message");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Network::Message>(_data, "Message");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 5);
_property = record_property<Duckvil::Network::MessageHeader>(_data, _type, offsetof(Duckvil::Network::Message, m_header), "m_header");
_property = record_property<std::vector<uint8_t>>(_data, _type, offsetof(Duckvil::Network::Message, m_aBody), "m_aBody");
_constructor = record_constructor<Duckvil::Network::Message>(_data, _type);
_namespaces.pop_back();
_namespaces.push_back("OwnedMessage");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Network::OwnedMessage>(_data, "OwnedMessage");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Network");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 5);
_property = record_property<std::shared_ptr<IConnection>>(_data, _type, offsetof(Duckvil::Network::OwnedMessage, m_pRemote), "m_pRemote");
_property = record_property<Duckvil::Network::Message>(_data, _type, offsetof(Duckvil::Network::OwnedMessage, m_message), "m_message");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Network/Message.h", 5);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 5;
}
#endif
