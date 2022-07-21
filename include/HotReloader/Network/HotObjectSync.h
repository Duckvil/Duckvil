#pragma once

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "HotReloader/Events/SwapEvent.h"

#include "Memory/FreeList.h"

#include "Network/IServer.h"
#include "Network/IClient.h"
#include "Network/NetworkSystem.h"

#include "HotReloader/RuntimeCompilerSystem.h"

#include "HotReloader/Network/HotObjectSync.generated.h"

namespace Duckvil { namespace HotReloader { namespace Network {

    DUCKVIL_CLASS(ReflectionFlags_AutoEventsAdding)
    class HotObjectSync : public ISystem, public Duckvil::Network::NetworkSystem
    {
        DUCKVIL_GENERATED_BODY
    public:
        enum NetworkCommands
        {
            SendHotObject
        };

    private:
        Duckvil::Network::IClient* m_pClient;
        Duckvil::Network::IServer* m_pServer;

        HotReloader::RuntimeCompilerSystem* m_pRCS;

    public:
        HotObjectSync(const Memory::FreeList& _heap, Duckvil::Network::IClient* _pClient, Duckvil::Network::IServer* _pServer, HotReloader::RuntimeCompilerSystem* _pRCS);
        ~HotObjectSync();

        bool Init();
        void Update(double _dDelta);

        void OnEvent(const HotReloader::SwapEvent& _event);

        void SetOwner(Duckvil::Network::IConnection::Owner _owner) override;

        bool OnMessage(const Duckvil::Network::Message& _message) override;
    };

}}}