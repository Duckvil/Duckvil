#include "TestRuntimeReflectionType/Test.h"

#include "RuntimeReflection/Recorder.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
    // _pRecorder->m_fnRecordType(_pMemoryInterface, _pAllocator, _pData, typeid(Duckvil::Test::TestType).hash_code(), "TestType");
    using namespace Duckvil::RuntimeReflection;

    DUCKVIL_RESOURCE(type_t) _type = Duckvil::RuntimeReflection::record_type<Duckvil::Test::TestType>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "TestType");

    // record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<Duckvil::Test::TestType>(_pData), __protection::__protection_public);
    // record_property<Duckvil::Test::TestType, float>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
    record_property<float>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
    record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
    record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Test");
}

DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(1)

namespace Duckvil { namespace Test {

    TestType::TestType()
    {

    }

    TestType::~TestType()
    {

    }

}}