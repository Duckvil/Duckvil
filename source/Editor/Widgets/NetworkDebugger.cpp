#include "Editor/Widgets/NetworkDebugger.h"

#include "imgui/imgui.h"

#include <chrono>
#include <iostream>

namespace Duckvil { namespace Editor {

    NetworkDebugger::NetworkDebugger(const Memory::FreeList& _heap, Network::IClient* _pClient, Network::IServer* _pServer) :
        m_pServer(_pServer),
        m_pClient(_pClient),
        m_heap(_heap)
    {

    }

    NetworkDebugger::~NetworkDebugger()
    {

    }

    bool NetworkDebugger::Init()
    {
        return true;
    }

    void NetworkDebugger::Update(double _dDelta)
    {

    }

    void NetworkDebugger::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));
    }

    void NetworkDebugger::OnDraw()
    {
        ImGui::Begin("Network");

        if(m_pClient && m_pClient->IsConnected())
        {
            if(ImGui::Button("Ping"))
            {
                Network::Message _message(Network::BasicCommands::Ping);

                _message << std::chrono::system_clock::now();

                m_pClient->Send(_message);
            }
        }

        ImGui::End();
    }

    void NetworkDebugger::SetOwner(Network::IConnection::Owner _owner)
    {
        m_owner = _owner;
    }

    bool NetworkDebugger::OnMessage(const Duckvil::Network::Message& _message, std::shared_ptr<Duckvil::Network::IConnection> _pClient)
    {
        Duckvil::Network::Message _msg = _message;

        if(_message == Duckvil::Network::CommonCommands::Client_Accepted)
        {
            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                std::cout << "Client was accepted!\n";

                Duckvil::Network::Message _message(Duckvil::Network::CommonCommands::Client_RegisterWithServer);

                m_pClient->Send(_message);

                return true;
            }
        }
        else if(_message == Duckvil::Network::CommonCommands::Client_AssignID)
        {
            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                uint32_t _clientID = -1;

                _msg >> _clientID;

                std::cout << "Assigned client ID: " << _clientID << "\n";

                return true;
            }
        }
        else if(_message == Duckvil::Network::CommonCommands::Client_RegisterWithServer)
        {
            if(m_owner == Network::IConnection::Owner::SERVER)
            {
                Network::Message _message(Network::CommonCommands::Client_AssignID);

                _message << _pClient->GetID();

                m_pServer->MessageClient(_pClient, _message);

                return true;
            }
        }
        else if(_message == Duckvil::Network::BasicCommands::Ping)
        {
            if(m_owner == Network::IConnection::Owner::CLIENT)
            {
                std::chrono::system_clock::time_point _timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point _timeThen;

                _msg >> _timeThen;

                std::cout << "Ping: " << std::chrono::duration<double>(_timeNow - _timeThen).count() << "\n";

                return true;
            }
            else
            {
                std::cout << "[" << _pClient->GetID() << "] Ping!\n";

                _pClient->Send(_message);

                return true;
            }
        }

        return false;
    }

}}