#pragma once

#include "Memory/Memory.h"
#include "Memory/Queue.h"

#include "Utils/Utils.h"

#define DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX 32
#define DUCKVIL_LOGGER_PATH_LENGTH_MAX 256

#define DUCKVIL_LOG_INFO(message) _pData->m_pLogger->log_info(_pData->m_pLoggerData, Logger::__log_info(__LINE__, __FILE__, message, Logger::__verbosity::__verbosity_info))

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
        __flags_log_to_console = 1 << 1
    };

    struct __log_info
    {
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
    };

    struct __data
    {
        Memory::Queue<__log_info> m_logs;
    };

    struct __ftable
    {
        __data* (*init)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);
        void (*log_info)(__data* _pData, const __log_info& _logInfo);
        void (*dispatch_logs)(__data* _pData);
    };

}}

typedef Duckvil::Logger::__ftable* (*duckvil_logger_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);

DUCKVIL_EXPORT Duckvil::Logger::__ftable* duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator);