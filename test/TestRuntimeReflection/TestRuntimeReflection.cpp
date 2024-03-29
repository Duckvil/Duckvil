#include "UniTestFramework/UniTestFramework.h"

#include "Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Meta.h"

#include "TestType/TestType.h"

#include "Memory/FreeList.h"

Duckvil::PlugNPlay::__module __duckvil_global::m_module;
Duckvil::PlugNPlay::__module_information __duckvil_global::m_memoryModule = Duckvil::PlugNPlay::__module_information("Memory");
Duckvil::Memory::ftable* __duckvil_global::m_pMemoryInterface;
void* __duckvil_global::m_pMemoryChunk;
void* __duckvil_global::m_pHeap;

DUCKVIL_TEST(RuntimeReflection)
{
    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::ftable* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::linear_allocator* _mainMemoryAllocator = 0;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::free_list_allocator* _free_list = _memoryInterface->m_fnLinearAllocateFreeListAllocator(_mainMemoryAllocator, 512 * 1024);

    Duckvil::Memory::FreeList _heap(_memoryInterface, _free_list);

    Duckvil::PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

    _module.load(&_runtimeReflectionModule);

    DUCKVIL_TEST_IS_NOT_NULL(_runtimeReflectionModule.m_pModule, "Could not load 'RuntimeReflection'");

    {
        duckvil_runtime_reflection_init_callback _rr_init;
        duckvil_runtime_reflection_recorder_init_callback _rr_recorder_init;

        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_rr_init);
        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_rr_recorder_init);

        DUCKVIL_TEST_IS_NOT_NULL((void*)_rr_init, "Could not get 'duckvil_runtime_reflection_init'");
        DUCKVIL_TEST_IS_NOT_NULL((void*)_rr_recorder_init, "Could not get 'duckvil_runtime_reflection_recorder_init'");

        Duckvil::RuntimeReflection::__ftable* _rr_ftable = _rr_init();
        Duckvil::RuntimeReflection::__recorder_ftable* _rr_recorder = _rr_recorder_init();

        DUCKVIL_TEST_IS_NOT_NULL(_rr_ftable, "Could not init runtime reflection functions table");
        DUCKVIL_TEST_IS_NOT_NULL(_rr_recorder, "Could not init runtime reflection recorder functions table");

        Duckvil::RuntimeReflection::__data* _rr_data = _rr_ftable->m_fnInit(_memoryInterface, _free_list, _rr_ftable);

        DUCKVIL_TEST_IS_NOT_NULL(_rr_data, "Could not init runtime reflection data");

        _rr_data->m_pEvents = static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_heap.Allocate<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>>(_heap));

        {
            Duckvil::PlugNPlay::__module_information _test_type_module("TestType", DUCKVIL_TEST_OUTPUT);

            _module.load(&_test_type_module);

            DUCKVIL_TEST_IS_NOT_NULL(_test_type_module.m_pModule, "Could not load 'TestType'");

            Duckvil::RuntimeReflection::GetRecordersCountFunction get_recorder_count = nullptr;

            _module.get(_test_type_module, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);

            DUCKVIL_TEST_IS_NOT_NULL((void*)get_recorder_count, "Could not get 'duckvil_get_runtime_reflection_recorder_count'");

            uint32_t _count = get_recorder_count();

            DUCKVIL_TEST_NOT_EQUAL(_count, (uint32_t)0, "Recorders count is 0");

            duckvil_runtime_reflection_recorder_stuff _stuff =
            {
                ._pMemoryInterface = _memoryInterface,
                ._pAllocator = _free_list,
                ._pFunctions = _rr_recorder,
                ._pData = _rr_data
            };

            for(uint32_t i = 0; i < _count; i++)
            {
                //duckvil_recorderd_types (*test_type)(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData);
                Duckvil::RuntimeReflection::RecordFunction record = nullptr;

                _module.get(_test_type_module, (std::string("duckvil_runtime_reflection_record_") + std::to_string(i)).c_str(), (void**)&record);

                DUCKVIL_TEST_IS_NOT_NULL((void*)get_recorder_count, ("Could not get 'duckvil_runtime_reflection_record_" + std::to_string(i) + "'").c_str());

                record(_stuff);
            }
        }

        Duckvil::RuntimeReflection::__duckvil_resource_type_t _t = Duckvil::RuntimeReflection::get_type(_rr_ftable, _rr_data, "TestType"); // _rr_ftable->m_fnGetTypeHandleByName(_rr_data, "TestType");

        DUCKVIL_TEST_NOT_EQUAL(_t.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'TestType'");

        Duckvil::RuntimeReflection::__duckvil_resource_property_t _propHandle = Duckvil::RuntimeReflection::get_property(_rr_ftable, _rr_data, _t, "m_fA"); // _rr_ftable->m_fnGetPropertyHandleByName(_rr_data, _t, "m_fA"); // Duckvil::RuntimeReflection::get_property_handle(_rr_data, _t, "m_fA");

        DUCKVIL_TEST_NOT_EQUAL(_propHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'm_fA'");

        Duckvil::RuntimeReflection::__duckvil_resource_constructor_t _consHandle = Duckvil::RuntimeReflection::get_constructor_handle(_rr_ftable, _rr_data, _t);

        DUCKVIL_TEST_NOT_EQUAL(_consHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get TestType default constructor");

        Duckvil::RuntimeReflection::__duckvil_resource_function_t _funcHandle = Duckvil::RuntimeReflection::get_function_handle<float, float>(_rr_ftable, _rr_data, _t, "test2");

        DUCKVIL_TEST_NOT_EQUAL(_funcHandle.m_ID, DUCKVIL_SLOT_ARRAY_INVALID_HANDLE, "Could not get 'test2'");

        void* _testT = Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_ftable, _rr_data, _t, false, 10.f);

        DUCKVIL_TEST_IS_NOT_NULL(_testT, "Could not create 'TestType' object");

        float* _prop = (float*)Duckvil::RuntimeReflection::get_property(_rr_ftable, _rr_data, "m_fA", (Duckvil::Test::TestType*)_testT);

        DUCKVIL_TEST_IS_NOT_NULL(_prop, "Could not get 'm_fA' property");
        DUCKVIL_TEST_EQUAL(10.f, *_prop, "Wrong property value");

        /*_prop = (float*)_rr_ftable->m_fnGetPropertyByName(_rr_data, _t, "m_fA", _testT); //Duckvil::RuntimeReflection::get_property(_rr_data, _t, "m_fA", _testT);

        DUCKVIL_TEST_IS_NOT_NULL(_prop, "Could not get 'm_fA' property");
        DUCKVIL_TEST_EQUAL(10.f, *_prop, "Wrong property value");*/

        Duckvil::RuntimeReflection::get_type(_rr_data, "TestType", "Duckvil", "Test");

        {
            // TODO:
            /*Duckvil::RuntimeReflection::invoke(_rr_data, "test", (Duckvil::Test::TestType*)_testT);
            Duckvil::RuntimeReflection::invoke(_rr_data, "test2", (Duckvil::Test::TestType*)_testT, 10.f, 20.f);
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test3");
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test4", 30.f, 40.f);
            Duckvil::RuntimeReflection::invoke(_rr_data, _t, _funcHandle, _testT, 10.f, 10.f);
            int _res = Duckvil::RuntimeReflection::invoke<int>(_rr_data, "test5", (Duckvil::Test::TestType*)_testT, 10.f, 20.f);
            _res = Duckvil::RuntimeReflection::invoke<int, Duckvil::Test::TestType>(_rr_data, "test6");
            Duckvil::RuntimeReflection::invoke(_rr_data, "test7", (Duckvil::Test::TestType*)_testT);
            Duckvil::RuntimeReflection::invoke(_rr_data, "test7", (Duckvil::Test::TestType*)_testT, 1);
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test8");
            Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test8", 1);*/
        }

        {
            DUCKVIL_TEST_EQUAL(10, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, "a"), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(20, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, "b"), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(30, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, 1), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(40, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, 1.f), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(11, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, _propHandle, 1), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(22, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, _propHandle, "a"), "Wrong meta value");
            DUCKVIL_TEST_EQUAL(1, Duckvil::RuntimeReflection::get_meta_value<int>(_rr_ftable, _rr_data, _t, _consHandle, "a"), "Wrong meta value");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}

int main(int argc, char* argv[])
{
    return DUCKVIL_TEST_FAILED;
}