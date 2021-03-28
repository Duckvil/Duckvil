#include "Logger/Logger.h"

namespace Duckvil {

    logger_data logger_init(const Memory::FreeList& _heap)
    {
        logger_data _data;

        _heap.Allocate(_data.m_aChannels, 1);

        return _data;
    }

    void logger_add(logger_data* _pData, LoggerChannel _channel, std::size_t _ullTypeID, uint32_t _uiChannel)
    {
        if(_pData->m_aChannels.Full())
        {
            _pData->m_aChannels.Resize(_pData->m_aChannels.Size() * 2);
        }

        _pData->m_aChannels.Allocate({ _channel, _ullTypeID, _uiChannel });
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

Duckvil::logger_ftable duckvil_logger_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::logger_ftable _ftable;

    _ftable.m_fnInitLogger = &Duckvil::logger_init;
    _ftable.m_fnAdd = Duckvil::logger_add;
    _ftable.m_fnLog = Duckvil::logger_log;

    return _ftable;
}