#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData, const std::size_t& _ullTypeID);

    template <typename Type>
    DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData)
    {
        static const std::size_t& _typeID = typeid(Type).hash_code();

        return _pFTable->m_fnGetTypeHandleByTypeID(_pData, _typeID);
    }

    template <std::size_t Length>
    DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData, const char (&_sName)[Length])
    {
        return _pFTable->m_fnGetTypeHandleByName(_pData, _sName, Length);
    }

    template <std::size_t Length, std::size_t... Lengths>
    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char (&_sName)[Length], const char (&..._sNamespaces)[Lengths])
    {
        const __data& _data = *_pData;
        static const std::size_t& _argsSize = sizeof...(Lengths);

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);

            if(_argsSize == DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data))
            {
                uint32_t _index = 0;
                bool _res = true;

                compare_namespace(_type.m_namespaces, _index, _res, _sNamespaces...);

                if(_res && strcmp(_type.m_sTypeName, _sName) == 0)
                {
                    return _type.m_uiSlotIndex;
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char* _sName, const std::vector<const char*>& _aNamespaces);

    const __type_t& get_type(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle);

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap);

    template <typename Type>
    bool inherits(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnInheritsByTypeID(_pData, _typeHandle, typeid(Type).hash_code());
    }

    DUCKVIL_RESOURCE(ntype_t) get_type(__ftable* _pReflection, __data* _pData, const std::vector<const char*>& _aNamespaces);

    const __ntype_t& get_type(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle);

}}