#pragma once

#include "Editor/Widget.h"

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "Network/IClient.h"
#include "Network/IServer.h"

#include "Editor/Widgets/NetworkDebugger.generated.h"

namespace Duckvil { namespace Editor {

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_UserSystem)
    class NetworkDebugger : public Widget, public ISystem
    {
        DUCKVIL_GENERATED_BODY
    private:
        Network::IClient* m_pClient;
        Network::IServer* m_pServer;

        Memory::FreeList m_heap;

    public:
        NetworkDebugger(const Memory::FreeList& _heap, Network::IClient* _pClient, Network::IServer* _pServer);
        ~NetworkDebugger();

        bool Init();
        void Update(double _dDelta);

        void InitEditor(void* _pImguiContext);
        void OnDraw();
    };

}}