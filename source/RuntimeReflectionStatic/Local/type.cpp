#pragma once

#include "RuntimeReflectionStatic/local/type.h"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) get_type(__ftable* _pFTable, __data* _pData, const std::size_t& _ullTypeID)
    {
        return _pFTable->m_fnGetTypeHandleByTypeID(_pData, _ullTypeID);
    }

    DUCKVIL_RESOURCE(type_t) get_type(__data* _pData, const char* _sName, const std::vector<const char*>& _aNamespaces)
    {
        const __data& _data = *_pData;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_data.m_aTypes.m_data); ++i)
        {
            const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_data.m_aTypes, i);
            size_t _namespacesSize = DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data);

            if(_aNamespaces.size() == _namespacesSize)
            {
                bool _valid = true;

                for(uint32_t j = 0; j < _aNamespaces.size(); ++j)
                {
                    const __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    if(strcmp(_namespace.m_sNamespaceName, _aNamespaces[j]) != 0)
                    {
                        _valid = false;

                        break;
                    }
                }

                if(_valid && strcmp(_sName, _type.m_sTypeName) == 0)
                {
                    return { i };
                }
            }
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    const __type_t& get_type(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle)
    {
        return _pReflection->m_fnGetTypeByHandle(_pData, _typeHandle);
    }

    Memory::Vector<DUCKVIL_RESOURCE(type_t)> get_types(__ftable* _pReflection, __data* _pData, const Memory::FreeList& _heap)
    {
        return _pReflection->m_fnGetTypes(_pData, _heap.GetMemoryInterface(), _heap.GetAllocator());
    }

    DUCKVIL_RESOURCE(ntype_t) get_type(__ftable* _pReflection, __data* _pData, const std::vector<const char*>& _aNamespaces)
    {
        return _pReflection->m_fnGetNTypeHandleByNamespaces(_pData, _aNamespaces);
    }

    const __ntype_t& get_type(__ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(ntype_t) _typeHandle)
    {
        return _pReflection->m_fnGetNTypeByHandle(_pData, _typeHandle);
    }

}}