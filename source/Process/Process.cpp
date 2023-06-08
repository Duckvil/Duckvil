#include "Process/Process.h"

#include "Process/Platform/Windows/Process.h"
#include "Process/Platform/Linux/Process.h"

namespace Duckvil { namespace Process {

}}

const Duckvil::Process::ftable* duckvil_process_init()
{
    static Duckvil::Process::ftable _ftable;

#ifdef DUCKVIL_PLATFORM_WINDOWS
    _ftable.m_fnInit = &Duckvil::Process::windows_init;
    _ftable.m_fnSetup = &Duckvil::Process::windows_setup;
    _ftable.m_fnStart = &Duckvil::Process::windows_start;
    _ftable.m_fnStop = &Duckvil::Process::windows_stop;
    _ftable.m_fnWait = &Duckvil::Process::windows_wait;
    _ftable.m_fnWrite = &Duckvil::Process::windows_write;
    _ftable.m_fnTerminate = &Duckvil::Process::windows_terminate;
    _ftable.m_fnCleanup = &Duckvil::Process::windows_cleanup;
#else
#ifdef DUCKVIL_PLATFORM_LINUX
    _ftable.m_fnInit = &Duckvil::Process::linux_init;
    _ftable.m_fnSetup = &Duckvil::Process::linux_setup;
    _ftable.m_fnStart = &Duckvil::Process::linux_start;
    _ftable.m_fnStop = &Duckvil::Process::linux_stop;
    _ftable.m_fnWait = &Duckvil::Process::linux_wait;
    _ftable.m_fnWrite = &Duckvil::Process::linux_write;
    _ftable.m_fnTerminate = &Duckvil::Process::linux_terminate;
    _ftable.m_fnCleanup = &Duckvil::Process::linux_cleanup;
#endif
#endif

    return &_ftable;
}