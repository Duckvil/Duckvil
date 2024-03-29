#pragma once

#include "Memory/Memory.h"

#include "Process/Process.h"

#include "Utils/Macro.h"

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_WINDOWS
    void windows_init(const Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
    bool windows_setup(data* _pData);
    void windows_write(data* _pData, const char* _csMessage);
    void windows_start(data* _pData);
    void windows_stop(data* _pData);
    void windows_wait(data* _pData);
    bool windows_terminate(data* _pData);
    void windows_cleanup(const Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData);
#endif

}}