#pragma once

// #include "Memory/Vector.h"
#include <vector>
#include <memory>

namespace Duckvil { namespace Network {

    struct MessageType
    {
        size_t m_ullTypeID;
        size_t m_ullValue;

        MessageType()
        {
            m_ullTypeID = -1;
            m_ullValue = -1;
        }

        template <typename T>
        MessageType(const T& _command)
        {
            m_ullTypeID = typeid(T).hash_code();
            m_ullValue = static_cast<size_t>(_command);
        }
    };

    struct MessageHeader
    {
        MessageType m_id;
        size_t m_ullSize;

        MessageHeader()
        {
            m_ullSize = 0;
        }

        template <typename T>
        MessageHeader(const T& _command) :
            m_id(_command)
        {
            m_ullSize = 0;
        }
    };

    struct Message
    {
        MessageHeader m_header;
        // Memory::Vector<uint8_t> m_aBody;
        std::vector<uint8_t> m_aBody;

        Message()
        {

        }

        template <typename T>
        Message(const T& _command) :
            m_header(_command)
        {

        }
    };

    template <typename DataType>
    Message& operator << (Message& _message, const DataType& _data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t _s = _message.m_aBody.size();

        // _message.m_aBody.Resize(_s + sizeof(DataType));
        _message.m_aBody.resize(_s + sizeof(DataType));

        std::memcpy(_message.m_aBody.data() + _s, &_data, sizeof(DataType));

        _message.m_header.m_ullSize = _message.m_aBody.size();

        return _message;
    }

    template <typename DataType>
    Message& operator >> (Message& _message, DataType& _data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t _s = _message.m_aBody.size() - sizeof(DataType);

        std::memcpy(&_data, _message.m_aBody.data() + _s, sizeof(DataType));

        // _message.m_aBody.Resize(_s);
        _message.m_aBody.resize(_s);

        _message.m_header.m_ullSize = _message.m_aBody.size();

        return _message;
    }

    struct IConnection;

    struct OwnedMessage
    {
        // void* m_pRemote;
        std::shared_ptr<IConnection> m_pRemote = nullptr;
        Message m_message;
    };

}}