#pragma once

#include <memory>

namespace Duckvil { namespace Network {

    enum BasicCommands
    {
        Ping
    };

    enum CommonCommands
    {
        Client_Accepted,
        Client_AssignID,
        Client_RegisterWithServer
    };

    struct IConnection;

    struct IServer
    {
        virtual ~IServer()
        {

        }

        virtual bool Start() = 0;
        virtual void Stop() = 0;
        virtual void Update(size_t _ullMaxMessages = -1, bool _bWait = false) = 0;

        virtual void OnClientValidated(std::shared_ptr<IConnection> _pClient) = 0;
    };

}}