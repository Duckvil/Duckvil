#pragma once

namespace Duckvil { namespace DependencyInjection {

    template<typename T>
    struct Tag
    {
        using type = T;
    };

    struct Inject
    {
    	size_t m_ullTypeID;

        template <typename Type>
        Inject(Tag<Type>)
        {
            m_ullTypeID = typeid(Type).hash_code();
        }
    };

}}