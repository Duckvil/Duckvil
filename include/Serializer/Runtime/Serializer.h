#pragma once

#include "Serializer/Runtime/ISerializer.h"

#include <map>

#include "HotReloader/HotObject.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Utils/FunctionArgumentsPusher.h"

namespace Duckvil { namespace RuntimeSerializer {

    class Serializer : public ISerializer
    {
    public:
        struct cmp_str
        {
            bool operator()(char const* a, char const* b) const
            {
                return std::strcmp(a, b) < 0;
            }
        };

        typedef std::map<const char*, const ISerializedValue*, cmp_str> ValueGroup;

    private:
        ValueGroup m_serializationMap;

        bool m_bIsLoading;

        void SetSerializedValue(const char* _sPropertyName, const ISerializedValue* _pValue)
        {
            m_serializationMap[_sPropertyName] = _pValue;
        }

        const ISerializedValue* GetSerializedValue(const char* _sPropertyName) const
        {
            return m_serializationMap.find(_sPropertyName)->second;
        }

    public:
        Serializer()
        {

        }

        ~Serializer()
        {
            for(ValueGroup::iterator it = m_serializationMap.begin(); it != m_serializationMap.end(); ++it)
            {
                delete it->second;
            }
        }

        void Serialize(void* _pHotObject, RuntimeReflection::__function<void(HotReloader::HotObject::*)(RuntimeSerializer::ISerializer*)>* _fnSerialize)
        {
            _fnSerialize->Invoke(_pHotObject, this);
        }

        void Serialize(void* _pHotObject, void* _pSerializeFunction)
        {
            FunctionArgumentsPusher _c(2);

            _c.Push(_pHotObject);
            _c.Push(this);
            _c.Call(_pSerializeFunction);

            _c.getCode<void(*)()>()();
        }

        template <typename Type>
        void Serialize(Type* _pHotObject, void (Type::*_fnSerialize)(RuntimeSerializer::ISerializer*))
        {
            Serialize(_pHotObject, (void*&)_fnSerialize);
        }

        template <typename Type>
        void Serialize(void* _pHotObject, void (Type::*_fnSerialize)(RuntimeSerializer::ISerializer*))
        {
            Serialize(_pHotObject, (void*&)_fnSerialize);
        }

        void Clear()
        {

        }

        void Clear(uint32_t _uiID)
        {

        }

        void Clear(uint32_t _uiID, const char* _sPropertyName)
        {

        }

        bool IsLoading() const
        {
            return m_bIsLoading;
        }

        void SetLoading(bool _bLoading)
        {
            m_bIsLoading = _bLoading;
        }
    };

}}