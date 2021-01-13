#include "UniTestFramework\UniTestFramework.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::UniTestFramework;
_type = record_type<Duckvil::UniTestFramework::__tests_result>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__tests_result");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "UniTestFramework");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::UniTestFramework::__tests_result, m_bTestsFailed), "m_bTestsFailed");
_type = record_type<Duckvil::UniTestFramework::Assert>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Assert");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "UniTestFramework");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_function<bool, const char*, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
record_function<bool, const std::string&, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
record_function<bool, const char*, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
record_function<bool, const A&, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
record_function<bool, const A*, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::AreEqual, "AreEqual");
record_function<bool, const char*, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
record_function<bool, const std::string&, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
record_function<bool, const char*, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
record_function<bool, const A&, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
record_function<bool, const A*, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::NotEqual, "NotEqual");
record_function<bool, const A*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::IsNULL, "IsNULL");
record_function<bool, const A*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::IsNotNULL, "IsNotNULL");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "UniTestFramework/UniTestFramework.h", 0 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
