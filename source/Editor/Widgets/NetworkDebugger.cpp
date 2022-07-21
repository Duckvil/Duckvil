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

    }

    bool NetworkDebugger::OnMessage(const Duckvil::Network::Message& _message)
    {
        Duckvil::Network::Message _msg = _message;

        if(_message.m_header.m_id.m_ullTypeID == typeid(Duckvil::Network::CommonCommands).hash_code())
        {
            if(_message.m_header.m_id.m_ullValue == Duckvil::Network::CommonCommands::Client_Accepted)
            {
                std::cout << "Client was accepted!\n";

                Duckvil::Network::Message _message(Duckvil::Network::CommonCommands::Client_RegisterWithServer);

                m_pClient->Send(_message);

                return true;
            }
            else if(_message.m_header.m_id.m_ullValue == Duckvil::Network::CommonCommands::Client_AssignID)
            {
                uint32_t _clientID = -1;

                _msg >> _clientID;

                std::cout << "Assigned client ID: " << _clientID << "\n";

                return true;
            }
        }
        else if(_message.m_header.m_id.m_ullTypeID == typeid(Duckvil::Network::BasicCommands).hash_code())
        {
            if(_message.m_header.m_id.m_ullValue == Duckvil::Network::BasicCommands::Ping)
            {
                std::chrono::system_clock::time_point _timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point _timeThen;

                _msg >> _timeThen;

                std::cout << "Ping: " << std::chrono::duration<double>(_timeNow - _timeThen).count() << "\n";

                return true;
            }
        }

        return false;
    }

}}