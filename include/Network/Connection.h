#pragma once

#include "Network/IConnection.h"
#include "Network/TSQueue.h"
#include "Network/IServer.h"

#include <sdkddkver.h>

#include "asio.hpp"

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

namespace Duckvil { namespace Network {

    class Connection : public IConnection, public std::enable_shared_from_this<Connection>
    {
    private:
        asio::ip::tcp::socket m_socket;
        asio::io_context& m_context;

        tsqueue<Message> m_qMessagesOut;
        tsqueue<OwnedMessage>& m_qMessagesIn;

        IConnection::Owner m_owner;

        uint32_t m_uiID;

        Message m_tmpMessageIn;

        size_t m_ullHandshakeOut;
        size_t m_ullHandshakeIn;
        size_t m_ullHandshakeCheck;

        void ReadHeader();
        void ReadBody();

        void WriteHeader();
        void WriteBody();

        void ReadValidation(IServer* _pServer = nullptr);
        void WriteValidation();

        void AddToIncomingMessageQueue();

        uint64_t Scramble(uint64_t _ullData);

        inline void SetID(uint32_t _uiID) override;

    public:
DUCKVIL_RUNTIME_REFLECTION_PAUSE
        Connection(IConnection::Owner _owner, asio::io_context& _context, asio::ip::tcp::socket _socket, tsqueue<OwnedMessage>& _qIn);
DUCKVIL_RUNTIME_REFLECTION_RESUME
        ~Connection();

        void ConnectToServer(asio::ip::tcp::resolver::results_type& _endpoints);

        void ConnectToClient(IServer* _pServer, uint32_t _uiID) override;
        void Disconnect() override;
        bool IsConnected() const override;

        void Send(const Message& _message) override;

        inline uint32_t GetID() const override;
    };

}}