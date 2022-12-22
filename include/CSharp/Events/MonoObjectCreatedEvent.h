#pragma once

#include "mono/jit/jit.h"

namespace Duckvil { namespace CSharp { namespace Event {

    struct MonoObjectCreatedEvent
    {
        MonoObject* m_pObject;
        MonoClass* m_pClass;
    };

}}}