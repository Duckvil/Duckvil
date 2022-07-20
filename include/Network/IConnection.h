#pragma once

#include <string>

#include "Network/Message.h"
#include "Network/IServer.h"

namespace Duckvil { namespace Network {

    struct IConnection
    {
        enum class Owner
        {
            SERVER,
            CLIENT
        };

        virtual ~IConnection()
        {

        }

        // virtual bool ConnectToServer(const std::string& _sHost, const uint16_t _u16Port) = 0;
        virtual void ConnectToClient(IServer* _pServer, uint32_t _uiID) = 0;
        virtual void Disconnect() = 0;
        virtual bool IsConnected() const = 0;

        virtual void Send(const Message& _message) = 0;

        virtual uint32_t GetID() const = 0;
    };

}}