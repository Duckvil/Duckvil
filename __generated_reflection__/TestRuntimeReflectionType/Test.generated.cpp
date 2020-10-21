#include "TestRuntimeReflectionType\Test.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
using namespace Duckvil::Test;
_type = record_type<TestType>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "TestType");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "a", 10);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "b", 20);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, 1, 30);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, 1.f, 40);
record_constructor<TestType>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_constructor<TestType, float>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
DUCKVIL_RESOURCE(property_t) _property = record_property<float>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(TestType, m_fA), "m_fA");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _property, 1, 11);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _property, "a", 22);
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &TestType::test, "test");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &TestType::test2, "test2");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &TestType::test3, "test3");
record_function(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &TestType::test4, "test4");
}
