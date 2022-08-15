#pragma once

#include "Network/IServer.h"
#include "Network/IConnection.h"
#include "Network/Message.h"
#include "Network/TSQueue.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include "asio.hpp"

#include <thread>

namespace Duckvil { namespace Network {

    class Server : public IServer
    {
    private:
        asio::io_context m_context;
        asio::ip::tcp::acceptor m_asioAcceptor;

        std::thread m_thread;

        tsqueue<OwnedMessage> m_qMessagesIn;
        Memory::FreeList m_heap;

        uint32_t m_uiIDs;

        // Memory::Vector<IConnection*> m_aConnections;
        std::deque<std::shared_ptr<IConnection>> m_aConnections;
        std::vector<NetworkSystem*> m_aSystems;

    public:
        Server(const Memory::FreeList& _heap, uint16_t _u16Port);
        ~Server();

        bool Start() override;
        void Stop() override;
        void Update(size_t _ullMaxMessages = -1, bool _bWait = false) override;

        void WaitForConnection();
        void MessageClient(std::shared_ptr<IConnection> _pClient, const Message& _message) override;
        void MessageAllClients(const Message& _message, std::shared_ptr<IConnection> _pIgnoreClient = nullptr) override;

// Problem with forward declaration and stripping type modifiers
DUCKVIL_RUNTIME_REFLECTION_PAUSE
        inline void AddSystem(NetworkSystem* _pSystem) override;
DUCKVIL_RUNTIME_REFLECTION_RESUME

        bool OnClientConnect(std::shared_ptr<IConnection> _pClient);
        void OnClientDisconnect(std::shared_ptr<IConnection> _pClient);
        void OnMessage(std::shared_ptr<IConnection> _pClient, Message& _message);

        void OnClientValidated(std::shared_ptr<IConnection> _pClient) override;
    };

}}