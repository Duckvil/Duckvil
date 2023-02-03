#pragma once

#include "Network/IClient.h"
#include "Network/Message.h"
#include "Network/IConnection.h"
#include "Network/NetworkSystem.h"

#include <sdkddkver.h>
#include "asio.hpp"

#include "Memory/FreeList.h"

#include <thread>

namespace Duckvil { namespace Network {

    class Client : public IClient
    {
    private:
        asio::io_context m_context;
        asio::ip::tcp::socket m_socket;

        tsqueue<OwnedMessage> m_qMessagesIn;
        Memory::FreeList m_heap;

        std::thread m_thread;

        IConnection* m_pConnection;

        std::vector<NetworkSystem*> m_aSystems;

        inline IConnection* GetConnection() const override
        {
            return m_pConnection;
        }

    public:
        Client(const Memory::FreeList& _heap);
        ~Client();

        bool Connect(const std::string& _sHost, const uint16_t _u16Port) override;
        void Disconnect() override;
        bool IsConnected() const override;

        void Update() override;

        inline void AddSystem(NetworkSystem* _pSystem) override;

        inline tsqueue<OwnedMessage>& Incoming() override;

        void Send(const Message& _message) const override;

        inline uint32_t GetID() const override;
    };

}}