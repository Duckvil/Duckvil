#pragma once

namespace Duckvil { namespace CSharp { namespace Event {

    struct MonoObjectCreatedEvent
    {
        void* m_pObject;
        void* m_pClass;
    };

}}}