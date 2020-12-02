#include "Duckvil/Duckvil.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "Process/Process.h"

#undef max

namespace Duckvil {

    bool init_runtime_reflection(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

        _pModule->load(&_runtimeReflectionModule);

        duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
        duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_runtimeReflectionInit);
        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_runtimeReflectionRecorderInit);

        _pData->m_pRuntimeReflection = _runtimeReflectionInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pRuntimeReflectionRecorder = _runtimeReflectionRecorderInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pRuntimeReflectionData = _pData->m_pRuntimeReflection->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pRuntimeReflection);

        return true;
    }

    bool init_logger(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _loggerModule("Logger");

        _pModule->load(&_loggerModule);

        duckvil_logger_init_callback _loggerInit;

        _pModule->get(_loggerModule, "duckvil_logger_init", (void**)&_loggerInit);

        _pData->m_pLogger = _loggerInit(_pData->m_pMemory, _pData->m_pHeap);
        _pData->m_pLoggerData = _pData->m_pLogger->init(_pData->m_pMemory, _pData->m_pHeap);

        std::string _outLog = (std::filesystem::path(DUCKVIL_OUTPUT) / "log.log").string();

        memcpy(_pData->m_pLoggerData->m_sPathFile, _outLog.c_str(), _outLog.size());
        _pData->m_pLoggerData->m_flags = static_cast<Logger::__logger_flags>(Logger::__logger_flags::__logger_flags_console_output | Logger::__logger_flags::__logger_flags_file_output);

        DUCKVIL_LOG_INFO("Logger initialization complete!");
        DUCKVIL_LOG_INFO("Logger file output: %s", _outLog.c_str());

        return true;
    }

    bool init(__data* _pData, Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator)
    {
        _pData->m_pHeap = _pAllocator;
        _pData->m_pMemory = _pMemoryInterface;
        _pData->m_aLoadedModules = nullptr;
        _pData->m_uiLoadedModulesCount = 0;
        _pData->m_dOneSecond = 0;
        _pData->m_time = time_init();
        _pData->m_heap = Memory::FreeList(_pData->m_pMemory, _pData->m_pHeap);

        // {
        //     Memory::Vector<Memory::Vector<float>> _vec;

        //     _pData->m_heap.Allocate(_vec, 1);
            // TODO: Here is problem...
            // Details: We are allocating '_vec[0]' using heap not '_vec'
        //     _pData->m_heap.Allocate(_vec[0], 1);

        //     Memory::Vector<float>& _vecc = _vec[0];

        //     // _vec[0].Allocate(1.f);
        // }

        // {
        //     Memory::Vector<Memory::Vector<float>> _vec;

        //     _pData->m_heap.Allocate(_vec, 2);

        //     {
        //         Memory::Vector<float> _vec2;

        //         _pData->m_heap.Allocate(_vec2, 1);

        //         _vec2.Allocate(1.f);

        //         _vec2.Resize(2);

        //         _vec2.Allocate(11.f);

        //         _vec.Allocate(_vec2);
        //     }

        //     {
        //         Memory::Vector<float> _vec3;

        //         _pData->m_heap.Allocate(_vec3, 1);

        //         _vec3.Allocate(2.f);

        //         _vec.Allocate(_vec3);
        //     }

        //     _vec.Resize(3);

        //     {
        //         Memory::Vector<float> _vec4;

        //         _pData->m_heap.Allocate(_vec4, 1);

        //         _vec4.Allocate(3.f);

        //         _vec.Allocate(_vec4);
        //     }

        //     printf("%f\n", _vec[0][0]);
        //     printf("%f\n", _vec[0][1]);
        //     printf("%f\n", _vec[1][0]);
        //     printf("%f\n", _vec[2][0]);
        // }

        _pData->m_heap.Allocate(_pData->m_aEngineSystems, 1);

        _pData->m_time.init(&_pData->m_timeData);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        init_logger(_pData, &_module);
        init_runtime_reflection(_pData, &_module);

        {
            PlugNPlay::__module_information _processModuleInfo("Process");

            _module.load(&_processModuleInfo);

            void (*_duckvilProcessInit)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::Process::ftable* _pFTable);

            _module.get(_processModuleInfo, "duckvil_process_init", (void**)&_duckvilProcessInit);

            Process::ftable _processFTable;
            void* _processImplementation;

            _duckvilProcessInit(_pData->m_pMemory, _pData->m_pHeap, &_processFTable);

            Process::data _processData = {};

            _processFTable.m_fnInit(_pData->m_pMemory, _pData->m_pHeap, &_processImplementation);
            _processFTable.m_fnSetup(&_processData, _processImplementation);
            _processFTable.m_fnWrite(_processImplementation, "vcvarsall x86_amd64\n");
            _processFTable.m_fnStart(_processImplementation);

            _processFTable.m_fnWrite(_processImplementation, "cl\n_COMPLETION_TOKEN_\n");
        }

        PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

        _autoLoader.LoadAll(_pMemoryInterface, _pAllocator, &_pData->m_aLoadedModules, &_pData->m_uiLoadedModulesCount);

        DUCKVIL_LOG_INFO("Modules to load %i", _pData->m_uiLoadedModulesCount);

        for(uint32_t i = 0; i < _pData->m_uiLoadedModulesCount; i++)
        {
            const PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];
            uint32_t (*get_recorder_count)();

            _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);

            if(get_recorder_count == nullptr)
            {
                DUCKVIL_LOG_INFO("No recorder for %s", _loadedModule.m_sName.m_sText);

                continue;
            }

            DUCKVIL_LOG_INFO("Module %s is present", _loadedModule.m_sName.m_sText);

            uint32_t _recordersCount = get_recorder_count();

            for(uint32_t j = 0; j < _recordersCount; j++)
            {
                void (*record)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

                _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), (void**)&record);

                if(record == nullptr)
                {
                    // TODO: Should return false?

                    continue;
                }

                record(_pMemoryInterface, _pAllocator, _pData->m_pRuntimeReflectionRecorder, _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData);
            }
        }

        _pData->m_bRunning = false;

        {
            auto _types = RuntimeReflection::get_types(_pData->m_pRuntimeReflectionData, _pData->m_pMemory, _pData->m_pHeap);

            for(uint32_t i = 0; i < _types.Size(); i++)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _type = _types[i];
                const RuntimeReflection::__variant& _variant = RuntimeReflection::get_meta(_pData->m_pRuntimeReflectionData, _type, ReflectionFlags::ReflectionFlags_EngineSystem);

                if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max())
                {
                    if((uint8_t)_variant.m_traits & (uint8_t)RuntimeReflection::__traits::is_bool)
                    {
                        if(_pData->m_aEngineSystems.Full())
                        {
                            _pData->m_aEngineSystems.Resize(_pData->m_aEngineSystems.Size() * 2);
                        }

                        ISystem* _obj = (ISystem*)RuntimeReflection::create<int, const Memory::FreeList&>(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pRuntimeReflectionData, _type, 10, _pData->m_heap, _pData->m_pRuntimeReflectionData);
                        system _system = {};

                        _system.m_type = _type;
                        _system.m_pObject = _obj;
                        _system.m_fnUpdateCallback = RuntimeReflection::get_function_callback<ISystem>(_pData->m_pRuntimeReflectionData, _type, "Update")->m_fnFunction;
                        _system.m_fnInitCallback = RuntimeReflection::get_function_callback<bool, ISystem>(_pData->m_pRuntimeReflectionData, _type, "Init")->m_fnFunction;

                        _obj->m_pLogger = _pData->m_pLogger;
                        _obj->m_pLoggerData = _pData->m_pLoggerData;

                        _pData->m_aEngineSystems.Allocate(_system);
                    }
                }
            }

            for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); i++)
            {
                const system& _system = _pData->m_aEngineSystems[i];

                if(!(_system.m_pObject->*_system.m_fnInitCallback)())
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool start(__data* _pData, __ftable* _pFTable)
    {
        if(_pData->m_bRunning)
        {
            return false;
        }

        _pData->m_bRunning = true;

        while(_pData->m_bRunning)
        {
            _pFTable->update(_pData, _pFTable);
        }

        return true;
    }

    bool stop(__data* _pData, __ftable* _pFTable)
    {
        if(!_pData->m_bRunning)
        {
            return false;
        }

        _pData->m_bRunning = false;

        return true;
    }

    void update(__data* _pData, __ftable* _pFTable)
    {
        _pData->m_time.update(&_pData->m_timeData);

        _pData->m_dOneSecond += _pData->m_timeData.m_dDelta;

        for(uint32_t i = 0; i < _pData->m_aEngineSystems.Size(); i++)
        {
            const system& _system = _pData->m_aEngineSystems[i];

            (_system.m_pObject->*_system.m_fnUpdateCallback)();
        }

        if(_pData->m_dOneSecond >= 1.0)
        {
            _pData->m_pLogger->dispatch_logs(_pData->m_pLogger, _pData->m_pLoggerData);

            // {
            //     Memory::Array<Memory::Array<float>> _array;

            //     _pData->m_heap.Allocate(_array, 1);

            //     Memory::Array<float> _array2;

            //     _pData->m_heap.Allocate(_array2, 2);

            //     _array2.Allocate(1.f);

            //     _array.Allocate(_array2);

            //     _array[0].Allocate(2.f);

            //     printf("%f\n", _array[0][0]);
            //     printf("%f\n", _array[0][1]);
            // }

            {
                Memory::Vector<Memory::Vector<float>> _vec;

                _pData->m_heap.Allocate(_vec, 2);

                {
                    Memory::Vector<float> _vec2;

                    _pData->m_heap.Allocate(_vec2, 3);

                    _vec2.Resize(2);

                    _vec2.Allocate(1.f);
                    _vec.Allocate(_vec2);
                }

                _vec.Resize(2);

                {
                    Memory::Vector<float> _vec2;

                    _pData->m_heap.Allocate(_vec2, 1);

                    _vec2.Resize(2);

                    _vec2.Allocate(4.f);
                    _vec.Allocate(_vec2);
                }

                _vec[0].Resize(3);

                _vec[0].Allocate(2.f);
                _vec[0].Allocate(3.f);

                // printf("%f\n", _vec[0][0]);
                // printf("%f\n", _vec[0][1]);
                // printf("%f\n", _vec[0][2]);
                // printf("%f\n", _vec[1][0]);

                // Memory::Vector<float> _vec2;

                // _pData->m_heap.Allocate(_vec2, 1);
                // _vec2.Resize(2);
                // _vec2.Allocate(1.f);
            }

            {
                Memory::Vector<Memory::Array<float>> _vec;

                _pData->m_heap.Allocate(_vec, 1);

                {
                    Memory::Array<float> _arr2;

                    _pData->m_heap.Allocate(_arr2, 2);

                    _arr2.Allocate(1.f);
                    // _arr2.Allocate(2.f);
                    _vec.Allocate(_arr2);
                }

                _vec[0].Allocate(2.f);

                _vec.Resize(2);

                {
                    Memory::Array<float> _arr2;

                    _pData->m_heap.Allocate(_arr2, 2);

                    _arr2.Allocate(3.f);
                    // _arr2.Allocate(4.f);
                    _vec.Allocate(_arr2);
                }

                _vec[1].Allocate(4.f);

                DUCKVIL_LOG_INFO("%f", _vec[0][0]);
                DUCKVIL_LOG_INFO("%f", _vec[0][1]);
                DUCKVIL_LOG_INFO("%f", _vec[1][0]);
                DUCKVIL_LOG_INFO("%f", _vec[1][1]);
            }

            DUCKVIL_LOG_INFO("Delta: %f ms", _pData->m_timeData.m_dDelta * 1000.0);
            DUCKVIL_LOG_INFO("Used memory: %f of %f", (float)_pData->m_pHeap->m_ullUsed / 1024.f, (float)_pData->m_pHeap->m_ullCapacity / 1024.f);

            _pData->m_dOneSecond = 0.0;
        }

#ifdef DUCKVIL_PLATFORM_WINDOWS
        // Sleep(1);
#endif
    }

}

Duckvil::__ftable* duckvil_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::__ftable* _ftable = (Duckvil::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::__ftable), alignof(Duckvil::__ftable));

    _ftable->init = Duckvil::init;
    _ftable->start = Duckvil::start;
    _ftable->stop = Duckvil::stop;
    _ftable->update = Duckvil::update;

    return _ftable;
}