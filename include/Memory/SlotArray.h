#pragma once

#include "Memory/DynamicArray.h"

#define DUCKVIL_SLOT_ARRAY_INVALID_HANDLE std::numeric_limits<uint32_t>::max()

struct __duckvil_slot_array_base
{
    DUCKVIL_DYNAMIC_ARRAY(uint32_t) m_handleIndices;
    DUCKVIL_DYNAMIC_ARRAY(uint32_t) m_freeIndices;
};

#define DUCKVIL_SLOT_ARRAY_DECLARE(t) \
    struct __duckvil_slot_array_##t \
    { \
        __duckvil_slot_array_base m_base; \
        DUCKVIL_DYNAMIC_ARRAY(t) m_data; \
        uint32_t (*_insert_callback)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __duckvil_slot_array_##t*, t); \
    }; \
    /*TODO: Force inline*/ uint32_t duckvil_slot_array_##t##_insert(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __duckvil_slot_array_ ## t* s, t v) \
    { \
        uint32_t _free_index = duckvil_slot_array_next(_pMemoryInterface, _pAllocator, (__duckvil_slot_array_base*)s); \
        DUCKVIL_DYNAMIC_ARRAY_PUSH(_pMemoryInterface, _pAllocator, s->m_data, v); \
        s->m_base.m_handleIndices[_free_index] = DUCKVIL_DYNAMIC_ARRAY_SIZE(s->m_data) - 1; \
        return _free_index; \
    } \
    inline __duckvil_slot_array_##t duckvil_slot_array_##t##_new(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator) \
    { \
        __duckvil_slot_array_##t _slot_array; \
        _slot_array.m_data = DUCKVIL_DYNAMIC_ARRAY_NEW(_pMemoryInterface, _pAllocator, t); \
        _slot_array.m_base.m_handleIndices = DUCKVIL_DYNAMIC_ARRAY_NEW(_pMemoryInterface, _pAllocator, uint32_t); \
        _slot_array.m_base.m_freeIndices = DUCKVIL_DYNAMIC_ARRAY_NEW(_pMemoryInterface, _pAllocator, uint32_t); \
        _slot_array._insert_callback = &duckvil_slot_array_##t##_insert; \
        return _slot_array; \
    }

/*TODO: Force inline*/
uint32_t duckvil_slot_array_next(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, __duckvil_slot_array_base* _pBase)
{
    if(DUCKVIL_DYNAMIC_ARRAY_EMPTY(_pBase->m_freeIndices))
    {
        DUCKVIL_DYNAMIC_ARRAY_PUSH(_pMemoryInterface, _pAllocator, _pBase->m_handleIndices, 0);

        return DUCKVIL_DYNAMIC_ARRAY_SIZE(_pBase->m_handleIndices) - 1;
    }

    uint32_t _index = _pBase->m_freeIndices[0];

    if(DUCKVIL_DYNAMIC_ARRAY_SIZE(_pBase->m_freeIndices) > 1)
    {
        std::size_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_pBase->m_freeIndices);
        uint32_t _tmp = _pBase->m_freeIndices[_size - 1];

        _pBase->m_freeIndices[0] = _tmp;

        DUCKVIL_DYNAMIC_ARRAY_POP(_pBase->m_freeIndices);
    }
    else
    {
        DUCKVIL_DYNAMIC_ARRAY_CLEAR(_pBase->m_freeIndices);
    }

    return _index;
}

#define DUCKVIL_SLOT_ARRAY_CLEAR(s) \
    DUCKVIL_DYNAMIC_ARRAY_CLEAR(s.m_base.m_handleIndices); \
    DUCKVIL_DYNAMIC_ARRAY_CLEAR(s.m_base.m_freeIndices); \
    DUCKVIL_DYNAMIC_ARRAY_CLEAR(s.m_data);

#define DUCKVIL_SLOT_ARRAY_SIZE(s) \
    DUCKVIL_DYNAMIC_ARRAY_SIZE(s.m_data)

#define DUCKVIL_SLOT_ARRAY_NEW(memory_interface, allocator, t) \
    duckvil_slot_array_##t##_new(memory_interface, allocator)

#define DUCKVIL_SLOT_ARRAY_INSERT(memory_interface, allocator, s, v) \
    s._insert_callback(memory_interface, allocator, &s, v)

#define DUCKVIL_SLOT_ARRAY_GET(s, handle) \
    (s.m_data[s.m_base.m_handleIndices[handle]])

#define DUCKVIL_SLOT_ARRAY_GET_POINTER(s, handle) \
    (&s.m_data[s.m_base.m_handleIndices[handle]])

#define DUCKVIL_SLOT_ARRAY_VALID(s, handle) \
    (handle < DUCKVIL_SLOT_ARRAY_SIZE(s))

#define DUCKVIL_SLOT_ARRAY(t) \
    __duckvil_slot_array_##t