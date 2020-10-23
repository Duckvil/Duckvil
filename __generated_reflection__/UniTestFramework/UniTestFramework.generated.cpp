#include "UniTestFramework\UniTestFramework.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::UniTestFramework;
_type = record_type<__tests_result>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__tests_result");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__tests_result, m_bTestsFailed), "m_bTestsFailed");
_type = record_type<Assert>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Assert");
record_function<bool, const char*, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::AreEqual, "AreEqual");
record_function<bool, const std::string&, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::AreEqual, "AreEqual");
record_function<bool, const char*, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::AreEqual, "AreEqual");
record_function<bool, const A&, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::AreEqual, "AreEqual");
record_function<bool, const A*, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::AreEqual, "AreEqual");
record_function<bool, const char*, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::NotEqual, "NotEqual");
record_function<bool, const std::string&, const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::NotEqual, "NotEqual");
record_function<bool, const char*, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::NotEqual, "NotEqual");
record_function<bool, const A&, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::NotEqual, "NotEqual");
record_function<bool, const A*, const A&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::NotEqual, "NotEqual");
record_function<bool, const A*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::IsNULL, "IsNULL");
record_function<bool, const A*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Assert::IsNotNULL, "IsNotNULL");
}
