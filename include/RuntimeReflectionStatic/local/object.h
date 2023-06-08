#pragma once

#include "RuntimeReflection/Resources.h"

namespace Duckvil { namespace RuntimeReflection {

    static const std::size_t CONST_CHAR_POINTER_ID = typeid(const char*).hash_code();

    template <typename... Args>
    void* create(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        const __type_t& _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, _typeHandle.m_ID);
        static const std::size_t& _constructorTypeID = typeid(void*(Args...)).hash_code();

        for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++j)
        {
            const __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

            if(_constructor.m_ullTypeID == _constructorTypeID)
            {
                auto _constructorCallback = reinterpret_cast<void* (*)(const Memory::ftable*, Memory::free_list_allocator*, const __ftable*, __data*, bool, Args...)>(_constructor.m_pData);

                return _constructorCallback(_pMemoryInterface, _pAllocator, _pReflection, _pData, _bTracked, _vArgs...);
            }
        }

        return nullptr;
    }

    template <size_t Length, typename... Args>
    void* create(const Memory::FreeList& _memory, const __ftable* _pReflection, __data* _pData, const char _sTypeName[Length], bool _bTracked, const Args&... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _sTypeName, _bTracked, _vArgs...);
    }

    template <typename... Args>
    void* create(const Memory::FreeList& _memory, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, const Args&... _vArgs)
    {
        return create<Args...>(_memory.GetMemoryInterface(), _memory.GetAllocator(), _pReflection, _pData, _typeHandle, _bTracked, _vArgs...);
    }

    void destroy(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject);

    void destroy(const Memory::FreeList& _memory, const __ftable* _pReflection, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, bool _bTracked, void* _pObject);

}}