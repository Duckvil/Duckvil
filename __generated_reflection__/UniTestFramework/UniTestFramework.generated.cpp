#include "UniTestFramework/UniTestFramework.h"
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
_namespaces.push_back("UniTestFramework");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("__tests_result");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::UniTestFramework;
_type = record_type<Duckvil::UniTestFramework::__tests_result>(_data, "__tests_result");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "UniTestFramework");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool>(_data, _type, offsetof(Duckvil::UniTestFramework::__tests_result, m_bTestsFailed), "m_bTestsFailed");
_namespaces.pop_back();
_namespaces.push_back("Assert");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::UniTestFramework::Assert>(_data, "Assert");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "UniTestFramework");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_function = record_function<bool, const char*, const char*>(_data, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
_function = record_function<bool, const std::string&, const char*>(_data, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
_function = record_function<bool, const char*, const std::string&>(_data, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
_function = record_function<bool, const char*, const char*>(_data, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
_function = record_function<bool, const std::string&, const char*>(_data, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
_function = record_function<bool, const char*, const std::string&>(_data, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "UniTestFramework/UniTestFramework.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
