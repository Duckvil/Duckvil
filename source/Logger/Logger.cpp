#include "Logger/Logger.h"

namespace Duckvil {

    logger_data logger_init(const Memory::FreeList& _heap)
    {
        logger_data _data;

        RuntimeReflection::ReflectedType<__logger_channel_data> _loggerType(_heap);

        static Event::Pool<Event::mode::immediate> _eventPool(_heap, RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData);
        static Memory::Vector<__logger_channel_data*> _loggers;

        _heap.Allocate(_loggers, 1);

        RuntimeReflection::record_meta(_heap.GetMemoryInterface(), _heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, _loggerType.GetTypeHandle(), "EventPool", _eventPool);
        RuntimeReflection::record_meta(_heap.GetMemoryInterface(), _heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflectionData, _loggerType.GetTypeHandle(), "Loggers", _loggers);

        _heap.Allocate(_data.m_aChannels, 1);

        return _data;
    }

    __logger_channel_data* logger_add(const Memory::FreeList& _heap, logger_data* _pData, const logger_channel_descriptor& _channel, std::size_t _ullTypeID, uint32_t _uiChannel)
    {
        if(_pData->m_aChannels.Full())
        {
            _pData->m_aChannels.Resize(_pData->m_aChannels.Size() * 2);
        }

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _loggerModule("Logger");

        _module.load(&_loggerModule);

        duckvil_logger_channel_init_callback _loggerInit;

        _module.get(_loggerModule, "duckvil_logger_channel_init", (void**)&_loggerInit);

        __logger_channel_ftable* m_pLogger = _loggerInit(_heap.GetMemoryInterface(), _heap.GetAllocator());
        __logger_channel_data* m_pLoggerData = m_pLogger->init(_heap.GetMemoryInterface(), _heap.GetAllocator());

        memcpy(m_pLoggerData->m_sPathFile, _channel.m_sPathFile, _channel.m_ullLength);
        m_pLoggerData->m_flags = _channel.m_flags;

        _pData->m_aChannels.Allocate({ m_pLoggerData, _ullTypeID, _uiChannel });

        return m_pLoggerData;
    }

    void logger_log(__logger_channel_ftable* _pLoggerChannel, logger_data* _pData, std::size_t _ullTypeID, uint32_t _uiChannel, uint32_t _uiLine, const char* _sFile, std::size_t _ullFileLength, const char* _sMessage, std::size_t _ullMessageLength, __logger_channel_verbosity _verbosity, const va_list& _vMessage)
    {
        for(const logger_channel_lookup& _channelLookup : _pData->m_aChannels)
        {
            if(_channelLookup.m_ullTypeID == _ullTypeID && (uint32_t)_channelLookup.m_uiValue == _uiChannel)
            {
                logger_log_v(_pLoggerChannel, _channelLookup.m_pChannel, _uiLine, _sFile, _ullFileLength, _sMessage, _ullMessageLength, _verbosity, _vMessage);
            }
        }
    }

}

Duckvil::logger_ftable duckvil_logger_init(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator)
{
    Duckvil::logger_ftable _ftable;

    _ftable.m_fnInitLogger = &Duckvil::logger_init;
    _ftable.m_fnAdd = Duckvil::logger_add;
    _ftable.m_fnLog = Duckvil::logger_log;

    return _ftable;
}