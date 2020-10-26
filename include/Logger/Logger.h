#pragma once

#include "Memory/Memory.h"
#include "Memory/Queue.h"

#include "Utils/Utils.h"

#include <chrono>
#include <ctime>
#include <cstdarg>

#define DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX 32
#define DUCKVIL_LOGGER_PATH_LENGTH_MAX 256

#define DUCKVIL_LOG_INFO(message, ...) Logger::log(_pData->m_pLogger, _pData->m_pLoggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_info, __VA_ARGS__)// _pData->m_pLogger->log(_pData->m_pLogger, _pData->m_pLoggerData, Logger::__log_info(__LINE__, __FILE__, message, Logger::__verbosity::__verbosity_info))
#define DUCKVIL_LOG_WARNING(message, ...) _pData->m_pLogger->log(_pData->m_pLogger, _pData->m_pLoggerData, Logger::__log_info(__LINE__, __FILE__, message, Logger::__verbosity::__verbosity_warning))

namespace Duckvil { namespace Logger {

    enum __verbosity
    {
        __verbosity_info,
        __verbosity_warning,
        __verbosity_error,
        __verbosity_fatal
    };

    enum __flags : uint8_t
    {
        __flags_dump_to_file = 1 << 0,
        __flags_log_to_console = 1 << 1,
        __flags_immediate_log = 1 << 2
    };

    struct __log_info
    {
        __log_info()
        {

        }

        template <std::size_t Length, std::size_t Length2>
        __log_info(uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __verbosity _verbosity) :
            m_uiLine(_uiLine),
            m_verbosity(_verbosity)
        {
            memcpy(m_sFile, _sFile, Length);
            memcpy(m_sMessage, _sMessage, Length2);

            _flags = {};
        }

        uint32_t m_uiLine;
        char m_sFile[DUCKVIL_LOGGER_PATH_LENGTH_MAX];
        char m_sMessage[DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX];
        __verbosity m_verbosity;
        __flags _flags;
        std::time_t m_time;
    };

    struct __data
    {
        Memory::Queue<__log_info> m_logs;
        // std::chrono::time_point<std::chrono::system_clock> m_initTime;
        long long m_llInitTime;
        char m_buffer[128];
    };

    struct __ftable
    {
        __data* (*init)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);
        void (*log)(__ftable* _pFTable, __data* _pData, __log_info& _logInfo);
        void (*format)(__data* _pData, const __log_info& _logInfo, char* _ppBuffer);
        void (*dispatch_logs)(__ftable* _pFTable, __data* _pData);
    };

    template <std::size_t Length, std::size_t Length2>
    static void log(__ftable* _pFTable, __data* _pData, uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __verbosity _verbosity, ...)
    {
        va_list _argList;
        __log_info _log;
        va_start(_argList, _verbosity);

        _vsprintf_l(_log.m_sMessage, _sMessage, nullptr, _argList);

        _log.m_verbosity = _verbosity;
        _log.m_uiLine = _uiLine;
        strcpy(_log.m_sFile, _sFile);

        _pFTable->log(_pFTable, _pData, _log);

        va_end(_argList);
    }

}}

typedef Duckvil::Logger::__ftable* (*duckvil_logger_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);