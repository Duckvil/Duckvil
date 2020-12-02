#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Process {

    void windows_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, void** _pImplementationData);
    bool windows_setup(void* _pData, void* _pImplementationData);
    void windows_write(void* _pImplementationData, const char* _csMessage);
    void windows_start(void* _pImplementationData);
    void windows_stop(void* _pImplementationData);

}}