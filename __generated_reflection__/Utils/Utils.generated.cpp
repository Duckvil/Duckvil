#include "Utils/Utils.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(11)
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
_namespaces.push_back("Utils");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("string");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.pop_back();
_namespaces.push_back("string");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Utils;
_type = record_type<Duckvil::Utils::string>(_data, "string");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Utils");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 11);
_constructor = record_constructor<Duckvil::Utils::string>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, string const&, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, string&&>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, std::size_t, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, char const*, std::size_t, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, char*, std::size_t, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, char*, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, char const*, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, std::string const&, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::string, std::filesystem::path const&, Memory::ftable const*, Memory::free_list_allocator*>(_data, _type);
_destructor = record_destructor<Duckvil::Utils::string>(_data, _type);
_property = record_property<char*>(_data, _type, offsetof(Duckvil::Utils::string, m_sText), "m_sText");
_property = record_property<std::size_t>(_data, _type, offsetof(Duckvil::Utils::string, m_ullLength), "m_ullLength");
_property = record_property<Memory::ftable const*>(_data, _type, offsetof(Duckvil::Utils::string, m_pMemory), "m_pMemory");
_property = record_property<Memory::free_list_allocator*>(_data, _type, offsetof(Duckvil::Utils::string, m_pAllocator), "m_pAllocator");
_function = record_function<Duckvil::Utils::string, string&, string&&>(_data, _type, &Duckvil::Utils::string::operator=, "operator=");
_function = record_function<Duckvil::Utils::string, string&, string const&>(_data, _type, &Duckvil::Utils::string::operator=, "operator=");
_function = record_function<Duckvil::Utils::string, char&, int>(_data, _type, &Duckvil::Utils::string::operator[], "operator[]");
_function = record_function<Duckvil::Utils::string, char, int>(_data, _type, &Duckvil::Utils::string::operator[], "operator[]");
_function = record_function<Duckvil::Utils::string, string, string const&>(_data, _type, &Duckvil::Utils::string::operator/, "operator/");
_function = record_function<string, string const&>(_data, _type, &Duckvil::Utils::string::parentPath, "parentPath");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/Utils.h", 11);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 11;
}
#endif
