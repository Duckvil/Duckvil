#pragma once

#include "Serializer/Runtime/SerializedValue.h"

#include <cstdint>

namespace Duckvil { namespace RuntimeSerializer {

    struct ISerializer
    {
        virtual ~ISerializer()
        {

        }

        virtual void Clear() = 0;
        virtual void Clear(uint32_t _uiID) = 0;
        virtual void Clear(uint32_t _uiID, const char* _sPropertyName) = 0;

        virtual bool IsLoading() const = 0;

        template <typename Type>
        bool SerializeProperty(const char* _sPropertyName, Type& _value)
        {
            if(IsLoading())
            {
                const SerializedValue<Type>* _serializedValue = (const SerializedValue<Type>*)GetSerializedValue(_sPropertyName);

                if(_serializedValue == nullptr)
                {
                    return false;
                }

                _value = _serializedValue->m_value;
            }
            else
            {
                const SerializedValue<Type>* _serializedValue = new SerializedValue<Type>(_value);

                SetSerializedValue(_sPropertyName, _serializedValue);
            }

            return true;
        }

        template <typename Type, std::size_t N>
        bool SerializeProperty(const char* _sPropertyName, const Type (&_value)[N])
        {
            if(IsLoading())
            {
                const SerializedArray<Type, N>* _serializedValue = (const SerializedArray<Type, N>*)GetSerializedValue(_sPropertyName);

                if(_serializedValue == nullptr)
                {
                    return false;
                }

                memcpy(_value, _serializedValue->m_value, sizeof(_value));
            }
            else
            {
                const SerializedArray<Type, N>* _serializedValue = new SerializedArray<Type, N>(_value);

                SetSerializedValue(_sPropertyName, _serializedValue);
            }

            return true;
        }
    
    protected:
        virtual void SetSerializedValue(const char* _sPropertyName, const ISerializedValue* _pValue) = 0;
        virtual const ISerializedValue* GetSerializedValue(const char* _sPropertyName) const = 0;
    };

}}