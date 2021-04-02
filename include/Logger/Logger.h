#pragma once

#include "Logger/LoggerChannel.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#define DUCKVIL_LOG_INFO(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_info, __VA_ARGS__)
#define DUCKVIL_LOG_WARNING(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_warning, __VA_ARGS__)
#define DUCKVIL_LOG_ERROR(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_error, __VA_ARGS__)
#define DUCKVIL_LOG_FATAL(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_fatal, __VA_ARGS__)
#else
#define DUCKVIL_LOG_INFO(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_info, ## __VA_ARGS__)
#define DUCKVIL_LOG_WARNING(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_warning, ## __VA_ARGS__)
#define DUCKVIL_LOG_ERROR(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_error, ## __VA_ARGS__)
#define DUCKVIL_LOG_FATAL(channel, message, ...) logger_log(channel, __LINE__, __FILE__, message, __logger_channel_verbosity::__verbosity_fatal, ## __VA_ARGS__)
#endif

namespace Duckvil {

    enum LoggerChannelID
    {
        Default
    };

    struct logger_channel_lookup
    {
        LoggerChannel m_channel;
        std::size_t m_ullTypeID; // Enum type
        uint32_t m_uiValue; // Enum value
    };

    struct logger_data
    {
        Memory::Vector<logger_channel_lookup> m_aChannels;
    };

    struct logger_ftable
    {
        logger_data (*m_fnInitLogger)(const Memory::FreeList& _heap);
        void (*m_fnAdd)(logger_data* _pData, LoggerChannel _channel, std::size_t _ullTypeID, uint32_t _uiChannel);
        void (*m_fnLog)(logger_data* _pData, std::size_t _ullTypeID, uint32_t _uiChannel, uint32_t _uiLine, const char* _sFile, std::size_t _ullFileLength, const char* _sMessage, std::size_t _ullMessageLength, __logger_channel_verbosity _verbosity, const va_list& _vMessage);
    };

    struct logger_context
    {
        logger_ftable m_logger;
        logger_data m_loggerData;
        bool m_bInitialized = false;
    };

    inline logger_context g_loggerContext;

    static void logger_make_current(const logger_context& _context)
    {
        if(g_loggerContext.m_bInitialized)
        {
            return;
        }

        g_loggerContext.m_logger = _context.m_logger;
        g_loggerContext.m_loggerData = _context.m_loggerData;
        g_loggerContext.m_bInitialized = true;
    }

    static const logger_context& logger_get_current()
    {
        return g_loggerContext;
    }

    template <typename Type>
    void logger_add(LoggerChannel _channel, const Type& _channelID)
    {
        g_loggerContext.m_logger.m_fnAdd(&g_loggerContext.m_loggerData, _channel, typeid(Type).hash_code(), (uint32_t)_channelID);
    }

    template <typename Type, std::size_t Length, std::size_t Length2>
    void logger_log(const Type& _channelID, uint32_t _uiLine, const char (&_sFile)[Length], const char (&_sMessage)[Length2], __logger_channel_verbosity _verbosity, ...)
    {
        va_list _argList;
        va_start(_argList, _verbosity);

        g_loggerContext.m_logger.m_fnLog(&g_loggerContext.m_loggerData, typeid(Type).hash_code(), (uint32_t)_channelID, _uiLine, _sFile, Length, _sMessage, Length2, _verbosity, _argList);

        va_end(_argList);
    }

}

DUCKVIL_EXPORT Duckvil::logger_ftable duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator);