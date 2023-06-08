#pragma once

#include "Memory/Memory.h"

#include "Utils/Macro.h"

#include <string>

namespace Duckvil { namespace Process {

    const static std::string cs_CompletionToken = "_COMPLETION_TOKEN_";

    struct data
    {
        void* m_pImplementationData;
        bool m_bComplete;
        bool m_bRunning;
    };

    struct ftable
    {
        void (*m_fnInit)(const Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
        bool (*m_fnSetup)(data* _pData);
        void (*m_fnStart)(data* _pData);
        void (*m_fnStop)(data* _pData);
        void (*m_fnWait)(data* _pData);
        void (*m_fnWrite)(data* _pData, const char* _csMessage);
        bool (*m_fnTerminate)(data* _pData);
        void (*m_fnCleanup)(const Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
    };

}}

DUCKVIL_EXPORT const Duckvil::Process::ftable* duckvil_process_init();