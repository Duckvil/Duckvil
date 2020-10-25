#include "UniTestFramework/UniTestFramework.h"

#include "Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "TestType//TestType.h"

Duckvil::PlugNPlay::module __duckvil_global::m_module;
Duckvil::PlugNPlay::__module_information __duckvil_global::m_memoryModule = Duckvil::PlugNPlay::__module_information("Memory");
Duckvil::Memory::IMemory* __duckvil_global::m_pMemoryInterface;
void* __duckvil_global::m_pMemoryChunk;
void* __duckvil_global::m_pHeap;

DUCKVIL_TEST(RuntimeReflection)
{
    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory.dll");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    Duckvil::PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection.dll");

    _module.load(&_runtimeReflectionModule);

    DUCKVIL_TEST_IS_NOT_NULL(_runtimeReflectionModule.m_pModule, "Could not load 'RuntimeReflection.dll'");

    {
        duckvil_runtime_reflection_init_callback _rr_init;
        duckvil_runtime_reflection_recorder_init_callback _rr_recorder_init;

        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_rr_init);
        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_rr_recorder_init);

        DUCKVIL_TEST_IS_NOT_NULL((void*)_rr_init, "Could not get 'duckvil_runtime_reflection_init'");
        DUCKVIL_TEST_IS_NOT_NULL((void*)_rr_recorder_init, "Could not get 'duckvil_runtime_reflection_recorder_init'");

        Duckvil::RuntimeReflection::__ftable* _rr_ftable = _rr_init(_memoryInterface, _free_list);
        Duckvil::RuntimeReflection::__recorder_ftable* _rr_recorder = _rr_recorder_init(_memoryInterface, _free_list);

        DUCKVIL_TEST_IS_NOT_NULL(_rr_ftable, "Could not init runtime reflection functions table");
        DUCKVIL_TEST_IS_NOT_NULL(_rr_recorder, "Could not init runtime reflection recorder functions table");

        Duckvil::RuntimeReflection::__data* _rr_data = _rr_ftable->m_fnInit(_memoryInterface, _free_list, _rr_ftable);

        DUCKVIL_TEST_IS_NOT_NULL(_rr_data, "Could not init runtime reflection data");

        {
            Duckvil::PlugNPlay::__module_information _test_type_module("TestType.dll", DUCKVIL_TEST_OUTPUT);

            _module.load(&_test_type_module);

            DUCKVIL_TEST_IS_NOT_NULL(_test_type_module.m_pModule, "Could not load 'TestType.dll'");

            uint32_t (*get_recorder_count)();

            _module.get(_test_type_module, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);

            DUCKVIL_TEST_IS_NOT_NULL((void*)get_recorder_count, "Could not get 'duckvil_get_runtime_reflection_recorder_count'");

            uint32_t _count = get_recorder_count();

            DUCKVIL_TEST_NOT_EQUAL(_count, (uint32_t)0, "Recorders count is 0");

            for(uint32_t i = 0; i < _count; i++)
            {
                void (*test_type)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData);

                _module.get(_test_type_module, (std::string("duckvil_runtime_reflection_record_") + std::to_string(i)).c_str(), (void**)&test_type);

                DUCKVIL_TEST_IS_NOT_NULL((void*)get_recorder_count, ("Could not get 'duckvil_runtime_reflection_record_" + std::to_string(i) + "'").c_str());

                test_type(_memoryInterface, _free_list, _rr_recorder, _rr_ftable, _rr_data);
            }
        }

        Duckvil::RuntimeReflection::__duckvil_resource_type_t _t = _rr_ftable->m_fnGetType(_rr_data, "TestType");

        DUCKVIL_TEST_NOT_EQUAL(_t.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'TestType'");

        Duckvil::RuntimeReflection::__duckvil_resource_property_t _propHandle = Duckvil::RuntimeReflection::get_property_handle(_rr_data, _t, "m_fA");

        DUCKVIL_TEST_NOT_EQUAL(_propHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'm_fA'");

        Duckvil::RuntimeReflection::__duckvil_resource_constructor_t _consHandle = Duckvil::RuntimeReflection::get_constructor_handle(_rr_data, _t);

        DUCKVIL_TEST_NOT_EQUAL(_consHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get TestType default constructor");

        Duckvil::RuntimeReflection::__duckvil_resource_function_t _funcHandle = Duckvil::RuntimeReflection::get_function_handle<float, float>(_rr_data, _t, "test2");

        DUCKVIL_TEST_NOT_EQUAL(_funcHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'test2'");

        void* _testT = Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data, _t, 10.f);

        DUCKVIL_TEST_IS_NOT_NULL(_testT, "Could not create 'TestType' object");

        float* _prop = (float*)Duckvil::RuntimeReflection::get_property(_rr_data, "m_fA", (Duckvil::Test::TestType*)_testT);

        DUCKVIL_TEST_IS_NOT_NULL(_prop, "Could not get 'm_fA' property");

        _prop = (float*)Duckvil::RuntimeReflection::get_property(_rr_data, _t, "m_fA", _testT);

        DUCKVIL_TEST_IS_NOT_NULL(_prop, "Could not get 'm_fA' property");

        Duckvil::RuntimeReflection::get_type(_rr_data, "TestType", "Duckvil", "Test");

        {
            Duckvil::RuntimeReflection::invoke(_rr_data, "test", (Duckvil::Test::TestType*)_testT);
            Duckvil::RuntimeReflection::invoke(_rr_data, "test2", (Duckvil::Test::TestType*)_testT, 10.f, 20.f);
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test3");
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test4", 30.f, 40.f);
            Duckvil::RuntimeReflection::invoke(_rr_data, _t, _funcHandle, _testT, 10.f, 10.f);
            int _res = Duckvil::RuntimeReflection::invoke<int>(_rr_data, "test5", (Duckvil::Test::TestType*)_testT, 10.f, 20.f);
            _res = Duckvil::RuntimeReflection::invoke<int, Duckvil::Test::TestType>(_rr_data, "test6");
            Duckvil::RuntimeReflection::invoke(_rr_data, "test7", (Duckvil::Test::TestType*)_testT);
            Duckvil::RuntimeReflection::invoke(_rr_data, "test7", (Duckvil::Test::TestType*)_testT, 1);
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test8");
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test8", 1);
        }

        {
            int _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, "a");
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, "b");
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, 1);
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, 1.f);
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, _propHandle, 1);
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, _propHandle, "a");
            _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, _consHandle, "a");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}