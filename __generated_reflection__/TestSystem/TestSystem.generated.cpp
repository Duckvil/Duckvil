#include "TestSystem\TestSystem.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::LOL;
_type = record_type<Duckvil::LOL::test>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "test");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "LOL");
_property = record_property<int>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::LOL::test, a), "a");
_type = record_type<Duckvil::LOL::test2>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "test2");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "LOL");
_property = record_property<int>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::LOL::test2, a), "a");
_type = record_type<Duckvil::LOL::aaa>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "aaa");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "LOL");
record_function<Duckvil::LOL::aaa, void, const test&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::aaa::OnEvent, "OnEvent");
_type = record_type<Duckvil::LOL::aaa2>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "aaa2");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "LOL");
record_function<Duckvil::LOL::aaa2, void, const test&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::aaa2::OnEvent, "OnEvent");
_type = record_type<Duckvil::LOL::Test>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "Test");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "LOL");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<ISystem>(_pData), __protection::__protection_public);
_constructor = record_constructor<Duckvil::LOL::Test, int, const Memory::FreeList&, RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::LOL::Test, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::Test::Init, "Init");
record_function<Duckvil::LOL::Test, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::Test::Update, "Update");
record_function<Duckvil::LOL::Test, void, const test&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::Test::OnEvent, "OnEvent");
record_function<Duckvil::LOL::Test, void, const test2&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::LOL::Test::OnEvent, "OnEvent");
}
