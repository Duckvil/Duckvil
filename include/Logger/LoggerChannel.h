#pragma once

#include "Memory/Memory.h"
#include "Memory/Queue.h"
#include "Memory/Vector.h"

#include "Utils/Utils.h"

#include <chrono>
#include <ctime>
#include <cstdarg>
#include <cassert>

#include "Event/ImmediatePool.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/ReflectedType.h"

#define DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX 256
#define DUCKVIL_LOGGER_PATH_LENGTH_MAX 256
#define DUCKVIL_LOGGER_OUT_FILE_PATH_LENGTH_MAX 128
#define DUCKVIL_LOGGER_BUFFER_MAX 256

namespace Duckvil {

    enum __logger_channel_verbosity : uint8_t
    {
        __verbosity_info,
        __verbosity_warning,
        __verbosity_error,
        __verbosity_fatal
    };

    enum __logger_channel_log_flags : uint8_t
    {
        __flags_immediate_log = 1 << 0
    };

    enum __logger_channel_flags : uint8_t
    {
        __logger_flags_file_output = 1 << 0,
        __logger_flags_console_output = 1 << 1,
        __logger_flags_editor_console_output = 1 << 2
    };

    struct __logger_channel_data;

    struct __logger_channel_log_info
    {
        __logger_channel_log_info()
        {

        }

        template <std::size_t Length, std::size_t Length2>
        __logger_channel_log_info(uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __logger_channel_verbosity _verbosity) :
            m_uiLine(_uiLine),
            m_verbosity(_verbosity)
        {
            if(Length < DUCKVIL_LOGGER_PATH_LENGTH_MAX)
            {
                memcpy(m_sFile, _sFile, Length);
            }
            else
            {
                assert(false && "Path is too long!");
            }

            if(Length2 < DUCKVIL_LOGGER_PATH_LENGTH_MAX)
            {
                memcpy(m_sMessage, _sMessage, Length2);
            }
            else
            {
                assert(false && "Message is too long!");
            }

            _flags = {};
        }

        uint32_t m_uiLine;
        char m_sFile[DUCKVIL_LOGGER_PATH_LENGTH_MAX];
        char m_sMessage[DUCKVIL_LOGGER_MESSAGE_LENGTH_MAX];
        __logger_channel_verbosity m_verbosity;
        __logger_channel_log_flags _flags;
        std::time_t m_time;
        __logger_channel_data* m_pOwner;
    };

    struct __logger_channel_data
    {
        Memory::Queue<__logger_channel_log_info> m_logs;
        int64_t m_llInitTime;
        char m_buffer[DUCKVIL_LOGGER_BUFFER_MAX];
        char m_sPathFile[DUCKVIL_LOGGER_OUT_FILE_PATH_LENGTH_MAX];
        time_t m_lastTime;
        __logger_channel_flags m_flags;
        // Memory::Vector<custom_log_t> m_aCustomLogs;
        Event::Pool<Event::mode::immediate>* m_pLogEventPool;
    };

    struct __logger_channel_ftable
    {
        __logger_channel_data* (*init)(Memory::IMemory* _pMemoryInterface, Memory::free_list_allocator* _pAllocator);
        void (*log)(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData, __logger_channel_log_info& _logInfo);
        void (*format)(__logger_channel_data* _pData, const __logger_channel_log_info& _logInfo, char* _ppBuffer);
        void (*dispatch_logs)(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData);
    };

    static void logger_log_v(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData, uint32_t _uiLine, const char* _sFile, std::size_t _ullFileLength, const char* _sMessage, std::size_t _ullMessageLength, __logger_channel_verbosity _verbosity, va_list _vMessage)
    {
        __logger_channel_log_info _log;

        vsprintf(_log.m_sMessage, _sMessage, _vMessage);

        _log.m_verbosity = _verbosity;
        _log.m_uiLine = _uiLine;
        _log._flags = {};

        if(_ullFileLength < DUCKVIL_LOGGER_PATH_LENGTH_MAX)
        {
            strcpy(_log.m_sFile, _sFile);
        }
        else
        {
            assert(false && "File path is too long!");
        }

        _pFTable->log(_pFTable, _pData, _log);
    }

    template <std::size_t Length, std::size_t Length2>
    static void logger_log_v(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData, uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __logger_channel_verbosity _verbosity, va_list _vMessage)
    {
        logger_log_v(_pFTable, _pData, _uiLine, _sFile, Length, _sMessage, Length2, _verbosity, _vMessage);
    }

    template <std::size_t Length, std::size_t Length2>
    static void logger_log(__logger_channel_ftable* _pFTable, __logger_channel_data* _pData, uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __logger_channel_verbosity _verbosity, ...)
    {
        va_list _argList;
        va_start(_argList, _verbosity);

        logger_log_v<Length, Length2>(_pFTable, _pData, _uiLine, _sFile, _sMessage, _verbosity, _argList);

        va_end(_argList);
    }

}

typedef Duckvil::__logger_channel_ftable* (*duckvil_logger_channel_init_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator);

namespace Duckvil {

    class LoggerChannel
    {
    private:
        __logger_channel_ftable* m_pLogger;
        __logger_channel_data* m_pLoggerData;

    public:
        LoggerChannel()
        {
            m_pLogger = nullptr;
            m_pLoggerData = nullptr;
        }

        LoggerChannel(const Memory::FreeList& _heap, const char* _sPath, std::size_t _ullPathLength, __logger_channel_flags _flags)
        {
            PlugNPlay::__module _module;

            PlugNPlay::module_init(&_module);

            PlugNPlay::__module_information _loggerModule("Logger");

            _module.load(&_loggerModule);

            duckvil_logger_channel_init_callback _loggerInit;

            _module.get(_loggerModule, "duckvil_logger_channel_init", (void**)&_loggerInit);

            RuntimeReflection::ReflectedType<__logger_channel_data> _loggerType(_heap);

            static Event::Pool<Event::mode::immediate> _eventPool(_heap, RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData);
            static Memory::Vector<__logger_channel_data*> _loggers;

            _heap.Allocate(_loggers, 1);

            RuntimeReflection::record_meta(_heap.GetMemoryInterface(), _heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, _loggerType.GetTypeHandle(), "EventPool", _eventPool);
            RuntimeReflection::record_meta(_heap.GetMemoryInterface(), _heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, _loggerType.GetTypeHandle(), "Loggers", _loggers);

            m_pLogger = _loggerInit(_heap.GetMemoryInterface(), _heap.GetAllocator());
            m_pLoggerData = m_pLogger->init(_heap.GetMemoryInterface(), _heap.GetAllocator());

            memcpy(m_pLoggerData->m_sPathFile, _sPath, _ullPathLength);
            m_pLoggerData->m_flags = _flags;
        }

        template <std::size_t Length>
        LoggerChannel(const Memory::FreeList& _heap, const char (&_sPath)[Length], __logger_channel_flags _flags) :
            LoggerChannel(_heap, _sPath, Length, _flags)
        {

        }

        ~LoggerChannel()
        {

        }

        void Dispatch()
        {
            m_pLogger->dispatch_logs(m_pLogger, m_pLoggerData);
        }

        inline __logger_channel_ftable* GetLogger() const
        {
            return m_pLogger;
        }

        inline __logger_channel_data* GetLoggerData() const
        {
            return m_pLoggerData;
        }
    };

}

DUCKVIL_EXPORT Duckvil::__logger_channel_ftable* duckvil_logger_channel_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator);