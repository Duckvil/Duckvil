#include "Network/Client.h"

#include <iostream>
#include <stack>

#include "Network/Connection.h"

namespace Duckvil { namespace Network {

    Client::Client(const Memory::FreeList& _heap) :
        m_heap(_heap),
        m_socket(m_context)
    {

    }

    Client::~Client()
    {
        Disconnect();
    }

    bool Client::Connect(const std::string& _sHost, const uint16_t _u16Port)
    {
        try
        {
            asio::ip::tcp::resolver _r(m_context);
            asio::ip::tcp::resolver::results_type _endpoints = _r.resolve(_sHost, std::to_string(_u16Port));

            m_pConnection = new Connection(
                IConnection::Owner::CLIENT,
                m_context,
                asio::ip::tcp::socket(m_context),
                m_qMessagesIn
            );

            (static_cast<Connection*>(m_pConnection))->ConnectToServer(_endpoints);

            m_thread = std::thread([this](){ m_context.run(); });
        }
        catch(std::exception& _e)
        {
            std::cerr << "Client exception: " << _e.what() << "\n";

            return false;
        }

        return true;
    }

    void Client::Disconnect()
    {
        if(IsConnected())
        {
            m_pConnection->Disconnect();
        }

        m_context.stop();

        if(m_thread.joinable())
        {
            m_thread.join();
        }

        delete m_pConnection;
    }

    bool Client::IsConnected() const
    {
        if(m_pConnection)
        {
            return m_pConnection->IsConnected();
        }
        else
        {
            return false;
        }
    }

    void Client::Update()
    {
        if(!IsConnected())
        {
            return;
        }

        std::stack<OwnedMessage> _notHandledMessages;

        while(!m_qMessagesIn.empty())
        {
            auto _serverMessage = m_qMessagesIn.pop_front();
            bool _wasHandled = false;

            for(NetworkSystem* _system : m_aSystems)
            {
                if(_system->OnMessage(_serverMessage.m_message, nullptr))
                {
                    _wasHandled = true;
                }
            }

            if(!_wasHandled)
            {
                _notHandledMessages.push(_serverMessage);
            }
        }

        while(!_notHandledMessages.empty())
        {
            m_qMessagesIn.push_back(_notHandledMessages.top());

            _notHandledMessages.pop();
        }
    }

    void Client::AddSystem(NetworkSystem* _pSystem)
    {
        _pSystem->SetOwner(IConnection::Owner::CLIENT);

        m_aSystems.push_back(_pSystem);
    }

    tsqueue<OwnedMessage>& Client::Incoming()
    {
        return m_qMessagesIn;
    }

    void Client::Send(const Message& _message) const
    {
        if(IsConnected())
        {
            m_pConnection->Send(_message);
        }
    }

    uint32_t Client::GetID() const
    {
        return m_pConnection->GetID();
    }

}}