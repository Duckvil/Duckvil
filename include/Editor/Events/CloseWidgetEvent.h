#pragma once

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Editor {

    struct CloseWidgetEvent
    {
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
        void* m_pObject;

        CloseWidgetEvent(const RuntimeReflection::__duckvil_resource_type_t& _typeHandle, void* _pObject) :
            m_typeHandle(_typeHandle),
            m_pObject(_pObject)
        {

        }

        template <typename Type>
        CloseWidgetEvent(Type* _pObject) :
            m_pObject(_pObject)
        {
            m_typeHandle = RuntimeReflection::get_type<Type>();
        }
    };

}}