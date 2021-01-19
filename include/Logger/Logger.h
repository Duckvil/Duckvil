#pragma once

#include "Memory/Memory.h"
#include "Memory/Queue.h"

#include "Utils/Utils.h"

#include <chrono>
#include <ctime>
#include <cstdarg>

#define DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX 256
#define DUCKVIL_LOGGER_PATH_LENGTH_MAX 256
#define DUCKVIL_LOGGER_OUT_FILE_PATH_LENGTH_MAX 128
#define DUCKVIL_LOGGER_BUFFER_MAX 256

#ifdef DUCKVIL_PLATFORM_WINDOWS
#define DUCKVIL_LOG_INFO_(logger, loggerData, message, ...) Logger::log(logger, loggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_info, __VA_ARGS__)
#define DUCKVIL_LOG_WARNING_(logger, loggerData, message, ...) Logger::log(logger, loggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_warning, __VA_ARGS__)
#define DUCKVIL_LOG_ERROR_(logger, loggerData, message, ...) Logger::log(logger, loggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_error, __VA_ARGS__)
#define DUCKVIL_LOG_FATAL_(logger, loggerData, message, ...) Logger::log(logger, loggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_fatal, __VA_ARGS__)

#define DUCKVIL_LOG_INFO(message, ...) DUCKVIL_LOG_INFO_(_pData->m_pLogger, _pData->m_pLoggerData, message, __VA_ARGS__)
#define DUCKVIL_LOG_WARNING(message, ...) DUCKVIL_LOG_WARNING_(_pData->m_pLogger, _pData->m_pLoggerData, message, __VA_ARGS__)
#define DUCKVIL_LOG_ERROR(message, ...) DUCKVIL_LOG_ERROR_(_pData->m_pLogger, _pData->m_pLoggerData, message, __VA_ARGS__)
#define DUCKVIL_LOG_FATAL(message, ...) DUCKVIL_LOG_FATAL_(_pData->m_pLogger, _pData->m_pLoggerData, message, __VA_ARGS__)
#else
#ifdef DUCKVIL_PLATFORM_LINUX
#define DUCKVIL_LOG_INFO(message, ...) Logger::log(_pData->m_pLogger, _pData->m_pLoggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_info, ## __VA_ARGS__)
#define DUCKVIL_LOG_WARNING(message, ...) Logger::log(_pData->m_pLogger, _pData->m_pLoggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_warning, ## __VA_ARGS__)
#define DUCKVIL_LOG_ERROR(message, ...) Logger::log(_pData->m_pLogger, _pData->m_pLoggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_error, ## __VA_ARGS__)
#define DUCKVIL_LOG_FATAL(message, ...) Logger::log(_pData->m_pLogger, _pData->m_pLoggerData, __LINE__, __FILE__, message, Logger::__verbosity::__verbosity_fatal, ## __VA_ARGS__)
#endif
#endif

namespace Duckvil { namespace Logger {

    enum __verbosity
    {
        __verbosity_info,
        __verbosity_warning,
        __verbosity_error,
        __verbosity_fatal
    };

    enum __log_flags : uint8_t
    {
        __flags_immediate_log = 1 << 0
    };

    enum __logger_flags : uint8_t
    {
        __logger_flags_file_output = 1 << 0,
        __logger_flags_console_output = 1 << 1
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
        __log_flags _flags;
        std::time_t m_time;
    };

    struct __data
    {
        Memory::Queue<__log_info> m_logs;
        int64_t m_llInitTime;
        char m_buffer[DUCKVIL_LOGGER_BUFFER_MAX];
        char m_sPathFile[DUCKVIL_LOGGER_OUT_FILE_PATH_LENGTH_MAX];
        time_t m_lastTime;
        __logger_flags m_flags;
    };

    struct __ftable
    {
        Logger::__data* (*init)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator);
        void (*log)(Logger::__ftable* _pFTable, Logger::__data* _pData, __log_info& _logInfo);
        void (*format)(Logger::__data* _pData, const __log_info& _logInfo, char* _ppBuffer);
        void (*dispatch_logs)(Logger::__ftable* _pFTable, Logger::__data* _pData);
    };

    template <std::size_t Length, std::size_t Length2>
    static void log(__ftable* _pFTable, __data* _pData, uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __verbosity _verbosity, ...)
    {
        va_list _argList;
        __log_info _log;
        va_start(_argList, _verbosity);

        vsprintf(_log.m_sMessage, _sMessage, _argList);

        _log.m_verbosity = _verbosity;
        _log.m_uiLine = _uiLine;
        _log._flags = {};
        strcpy(_log.m_sFile, _sFile);

        _pFTable->log(_pFTable, _pData, _log);

        va_end(_argList);
    }

}}

typedef Duckvil::Logger::__ftable* (*duckvil_logger_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);