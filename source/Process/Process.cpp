#include "Process/Process.h"

#include "Process/Platform/Windows/Process.h"

namespace Duckvil { namespace Process {

}}

void duckvil_process_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::Process::ftable* _pFTable)
{
#ifdef DUCKVIL_PLATFORM_WINDOWS
    _pFTable->m_fnInit = &Duckvil::Process::windows_init;
    _pFTable->m_fnSetup = &Duckvil::Process::windows_setup;
    _pFTable->m_fnStart = &Duckvil::Process::windows_start;
    _pFTable->m_fnStop = &Duckvil::Process::windows_stop;
    _pFTable->m_fnWrite = &Duckvil::Process::windows_write;
#endif
}