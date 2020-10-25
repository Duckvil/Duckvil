#include "Logger/Logger.h"

namespace Duckvil { namespace Logger {

    __data* init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_logs = Memory::Queue<__log_info>(_pMemoryInterface, _pAllocator, 64);

        return _data;
    }

    void log_info(__data* _pData, const __log_info& _logInfo)
    {
        _pData->m_logs.Allocate(_logInfo);
    }

    void dispatch_logs(__data* _pData)
    {
        while(!_pData->m_logs.Empty())
        {
            const __log_info& _log = _pData->m_logs.Begin();

            printf("[%s:%u]: %s\n", _log.m_sFile, _log.m_uiLine, _log.m_sMessage);

            _pData->m_logs.Pop();
        }
    }

}}

Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Logger::__ftable* _ftable = (Duckvil::Logger::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Logger::__ftable), alignof(Duckvil::Logger::__ftable));

    _ftable->init = &Duckvil::Logger::init;
    _ftable->log_info = &Duckvil::Logger::log_info;
    _ftable->dispatch_logs = &Duckvil::Logger::dispatch_logs;

    return _ftable;
}