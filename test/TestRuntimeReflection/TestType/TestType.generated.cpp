#include "TestType.h"
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
_namespaces.push_back("Test");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("TestType");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Test;
_type = record_type<Duckvil::Test::TestType>(_data, "TestType");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Test");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_meta(_data, _type, "a", 10);
record_meta(_data, _type, "b", 20);
record_meta(_data, _type, 1, 30);
record_meta(_data, _type, 1.f, 40);
_constructor = record_constructor<Duckvil::Test::TestType>(_data, _type);
record_meta(_data, _type, _constructor, "a", 1);
_constructor = record_constructor<Duckvil::Test::TestType, float>(_data, _type);
_destructor = record_destructor<Duckvil::Test::TestType>(_data, _type);
_property = record_property<float>(_data, _type, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
record_meta(_data, _type, _property, 1, 11);
record_meta(_data, _type, _property, "a", 22);
_function = record_function<Duckvil::Test::TestType, void>(_data, _type, &Duckvil::Test::TestType::test, "test");
_function = record_function<Duckvil::Test::TestType, void, float, float>(_data, _type, &Duckvil::Test::TestType::test2, "test2");
_function = record_function<void>(_data, _type, &Duckvil::Test::TestType::test3, "test3");
_function = record_function<void, float, float>(_data, _type, &Duckvil::Test::TestType::test4, "test4");
_function = record_function<Duckvil::Test::TestType, int, float, float>(_data, _type, &Duckvil::Test::TestType::test5, "test5");
_function = record_function<int>(_data, _type, &Duckvil::Test::TestType::test6, "test6");
_function = record_function<Duckvil::Test::TestType, void>(_data, _type, &Duckvil::Test::TestType::test7, "test7");
_function = record_function<Duckvil::Test::TestType, void, int>(_data, _type, &Duckvil::Test::TestType::test7, "test7");
_function = record_function<void>(_data, _type, &Duckvil::Test::TestType::test8, "test8");
_function = record_function<void, int>(_data, _type, &Duckvil::Test::TestType::test8, "test8");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "TestType/TestType.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
