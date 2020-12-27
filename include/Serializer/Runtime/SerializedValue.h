#pragma once

#include "Serializer/Runtime/ISerializedValue.h"

#include <cstddef>
#include <memory>

namespace Duckvil { namespace RuntimeSerializer {

    template <typename Type>
    struct SerializedValue : public ISerializedValue
    {
        SerializedValue(const Type& _value) :
            m_value(_value)
        {

        }

        Type m_value;
    };

    template <typename Type, std::size_t N>
    struct SerializedArray : public ISerializedValue
    {
        SerializedArray(const Type(&_value)[N])
        {
            memcpy(m_value, _value, sizeof(m_value));
        }

        Type m_value[N];
    };

}}