#pragma once

#define DUCKVIL_RUNTIME_REFLECTION_SKIP

DUCKVIL_RUNTIME_REFLECTION_SKIP

#include "Memory/DynamicArray.h"

#undef max

#define DUCKVIL_SLOT_ARRAY_INVALID_HANDLE static_cast<uint32_t>(-1)

#define DUCKVIL_SLOT_ARRAY_DECLARE(t) \
    struct __duckvil_slot_array_##t \
    { \
        DUCKVIL_DYNAMIC_ARRAY(uint32_t) m_indices = nullptr; \
        DUCKVIL_DYNAMIC_ARRAY(t) m_data = nullptr; \
    }; \
    static inline __duckvil_slot_array_##t duckvil_slot_array_##t##_new(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator) \
    { \
        __duckvil_slot_array_##t _slot_array; \
        _slot_array.m_data = DUCKVIL_DYNAMIC_ARRAY_NEW(_pMemoryInterface, _pAllocator, t); \
        _slot_array.m_indices = DUCKVIL_DYNAMIC_ARRAY_NEW(_pMemoryInterface, _pAllocator, uint32_t); \
        return _slot_array; \
    }

_force_inline uint32_t duckvil_slot_array_find_next_available_index(DUCKVIL_DYNAMIC_ARRAY(uint32_t) _indices)
{
    uint32_t _index = -1;
    uint32_t _size = duckvil_dynamic_array_size(_indices);

    for(uint32_t i = 0; i < _size; ++i)
    {
        if(_indices[i] == -1)
        {
            _index = i;

            break;
        }
    }

    if(_index == -1)
    {
        _index = duckvil_dynamic_array_size(_indices);
    }

    return _index;
}

_force_inline uint32_t duckvil_slot_array_insert_func(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, void** _ppIndices, void** _ppData, void* _pData, size_t _ullSize, uint32_t* _puiIndex)
{
    uint32_t _index = duckvil_slot_array_find_next_available_index((uint32_t*)*_ppIndices);

    if(_index == duckvil_dynamic_array_size(*_ppIndices))
    {
        uint32_t v = 0;

        duckvil_dynamic_array_push_data(_pMemoryInterface, _pAllocator, _ppIndices, &v, sizeof(uint32_t));

        _index = duckvil_dynamic_array_size(*_ppIndices) - 1;
    }

    duckvil_dynamic_array_push_data(_pMemoryInterface, _pAllocator, _ppData, _pData, _ullSize);

    uint32_t _size = duckvil_dynamic_array_size(*_ppData) - 1;

    duckvil_dynamic_array_set_data_i(_ppIndices, &_size, sizeof(uint32_t), _index);

    if(_puiIndex)
    {
        *_puiIndex = _index;
    }

    return _index;
}

#define DUCKVIL_SLOT_ARRAY_CLEAR(s) \
    DUCKVIL_DYNAMIC_ARRAY_CLEAR(s.m_indices); \
    DUCKVIL_DYNAMIC_ARRAY_CLEAR(s.m_data);

#define DUCKVIL_SLOT_ARRAY_FREE(memory_interface, allocator, s) \
    DUCKVIL_DYNAMIC_ARRAY_FREE(memory_interface, allocator, s.m_indices); \
    s.m_indices = nullptr; \
    DUCKVIL_DYNAMIC_ARRAY_FREE(memory_interface, allocator, s.m_data); \
    s.m_data = nullptr;

#define DUCKVIL_SLOT_ARRAY_SIZE(s) \
    DUCKVIL_DYNAMIC_ARRAY_SIZE(s.m_data)

#define DUCKVIL_SLOT_ARRAY_NEW(memory_interface, allocator, t) \
    duckvil_slot_array_##t##_new(memory_interface, allocator)

#define DUCKVIL_SLOT_ARRAY_EXISTS(s, handle) \
    (handle < DUCKVIL_SLOT_ARRAY_SIZE(s))

#define DUCKVIL_SLOT_ARRAY_HANDLE_VALID(s, handle) \
    (handle < DUCKVIL_DYNAMIC_ARRAY_SIZE(s.m_indices) && s.m_indices[handle] != -1)

template <typename A, typename B>
static inline uint32_t duckvil_slot_array_insert(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, A& _slotArray, B _value)
{
    return duckvil_slot_array_insert_func(_pMemoryInterface, _pAllocator, (void**)&(_slotArray.m_indices), (void**)&(_slotArray.m_data), &_value, sizeof(B), nullptr);
}

template <typename A>
void duckvil_slot_array_erase(A& _slotArray, uint32_t _uiHandle)
{
    if(DUCKVIL_SLOT_ARRAY_SIZE(_slotArray) == 1)
    {
        DUCKVIL_SLOT_ARRAY_CLEAR(_slotArray);
    }
    else if(!DUCKVIL_SLOT_ARRAY_HANDLE_VALID(_slotArray, _uiHandle))
    {

    }
    else
    {
        uint32_t _index = _slotArray.m_indices[_uiHandle];
        uint32_t _current = 0;
        uint32_t _size = DUCKVIL_DYNAMIC_ARRAY_SIZE(_slotArray.m_indices);
        uint32_t _dataSize = DUCKVIL_DYNAMIC_ARRAY_SIZE(_slotArray.m_data);

        for(uint32_t i = 0; i < _size; ++i)
        {
            if(_slotArray.m_indices[i] == _dataSize - 1)
            {
                _current = i;

                break;
            }
        }

        _slotArray.m_data[_index] = *DUCKVIL_DYNAMIC_ARRAY_BACK(_slotArray.m_data);

        DUCKVIL_DYNAMIC_ARRAY_POP(_slotArray.m_data);

        _slotArray.m_indices[_current] = _index;
        _slotArray.m_indices[_uiHandle] = -1;
    }
}

#define DUCKVIL_SLOT_ARRAY_GET(s, handle) \
    (s.m_data[s.m_indices[handle % DUCKVIL_DYNAMIC_ARRAY_SIZE((s.m_indices))]])

#define DUCKVIL_SLOT_ARRAY_GET_POINTER(s, handle) \
    (&(DUCKVIL_SLOT_ARRAY_GET(s, handle)))

#define DUCKVIL_SLOT_ARRAY_VALID(s, handle) \
    (handle < DUCKVIL_SLOT_ARRAY_SIZE(s))

#define DUCKVIL_SLOT_ARRAY(t) \
    __duckvil_slot_array_##t
