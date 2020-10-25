#pragma once

#include "Memory/Memory.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"

#include "Memory/SlotArray.h"

#include "Logger/Logger.h"

namespace Duckvil {

    struct __data
    {
        bool m_bRunning;

        Memory::IMemory* m_pMemory;
        Memory::__free_list_allocator* m_pHeap;

        RuntimeReflection::__ftable* m_pRuntimeReflection;
        RuntimeReflection::__recorder_ftable* m_pRuntimeReflectionRecorder;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        PlugNPlay::__module_information* m_aLoadedModules;
        uint32_t m_uiLoadedModulesCount;

        Logger::__ftable* m_pLogger;
        Logger::__data* m_pLoggerData;
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