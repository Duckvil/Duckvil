#pragma once

#include "Memory/Memory.h"
#include "Memory/SlotArray.h"
#include "Memory/FreeList.h"
#include "Memory/Vector.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"

#include "Logger/Logger.h"

#include "Engine/Time.h"
#include "Engine/ReflectionFlags.h"
#include "Engine/ISystem.h"

#include "HotReloader/RuntimeCompilerSystem.h"

#include "Event/ImmediatePool.h"

namespace Duckvil {

    struct system
    {
    // TODO: Currently it is workaround for member callbacks, need to be fixed(Reflection recorder)
        typedef void (ISystem::*UpdateCallback)();
        typedef bool (ISystem::*InitCallback)();

        RuntimeReflection::__duckvil_resource_type_t m_type;
        // void** m_pObject;
        HotReloader::ITrackKeeper* m_pTrackKeeper;
        ISystem* m_pISystem;
        system::UpdateCallback m_fnUpdateCallback;
        system::InitCallback m_fnInitCallback;
    };

    struct __data
    {
        bool m_bRunning;

        Memory::IMemory* m_pMemory;
        Memory::__free_list_allocator* m_pHeap;

        Memory::FreeList m_heap;

        RuntimeReflection::__ftable* m_pRuntimeReflection;
        RuntimeReflection::__recorder_ftable* m_pRuntimeReflectionRecorder;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        duckvil_recorderd_types* m_aRecordedTypes;
        size_t m_ullRecordedTypesCount;

        PlugNPlay::__module_information* m_aLoadedModules;
        uint32_t m_uiLoadedModulesCount;

        HotReloader::RuntimeCompilerSystem* m_pRuntimeCompiler;
        system::UpdateCallback m_fnRuntimeCompilerUpdate;
        system::InitCallback m_fnRuntimeCompilerInit;

        Logger::__ftable* m_pLogger;
        Logger::__data* m_pLoggerData;

        __time_ftable m_time;
        __time_data m_timeData;
        double m_dOneSecond;

        Event::Pool<Event::mode::immediate> m_eventPool;

        Memory::Vector<Duckvil::system> m_aEngineSystems;
    };

    struct __ftable
    {
        bool (*init)(Duckvil::__data* _pData, Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator);
        bool (*start)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        bool (*stop)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        void (*update)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
    };

}

typedef Duckvil::__ftable* (*duckvil_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::__ftable* duckvil_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);