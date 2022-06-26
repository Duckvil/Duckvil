#include "TestType.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Test;
_type = record_type<Duckvil::Test::TestType>(_data, "TestType");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Test");
record_meta(_data, _type, "a", 10);
record_meta(_data, _type, "b", 20);
record_meta(_data, _type, 1, 30);
record_meta(_data, _type, 1.f, 40);
_constructor = record_constructor<Duckvil::Test::TestType>(_data, _type);
record_meta(_data, _type, _constructor, "a", 1);
_constructor = record_constructor<Duckvil::Test::TestType, float>(_data, _type);
_property = record_property<float>(_data, _type, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
record_meta(_data, _type, _property, 1, 11);
record_meta(_data, _type, _property, "a", 22);
record_function<Duckvil::Test::TestType, void>(_data, _type, &Duckvil::Test::TestType::test, "test");
record_function<Duckvil::Test::TestType, void, float, float>(_data, _type, &Duckvil::Test::TestType::test2, "test2");
record_function<void>(_data, _type, &Duckvil::Test::TestType::test3, "test3");
record_function<void, float, float>(_data, _type, &Duckvil::Test::TestType::test4, "test4");
record_function<Duckvil::Test::TestType, int, float, float>(_data, _type, &Duckvil::Test::TestType::test5, "test5");
record_function<int>(_data, _type, &Duckvil::Test::TestType::test6, "test6");
record_function<Duckvil::Test::TestType, void>(_data, _type, &Duckvil::Test::TestType::test7, "test7");
record_function<Duckvil::Test::TestType, void, int>(_data, _type, &Duckvil::Test::TestType::test7, "test7");
record_function<void>(_data, _type, &Duckvil::Test::TestType::test8, "test8");
record_function<void, int>(_data, _type, &Duckvil::Test::TestType::test8, "test8");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = (Duckvil::RuntimeReflection::__duckvil_resource_type_t*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _recordedTypes.size() * sizeof(Duckvil::RuntimeReflection::__duckvil_resource_type_t), 8);
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "test/TestRuntimeReflection/TestType/TestType.h", 0 };
}
