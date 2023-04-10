#include "Engine/runtime_reflection.h"

namespace Duckvil {

    bool init_runtime_reflection_module(__data* _pData, PlugNPlay::__module* _pModule)
    {
        PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

        _pModule->load(&_runtimeReflectionModule);

        duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
        duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", reinterpret_cast<void**>(&_runtimeReflectionInit));
        _pModule->get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", reinterpret_cast<void**>(&_runtimeReflectionRecorderInit));

        _pData->m_pRuntimeReflection = _runtimeReflectionInit();
        _pData->m_pRuntimeReflectionRecorder = _runtimeReflectionRecorderInit();
        _pData->m_pRuntimeReflectionData = _pData->m_pRuntimeReflection->m_fnInit(_pData->m_pMemory, _pData->m_pHeap, _pData->m_pRuntimeReflection);

        RuntimeReflection::make_current({ _pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _pData->m_pRuntimeReflectionRecorder });

        _pData->m_pRuntimeReflectionData->m_pEvents = static_cast<Event::Pool<Event::mode::immediate>*>(_pData->m_heap.Allocate<Event::Pool<Event::mode::immediate>>(_pData->m_heap));

        return true;
    }

    void init_runtime_reflection(__data* _pData, const PlugNPlay::__module& _module)
    {
        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = _pData->m_heap.GetMemoryInterface(),
            ._pAllocator = _pData->m_heap.GetAllocator(),
            ._pFunctions = RuntimeReflection::get_current().m_pRecorder,
            ._pData = RuntimeReflection::get_current().m_pReflectionData
        };

        for(uint32_t i = 0; i < _pData->m_aLoadedModules.Size(); ++i)
        {
            PlugNPlay::__module_information& _loadedModule = _pData->m_aLoadedModules[i];

            try
            {
                printf("RuntimeReflection: Loading %s\n", _loadedModule.m_sName.m_sText);

                Memory::Vector<duckvil_recorderd_types> _types = _stuff._pFunctions->m_fnRecordModule(_stuff, _module, _loadedModule, _pData->m_heap);

                for(uint32_t _i = 0; _i < _types.Size(); ++_i)
                {
                    if(_pData->m_aRecordedTypes.Full())
                    {
                        _pData->m_aRecordedTypes.Resize(_pData->m_aRecordedTypes.Size() * 2);
                    }

                    _pData->m_aRecordedTypes.Allocate(_types[_i]);
                }
            }
            catch(const RuntimeReflection::recorders_count_getter_not_found& _e)
            {
                printf("RuntimeReflection: Skipping %s\n", _loadedModule.m_sName.m_sText);

	            continue;
            }

            void (*make_current_runtime_reflection_context)(const duckvil_frontend_reflection_context&) = nullptr;
            void (*make_current_heap_context)(const Memory::free_list_context&) = nullptr;
            void (*make_current_ecs_context)() = nullptr;

            _module.get(_loadedModule, "duckvil_plugin_make_current_runtime_reflection_context", reinterpret_cast<void**>(&make_current_runtime_reflection_context));
            _module.get(_loadedModule, "duckvil_plugin_make_current_heap_context", reinterpret_cast<void**>(&make_current_heap_context));
            _module.get(_loadedModule, "duckvil_plugin_make_current_ecs_context", reinterpret_cast<void**>(&make_current_ecs_context));

            if(make_current_runtime_reflection_context != nullptr)
            {
                make_current_runtime_reflection_context(RuntimeReflection::get_current());
            }

            if(make_current_heap_context != nullptr)
            {
                make_current_heap_context(Memory::heap_get_current());
            }

            if(make_current_ecs_context != nullptr)
            {
                make_current_ecs_context();
            }
        }

        RuntimeReflection::record_meta(_stuff, RuntimeReflection::get_type<__data>(), "Time", &(_pData->m_timeData));
    }

}