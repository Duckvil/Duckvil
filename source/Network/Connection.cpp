#include "Network/Connection.h"

#include <iostream>

namespace Duckvil { namespace Network {

    void Connection::ReadHeader()
    {
        asio::async_read(m_socket, asio::buffer(&m_tmpMessageIn.m_header, sizeof(MessageHeader)),
            [this](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    if(m_tmpMessageIn.m_header.m_ullSize > 0)
                    {
                        m_tmpMessageIn.m_aBody.resize(m_tmpMessageIn.m_header.m_ullSize);

                        ReadBody();
                    }
                    else
                    {
                        AddToIncomingMessageQueue();
                    }
                }
                else
                {
                    std::cout << "[" << m_uiID << "] Read header error: " << _ec.message() << "\n";

                    m_socket.close();
                }
            }
        );
    }

    void Connection::ReadBody()
    {
        asio::async_read(m_socket, asio::buffer(m_tmpMessageIn.m_aBody.data(), m_tmpMessageIn.m_aBody.size()),
            [this](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    AddToIncomingMessageQueue();
                }
                else
                {
                    std::cout << "[" << m_uiID << "] Read body error: " << _ec.message() << "\n";

                    m_socket.close();
                }
            }
        );
    }

    void Connection::WriteHeader()
    {
        asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().m_header, sizeof(MessageHeader)),
            [this](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    if(m_qMessagesOut.front().m_aBody.size() > 0)
                    {
                        WriteBody();
                    }
                    else
                    {
                        m_qMessagesOut.pop_front();

                        if(!m_qMessagesOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    std::cout << "[" << m_uiID << "] Write header error: " << _ec.message() << "\n";

                    m_socket.close();
                }
            }
        );
    }

    void Connection::WriteBody()
    {
        asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().m_aBody.data(), m_qMessagesOut.front().m_aBody.size()),
            [this](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    m_qMessagesOut.pop_front();

                    if(!m_qMessagesOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    std::cout << "[" << m_uiID << "] Write body error: " << _ec.message() << "\n";

                    m_socket.close();
                }
            }
        );
    }

    void Connection::ReadValidation(IServer* _pServer)
    {
        asio::async_read(m_socket, asio::buffer(&m_ullHandshakeIn, sizeof(size_t)),
            [this, _pServer](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    if(m_owner == IConnection::Owner::SERVER)
                    {
                        if(m_ullHandshakeIn == m_ullHandshakeCheck)
                        {
                            std::cout << "Client validated\n";

                            _pServer->OnClientValidated(this->shared_from_this());

                            ReadHeader();
                        }
                        else
                        {
                            m_socket.close();
                        }
                    }
                    else
                    {
                        m_ullHandshakeOut = Scramble(m_ullHandshakeIn);

                        WriteValidation();
                    }
                }
                else
                {
                    std::cout << "Client disconnected (ReadValidation)\n";

                    m_socket.close();
                }
            }
        );
    }

    void Connection::WriteValidation()
    {
        asio::async_write(m_socket, asio::buffer(&m_ullHandshakeOut, sizeof(size_t)),
            [this](std::error_code _ec, std::size_t _len)
            {
                if(!_ec)
                {
                    if(m_owner == IConnection::Owner::CLIENT)
                    {
                        ReadHeader();
                    }
                }
                else
                {
                    m_socket.close();
                }
            }
        );
    }

    void Connection::AddToIncomingMessageQueue()
    {
        OwnedMessage _m;

        _m.m_message = m_tmpMessageIn;

        if(m_owner == IConnection::Owner::SERVER)
        {
            _m.m_pRemote = this->shared_from_this();

            m_qMessagesIn.push_back(_m);
        }
        else
        {
            _m.m_pRemote = nullptr;

            m_qMessagesIn.push_back(_m);
        }

        ReadHeader();
    }

    uint64_t Connection::Scramble(uint64_t _ullData)
    {
        uint64_t _out = _ullData ^ 0xDEADBEEFC0DECAFE;

        _out = (_out & 0xF0F0F0F0F0F0F0F0) >> 4 | (_out & 0x0F0F0F0F0F0F0F0F) << 4;

        return _out ^ 0xC0DEFACE12345678;
    }

    void Connection::SetID(uint32_t _uiID)
    {
        m_uiID = _uiID;
    }

    Connection::Connection(IConnection::Owner _owner, asio::io_context& _context, asio::ip::tcp::socket _socket, tsqueue<OwnedMessage>& _qIn) :
        m_context(_context),
        m_socket(std::move(_socket)),
        m_qMessagesIn(_qIn)
    {
        m_owner = _owner;

        if(m_owner == IConnection::Owner::SERVER)
        {
            m_ullHandshakeOut = size_t(std::chrono::system_clock::now().time_since_epoch().count());
            m_ullHandshakeCheck = Scramble(m_ullHandshakeOut);
            m_ullHandshakeIn = 0;
        }
        else
        {
            m_ullHandshakeIn = 0;
            m_ullHandshakeOut = 0;
        }
    }

    Connection::~Connection()
    {

    }

    void Connection::ConnectToServer(asio::ip::tcp::resolver::results_type& _endpoints)
    {
        if(m_owner == IConnection::Owner::CLIENT)
        {
            asio::async_connect(m_socket, _endpoints,
                [this](std::error_code _ec, asio::ip::tcp::endpoint _endpoint)
                {
                    if(!_ec)
                    {
                        // ReadHeader();
                        ReadValidation();
                    }
                    else
                    {
                        std::cout << "Error while connecting: " << _ec.message() << "\n";
                    }
                }
            );
        }
    }

    void Connection::ConnectToClient(IServer* _pServer, uint32_t _uiID)
    {
        if(m_owner == IConnection::Owner::SERVER)
        {
            if(m_socket.is_open())
            {
                m_uiID = _uiID;

                // ReadHeader();

                WriteValidation();
                ReadValidation(_pServer);
            }
        }
    }

    void Connection::Disconnect()
    {
        if(IsConnected())
        {
            asio::post(m_context,
                [this]()
                {
                    m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
                    m_socket.close();
                }
            );
        }
    }

    bool Connection::IsConnected() const
    {
        return m_socket.is_open();
    }

    void Connection::Send(const Message& _message)
    {
        asio::post(m_context,
            [this, _message]()
            {
                bool _writingMessage = !m_qMessagesOut.empty();

                m_qMessagesOut.push_back(_message);

                if(!_writingMessage)
                {
                    WriteHeader();
                }
            }
        );
    }

    uint32_t Connection::GetID() const
    {
        return m_uiID;
    }

}}