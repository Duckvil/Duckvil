#include "TestRuntimeReflectionType/Test.h"

#include "RuntimeReflection/Recorder.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
    // _pRecorder->m_fnRecordType(_pMemoryInterface, _pAllocator, _pData, typeid(Duckvil::Test::TestType).hash_code(), "TestType");
    using namespace Duckvil::RuntimeReflection;

    DUCKVIL_RESOURCE(type_t) _type = Duckvil::RuntimeReflection::record_type<Duckvil::Test::TestType>(_pMemoryInterface, _pAllocator, _pRecorder, _pData, "TestType");

    // Duckvil::RuntimeReflection::record_property<Duckvil::Test::TestType, float>(_pMemoryInterface, _pAllocator, _pRecorder, _pData, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
    Duckvil::RuntimeReflection::record_property<float>(_pMemoryInterface, _pAllocator, _pRecorder, _pData, _type, offsetof(Duckvil::Test::TestType, m_fA), "m_fA");
    Duckvil::RuntimeReflection::record_namespace(_pMemoryInterface, _pAllocator, _pRecorder, _pData, _type, "Duckvil");
    Duckvil::RuntimeReflection::record_namespace(_pMemoryInterface, _pAllocator, _pRecorder, _pData, _type, "Test");
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