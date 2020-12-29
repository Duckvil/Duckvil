#include "UniTestFramework\UniTestFramework.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
Duckvil::Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemoryInterface, _pAllocator, 1);
using namespace Duckvil::UniTestFramework;
_type = record_type<Duckvil::UniTestFramework::__tests_result>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__tests_result");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "UniTestFramework");
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::UniTestFramework::__tests_result, m_bTestsFailed), "m_bTestsFailed");
record_function<void*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::__tests_result::Cast, "Cast");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::UniTestFramework::Assert>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Assert");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "UniTestFramework");
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
record_function<void*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::UniTestFramework::Assert::Cast, "Cast");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
return _types;
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
