#include "Logger/Logger.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Duckvil { namespace Logger {

    __data* init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
    {
        __data* _data = (__data*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(__data), alignof(__data));

        _data->m_llInitTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        _data->m_logs = Memory::Queue<__log_info>(_pMemoryInterface, _pAllocator, 64);

        memset(_data->m_buffer, 0, 128);

        return _data;
    }

    void log(__ftable* _pFTable, __data* _pData, __log_info& _logInfo)
    {
        _logInfo.m_time = std::time(nullptr);

        if(_logInfo._flags & __flags::__flags_immediate_log)
        {
            _pFTable->format(_pData, _logInfo, _pData->m_buffer);

            printf("%s\n", _pData->m_buffer);
        }
        else
        {
            _pData->m_logs.Allocate(_logInfo);
        }
    }

    void format(__data* _pData, const __log_info& _logInfo, char* _ppBuffer)
    {
        long long _upTime = std::chrono::high_resolution_clock::now().time_since_epoch().count() - _pData->m_llInitTime;

    // TODO: I dont understand the whole 'chrono' lib...

        switch(_logInfo.m_verbosity)
        {
        case __verbosity::__verbosity_info:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ INFO }: %s", std::ctime(&_logInfo.m_time), _upTime * 0.000000001f, _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_warning:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ WARN }: %s", std::ctime(&_logInfo.m_time), _upTime * 0.000000001f, _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_error:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ ERROR }: %s", std::ctime(&_logInfo.m_time), _upTime * 0.000000001f, _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        case __verbosity::__verbosity_fatal:
            sprintf(_ppBuffer, "%s %f [%s:%u]{ FATAL }: %s", std::ctime(&_logInfo.m_time), _upTime * 0.000000001f, _logInfo.m_sFile, _logInfo.m_uiLine, _logInfo.m_sMessage);
            break;
        }
    }

    void dispatch_logs(__ftable* _pFTable, __data* _pData)
    {
        while(!_pData->m_logs.Empty())
        {
            const __log_info& _log = _pData->m_logs.Begin();
            uint32_t _color = 0;

            _pFTable->format(_pData, _log, _pData->m_buffer);

            switch(_log.m_verbosity)
            {
            case __verbosity::__verbosity_info:
                _color = 15;
                break;
            case __verbosity::__verbosity_warning:
                _color = 14;
                break;
            case __verbosity::__verbosity_error:
                _color = 12;
                break;
            case __verbosity::__verbosity_fatal:
                _color = 12 * 16;
                break;
            default:
                break;
            }

#ifdef DUCKVIL_PLATFORM_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _color);
            printf("%s\n", _pData->m_buffer);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
#ifdef DUCKVIL_PLATFORM_LINUX
            printf("%s\n", _pData->m_buffer);
#endif
#endif

            _pData->m_logs.Pop();
        }
    }

}}

Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Logger::__ftable* _ftable = (Duckvil::Logger::__ftable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Logger::__ftable), alignof(Duckvil::Logger::__ftable));

    _ftable->init = &Duckvil::Logger::init;
    _ftable->log = &Duckvil::Logger::log;
    _ftable->format = &Duckvil::Logger::format;
    _ftable->dispatch_logs = &Duckvil::Logger::dispatch_logs;

    return _ftable;
}