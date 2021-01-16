#pragma once

#include "Serializer/Runtime/ISerializer.h"

#include <map>

#include "HotReloader/HotObject.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace RuntimeSerializer {

    class Serializer : public ISerializer
    {
    public:
        typedef std::map<const char*, const ISerializedValue*> ValueGroup;

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

        }

        void Serialize(void* _pHotObject, RuntimeReflection::__function<void(RuntimeCompiler::HotObject::*)(RuntimeSerializer::ISerializer*)>* _fnSerialize)
        {
            _fnSerialize->Invoke(_pHotObject, this);
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