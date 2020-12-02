#pragma once

#include "Memory/Memory.h"

#include "Utils/Macro.h"

#include <string>

namespace Duckvil { namespace Process {

    const static std::string cs_CompletionToken = "_COMPLETION_TOKEN_";

    struct data
    {
        bool m_bComplete;
        bool m_bRunning;
    };

    struct ftable
    {
        void (*m_fnInit)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, void** _pImplementationData);
        bool (*m_fnSetup)(void* _pData, void* _pImplementationData);
        void (*m_fnStart)(void* _pImplementationData);
        void (*m_fnStop)(void* _pImplementationData);
        void (*m_fnWrite)(void* _pImplementationData, const char* _csMessage);
    };

}}

DUCKVIL_EXPORT void duckvil_process_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::Process::ftable* _pFTable);