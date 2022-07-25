#pragma once

#include <string>

#include "Network/Message.h"
#include "Network/NetworkSystem.h"

#include "Network/TSQueue.h"

namespace Duckvil { namespace Network {

DUCKVIL_RUNTIME_REFLECTION_PAUSE

    struct IConnection;

DUCKVIL_RUNTIME_REFLECTION_RESUME

    struct IClient
    {
    protected:
        void SetID(uint32_t _uiID)
        {
            GetConnection()->SetID(_uiID);
        }

        virtual IConnection* GetConnection() const = 0;

    public:
        virtual ~IClient()
        {

        }

        virtual bool Connect(const std::string& _sHost, const uint16_t _u16Port) = 0;
        virtual void Disconnect() = 0;
        virtual bool IsConnected() const = 0;

        virtual void Update() = 0;

        virtual void AddSystem(NetworkSystem* _pSystem) = 0;

        virtual tsqueue<OwnedMessage>& Incoming() = 0;

        virtual void Send(const Message& _message) const = 0;

        virtual uint32_t GetID() const = 0;
    };

}}