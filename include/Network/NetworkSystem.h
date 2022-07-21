#pragma once

#include "Network/Message.h"
#include "Network/IConnection.h"

namespace Duckvil { namespace Network {

    struct NetworkSystem
    {
        virtual void SetOwner(IConnection::Owner _owner) = 0;

        virtual bool OnMessage(const Duckvil::Network::Message& _message)
        {
            return false;
        }
    };

}}