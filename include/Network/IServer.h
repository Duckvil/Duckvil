#pragma once

#include <memory>

#include "Network/Message.h"

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

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

DUCKVIL_RUNTIME_REFLECTION_PAUSE

    struct IConnection;
    struct NetworkSystem;

DUCKVIL_RUNTIME_REFLECTION_RESUME

    struct IServer
    {
        virtual ~IServer()
        {

        }

        virtual bool Start() = 0;
        virtual void Stop() = 0;
        virtual void Update(size_t _ullMaxMessages = -1, bool _bWait = false) = 0;

// Problem with forward declaration and stripping type modifiers
DUCKVIL_RUNTIME_REFLECTION_PAUSE
        virtual void AddSystem(NetworkSystem* _pSystem) = 0;
DUCKVIL_RUNTIME_REFLECTION_RESUME

        virtual void MessageClient(std::shared_ptr<IConnection> _pClient, const Message& _message) = 0;
        virtual void MessageAllClients(const Message& _message, std::shared_ptr<IConnection> _pIgnoreClient = nullptr) = 0;

        virtual void OnClientValidated(std::shared_ptr<IConnection> _pClient) = 0;
    };

}}