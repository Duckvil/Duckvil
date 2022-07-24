#pragma once

#include "Editor/Widget.h"

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "Network/IClient.h"
#include "Network/IServer.h"
#include "Network/NetworkSystem.h"

#include "Editor/Widgets/NetworkDebugger.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_UserSystem)
    class NetworkDebugger : public Widget, public ISystem, public Network::NetworkSystem
    {
        DUCKVIL_GENERATED_BODY
    private:
        Network::IClient* m_pClient;
        Network::IServer* m_pServer;

        Memory::FreeList m_heap;

        Network::IConnection::Owner m_owner;

    public:
        NetworkDebugger(const Memory::FreeList& _heap, Network::IClient* _pClient, Network::IServer* _pServer);
        ~NetworkDebugger();

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();

        void SetOwner(Network::IConnection::Owner _owner) override;
        bool OnMessage(const Duckvil::Network::Message& _message, std::shared_ptr<Duckvil::Network::IConnection> _pClient) override;
    };

}}