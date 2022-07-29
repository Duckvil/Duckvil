#include "Logger/LoggerChannel.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <fstream>

#include "RuntimeReflection/Meta.h"

namespace Duckvil {

    __logger_channel_data* init(const Memory::FreeList& _heap)
    {
        __logger_channel_data* _data = _heap.Allocate<__logger_channel_data>();

        _data->m_llInitTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        _data->m_flags = {};

        // _heap.Allocate(_data->m_logs, 1);
        memset(_data->m_buffer, 0, 128);

        _data->m_logs = Memory::ThreadsafeQueue<__logger_channel_log_info>(_heap);

        // _data->m_aCustomLogs = Memory::Vector<custom_log_t>(_pMemoryInterface, _pAllocator, 1);

        // DUCKVIL_DEBUG_MEMORY(_data->m_logs.GetAllocator(), "m_logs");

        _data->m_pLogEventPool = (Event::Pool<Event::mode::immediate>*)RuntimeReflection::get_meta_value_ptr(RuntimeReflection::get_type<__logger_channel_data>(), "EventPool");

        auto lol = (Memory::Vector<__logger_channel_data*>*)RuntimeReflection::get_meta_value_ptr(RuntimeReflection::get_type<__logger_channel_data>(), "Loggers");

        lol->Allocate(_data);

        return _data;
    }

    void log(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData, const __logger_channel_log_info& _logInfo2)
    {
        __logger_channel_log_info _logInfo = _logInfo2;
        _logInfo.m_time = std::time(nullptr);
        _logInfo.m_pOwner = _pData;

        if(_pData->m_flags & __logger_channel_flags::__logger_flags_console_output)
        {
            if(_logInfo.m_flags & __logger_channel_log_flags::__flags_immediate_log)
            {
                _pFTable->format(_pData, _logInfo, _pData->m_buffer, DUCKVIL_LOGGER_BUFFER_MAX);

                printf("%s\n", _pData->m_buffer);
            }
            else
            {
                if(_pData->m_logs.Full())
                {
                    _pData->m_logs.Resize(_pData->m_logs.GetSize() * 2);
                }

                _pData->m_logs.Allocate(_logInfo);
            }
        }
        else if(_pData->m_flags & __logger_channel_flags::__logger_flags_file_output)
        {
            if(_logInfo.m_flags & __logger_channel_log_flags::__flags_immediate_log)
            {
                // TODO: Print some error?
            }
            else
            {
                if(_pData->m_logs.Full())
                {
                    _pData->m_logs.Resize(_pData->m_logs.GetSize() * 2);
                }

                _pData->m_logs.Allocate(_logInfo);
            }
        }
    }

    void format(__logger_channel_data* _pData, const __logger_channel_log_info& _logInfo, char* _ppBuffer, const size_t _uiBufferSize)
    {
        long long _upTime = std::chrono::high_resolution_clock::now().time_since_epoch().count() - _pData->m_llInitTime;
        const time_t& _time = _logInfo.m_time;

    // TODO: I dont understand the whole 'chrono' lib...

        if(_pData->m_lastTime != _time)
        {
            char _timeStr[26];

#ifdef DUCKVIL_PLATFORM_WINDOWS
            ctime_s(_timeStr, sizeof(_timeStr), &_time);
#else
#ifdef DUCKVIL_PLATFORM_LINUX
            ctime_r(&_time, _timeStr);
#endif
#endif

            switch(_logInfo.m_verbosity)
            {
            case __logger_channel_verbosity::__verbosity_info:
                snprintf(_ppBuffer, _uiBufferSize, "%s %f{ INFO }: %s", _timeStr, _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_warning:
                snprintf(_ppBuffer, _uiBufferSize, "%s %f{ WARN }: %s", _timeStr, _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_error:
                snprintf(_ppBuffer, _uiBufferSize, "%s %f{ ERROR }: %s", _timeStr, _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_fatal:
                snprintf(_ppBuffer, _uiBufferSize, "%s %f{ FATAL }: %s", _timeStr, _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            }

            _pData->m_lastTime = _time;
        }
        else
        {
            switch(_logInfo.m_verbosity)
            {
            case __logger_channel_verbosity::__verbosity_info:
                snprintf(_ppBuffer, _uiBufferSize, " %f{ INFO }: %s", _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_warning:
                snprintf(_ppBuffer, _uiBufferSize, " %f{ WARN }: %s", _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_error:
                snprintf(_ppBuffer, _uiBufferSize, " %f{ ERROR }: %s", _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            case __logger_channel_verbosity::__verbosity_fatal:
                snprintf(_ppBuffer, _uiBufferSize, " %f{ FATAL }: %s", _upTime * 0.000000001, _logInfo.m_sMessage);
                break;
            }
        }
    }

    void dispatch_logs(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData)
    {
        std::ofstream _logOutFile;

        if(_pData->m_flags & __logger_channel_flags::__logger_flags_file_output)
        {
            _logOutFile.open(_pData->m_sPathFile, std::ios::app);
        }

        while(!_pData->m_logs.Empty())
        {
            const __logger_channel_log_info& _log = _pData->m_logs.Begin();

            _pFTable->format(_pData, _log, _pData->m_buffer, DUCKVIL_LOGGER_BUFFER_MAX);

            if(_pData->m_flags & __logger_channel_flags::__logger_flags_console_output)
            {
#ifdef DUCKVIL_PLATFORM_WINDOWS
                uint32_t _color = 0;

                switch(_log.m_verbosity)
                {
                case __logger_channel_verbosity::__verbosity_info:
                    _color = 15;
                    break;
                case __logger_channel_verbosity::__verbosity_warning:
                    _color = 14;
                    break;
                case __logger_channel_verbosity::__verbosity_error:
                    _color = 12;
                    break;
                case __logger_channel_verbosity::__verbosity_fatal:
                    _color = 12 * 16;
                    break;
                default:
                    break;
                }

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _color);
                printf("%s\n", _pData->m_buffer);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
#ifdef DUCKVIL_PLATFORM_LINUX
                switch(_log.m_verbosity)
                {
                case __logger_channel_verbosity::__verbosity_info:
                    printf("\033[97m%s\033[0m\n", _pData->m_buffer);
                    break;
                case __logger_channel_verbosity::__verbosity_warning:
                    printf("\033[93m%s\033[0m\n", _pData->m_buffer);
                    break;
                case __logger_channel_verbosity::__verbosity_error:
                    printf("\033[31m%s\033[0m\n", _pData->m_buffer);
                    break;
                case __logger_channel_verbosity::__verbosity_fatal:
                    printf("\033[41m%s\033[0m\n", _pData->m_buffer);
                    break;
                default:
                    break;
                }
#endif
#endif
            }

            if(_pData->m_flags & __logger_channel_flags::__logger_flags_file_output)
            {
                _logOutFile << _pData->m_buffer << "\n";
            }

            // for(uint32_t i = 0; i < _pData->m_aCustomLogs.Size(); i++)
            // {
            //     const custom_log_t& _fnLog = _pData->m_aCustomLogs[i];

            //     _fnLog(_log);
            // }

            _pData->m_pLogEventPool->Broadcast(_log);

            _pData->m_logs.Pop();
        }

        if(_pData->m_flags & __logger_channel_flags::__logger_flags_file_output)
        {
            _logOutFile.close();
        }
    }

}

Duckvil::__logger_channel_ftable* duckvil_logger_channel_init()
{
    static Duckvil::__logger_channel_ftable _ftable = { 0 };

    _ftable.init = &Duckvil::init;
    _ftable.log = &Duckvil::log;
    _ftable.format = &Duckvil::format;
    _ftable.dispatch_logs = &Duckvil::dispatch_logs;

    return &_ftable;
}