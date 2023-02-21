#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

#include "Utils/RuntimeDependencyInjector.h"

namespace Duckvil {

    struct InjectConstructorArgumentEvent
    {
        struct Info
        {
            RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
            RuntimeReflection::__duckvil_resource_constructor_t m_constructorHandle;
            uint32_t m_uiArgumentIndex;
        };

        bool m_bSuccess;
        DependencyInjection::IDependencyInjector* m_pFAP;
        InjectConstructorArgumentEvent::Info m_info;
        RuntimeReflection::__argument_t m_argument;

        InjectConstructorArgumentEvent(DependencyInjection::IDependencyInjector* _pFAP, const InjectConstructorArgumentEvent::Info& _info, const RuntimeReflection::__argument_t& _argument) :
            m_bSuccess(false),
            m_pFAP(_pFAP),
            m_info(_info),
            m_argument(_argument)
        {

        }
    };

}