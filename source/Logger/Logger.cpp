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

    void logger_add(logger_data* _pData, const LoggerChannel& _channel, std::size_t _ullTypeID, uint32_t _uiChannel)
    {
        if(_pData->m_aChannels.Full())
        {
            _pData->m_aChannels.Resize(_pData->m_aChannels.Size() * 2);
        }

        logger_channel_lookup _lookup;

        _lookup.m_channel = _channel;
        _lookup.m_uiValue = _uiChannel;
        _lookup.m_ullTypeID = _ullTypeID;

        _pData->m_aChannels.Allocate(_lookup);
    }

    void logger_log(logger_data* _pData, std::size_t _ullTypeID, uint32_t _uiChannel, uint32_t _uiLine, const char* _sFile, std::size_t _ullFileLength, const char* _sMessage, std::size_t _ullMessageLength, __logger_channel_verbosity _verbosity, const va_list& _vMessage)
    {
        for(const logger_channel_lookup& _channelLookup : _pData->m_aChannels)
        {
            if(_channelLookup.m_ullTypeID == _ullTypeID && (uint32_t)_channelLookup.m_uiValue == _uiChannel)
            {
                logger_log_v(_channelLookup.m_channel.GetLogger(), _channelLookup.m_channel.GetLoggerData(), _uiLine, _sFile, _ullFileLength, _sMessage, _ullMessageLength, _verbosity, _vMessage);
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