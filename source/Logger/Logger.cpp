#include "Logger/Logger.h"

namespace Duckvil { namespace Logger {

    __data* init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_initTime = std::chrono::high_resolution_clock::now();
        _data->m_logs = Memory::Queue<__log_info>(_pMemoryInterface, _pAllocator, 64);

        memset(_data->m_buffer, 0, 128);

        return _data;
    }

    void log_info(__data* _pData, __log_info& _logInfo)
    {
        _logInfo.m_time = std::time(nullptr);

        if(_logInfo._flags & __flags::__flags_immediate_log)
        {
            // Dispatch now
        }
        else
        {
            _pData->m_logs.Allocate(_logInfo);
        }
    }

    void format(__data* _pData, const __log_info& _logInfo, char* _ppBuffer)
    {
        std::chrono::duration<float> _upTime = std::chrono::high_resolution_clock::now() - _pData->m_initTime;

        switch(_logInfo.m_verbosity)
        {
        case __verbosity::__verbosity_info:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ INFO }: %s", std::ctime(&_logInfo.m_time), _upTime.count(), _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_warning:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ WARNING }: %s", std::ctime(&_logInfo.m_time), _upTime.count(), _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_error:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ ERROR }: %s", std::ctime(&_logInfo.m_time), _upTime.count(), _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_fatal:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ FATAL }: %s", std::ctime(&_logInfo.m_time), _upTime.count(), _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        }
    }

    void dispatch_logs(__ftable* _pFTable, __data* _pData)
    {
        while(!_pData->m_logs.Empty())
        {
            const __log_info& _log = _pData->m_logs.Begin();

            _pFTable->format(_pData, _log, _pData->m_buffer);

            printf("%s\n", _pData->m_buffer);

            _pData->m_logs.Pop();
        }
    }

}}

Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Logger::__ftable* _ftable = (Duckvil::Logger::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Logger::__ftable), alignof(Duckvil::Logger::__ftable));

    _ftable->init = &Duckvil::Logger::init;
    _ftable->log_info = &Duckvil::Logger::log_info;
    _ftable->format = &Duckvil::Logger::format;
    _ftable->dispatch_logs = &Duckvil::Logger::dispatch_logs;

    return _ftable;
}