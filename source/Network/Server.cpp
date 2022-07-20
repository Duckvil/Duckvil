#include "Network/Server.h"

#include <iostream>

#include "Network/Connection.h"

namespace Duckvil { namespace Network {

    Server::Server(const Memory::FreeList& _heap, uint16_t _u16Port) :
        m_heap(_heap),
        m_asioAcceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _u16Port))
    {

    }

    Server::~Server()
    {
        Stop();
    }

    bool Server::Start()
    {
        try
        {
            WaitForConnection();

            m_thread = std::thread([this](){ m_context.run(); });
        }
        catch(std::exception& _e)
        {
            std::cerr << "Server exception: " << _e.what() << "\n";

            return false;
        }

        std::cout << "Server started!\n";

        return true;
    }

    void Server::Stop()
    {
        m_asioAcceptor.close();
        m_context.stop();

        if(m_thread.joinable())
        {
            m_thread.join();
        }

        std::cout << "Server stopped!\n";
    }

    void Server::Update(size_t _ullMaxMessages, bool _bWait)
    {
        size_t _messageCount = 0;

        while(_messageCount < _ullMaxMessages && !m_qMessagesIn.empty())
        {
            auto _message = m_qMessagesIn.pop_front();

            OnMessage(_message.m_pRemote, _message.m_message);

            _messageCount++;
        }
    }

    void Server::WaitForConnection()
    {
        m_asioAcceptor.async_accept(
            [this](std::error_code _ec, asio::ip::tcp::socket _socket)
            {
                if(!_ec)
                {
                    std::cout << "New connection: " << _socket.remote_endpoint() << "\n";

                    std::shared_ptr<IConnection> _newConnection = std::make_shared<Connection>(Connection::Owner::SERVER, m_context, std::move(_socket), m_qMessagesIn);

                    if(OnClientConnect(_newConnection))
                    {
                        m_aConnections.push_back(std::move(_newConnection));

                        m_aConnections.back()->ConnectToClient(this, m_uiIDs++);

                        std::cout << "Connection approved!\n";
                    }
                    else
                    {
                        std::cout << "Connection denied!\n";
                    }
                }
                else
                {
                    std::cout << "New connection error: " << _ec.message() << "\n";
                }

                WaitForConnection();
            }
        );
    }

    void Server::MessageClient(std::shared_ptr<IConnection> _pClient, const Message& _message)
    {
        if(_pClient && _pClient->IsConnected())
        {
            _pClient->Send(_message);
        }
        else
        {
            OnClientDisconnect(_pClient);

            _pClient.reset();

            m_aConnections.erase(
                std::remove(
                    m_aConnections.begin(),
                    m_aConnections.end(),
                    _pClient
                ),
                m_aConnections.end()
            );
        }
    }

    void Server::MessageAllClients(const Message& _message, std::shared_ptr<IConnection> _pIgnoreClient)
    {
        bool _invalidClientExists = false;

        for(auto& _client : m_aConnections)
        {
            if(_client && _client->IsConnected())
            {
                if(_client != _pIgnoreClient)
                {
                    _client->Send(_message);
                }
            }
            else
            {
                OnClientDisconnect(_client);

                _client.reset();

                _invalidClientExists = true;
            }
        }

        if(_invalidClientExists)
        {
            m_aConnections.erase(
                std::remove(
                    m_aConnections.begin(),
                    m_aConnections.end(),
                    nullptr
                ),
                m_aConnections.end()
            );
        }
    }

    bool Server::OnClientConnect(std::shared_ptr<IConnection> _pClient)
    {
        return true;
    }

    void Server::OnClientDisconnect(std::shared_ptr<IConnection> _pClient)
    {

    }

    void Server::OnMessage(std::shared_ptr<IConnection> _pClient, Message& _message)
    {
        const auto& _id = _message.m_header.m_id;

        if(_id.m_ullTypeID == typeid(BasicCommands).hash_code())
        {
            if(_id.m_ullValue == BasicCommands::Ping)
            {
                std::cout << "[" << _pClient->GetID() << "] Ping!\n";

                _pClient->Send(_message);
            }
        }
        else if(_id.m_ullTypeID == typeid(CommonCommands).hash_code())
        {
            if(_id.m_ullValue == CommonCommands::Client_RegisterWithServer)
            {
                Message _message(CommonCommands::Client_AssignID);

                _message << _pClient->GetID();

                MessageClient(_pClient, _message);
            }
        }
    }

    void Server::OnClientValidated(std::shared_ptr<IConnection> _pClient)
    {
        Message _message;

        _message.m_header.m_id.m_ullTypeID = typeid(CommonCommands).hash_code();
        _message.m_header.m_id.m_ullValue = CommonCommands::Client_Accepted;

        _pClient->Send(_message);
    }

}}