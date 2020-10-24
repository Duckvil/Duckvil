#include "Utils\Utils.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::Utils;
_type = record_type<string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "string");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Utils");
_constructor = record_constructor<string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<string, const string&, Memory::IMemory*, Memory::__free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<string, const char*, std::size_t, Memory::IMemory*, Memory::__free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<string, const char, Memory::IMemory*, Memory::__free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(string, m_ullLength), "m_ullLength");
_property = record_property<Memory::IMemory*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(string, m_pMemory), "m_pMemory");
_property = record_property<Memory::__free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(string, m_pAllocator), "m_pAllocator");
record_function<string, string&, const char>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &string::operator=, "operator=");
record_function<string, string&, const string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &string::operator=, "operator=");
record_function<string, void, std::size_t, Memory::IMemory*, Memory::__free_list_allocator*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &string::Allocate, "Allocate");
}
