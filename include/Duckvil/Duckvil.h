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

#include "Duckvil/Time.h"
#include "Duckvil/ReflectionFlags.h"
#include "Duckvil/ISystem.h"

#include "RuntimeCompiler/RuntimeCompilerSystem.h"

namespace Duckvil {

    struct system
    {
        RuntimeReflection::__duckvil_resource_type_t m_type;
        void** m_pObject;
        ISystem* m_pISystem;
        void (ISystem::*m_fnUpdateCallback)();
        bool (ISystem::*m_fnInitCallback)();
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

        RuntimeCompiler::RuntimeCompilerSystem* m_pRuntimeCompiler;
        void (ISystem::*m_fnRuntimeCompilerUpdate)();
        bool (ISystem::*m_fnRuntimeCompilerInit)();

        Logger::__ftable* m_pLogger;
        Logger::__data* m_pLoggerData;

        __time_ftable m_time;
        __time_data m_timeData;
        double m_dOneSecond;

        Memory::Vector<system> m_aEngineSystems;
    };

    struct __ftable
    {
        bool (*init)(__data* _pData, Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator);
        bool (*start)(__data* _pData, __ftable* _pFTable);
        bool (*stop)(__data* _pData, __ftable* _pFTable);
        void (*update)(__data* _pData, __ftable* _pFTable);
    };

}

typedef Duckvil::__ftable* (*duckvil_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::__ftable* duckvil_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);