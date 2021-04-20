#pragma once

#include <cstddef>

#include "Memory/Internal/FreeListAllocator.h"

#if ( defined _WIN32 || defined _WIN64 )
#define _force_inline 		static __forceinline
#elif ( defined __APPLE__ || defined _APPLE )
#define _force_inline 		static __attribute__((always_inline))
#else
#define _force_inline 		static inline
#endif

struct __duckvil_dynamic_array
{
    uint32_t m_ullSize;
    uint32_t m_ullCapacity;
};

inline __duckvil_dynamic_array* duckvil_dynamic_array_head(void* _pArray)
{
    return (__duckvil_dynamic_array*)((uint8_t*)_pArray - sizeof(__duckvil_dynamic_array));
}

inline uint32_t duckvil_dynamic_array_size(void* _pArray)
{
    return _pArray == nullptr ? 0 : duckvil_dynamic_array_head(_pArray)->m_ullSize;
}

inline uint32_t duckvil_dynamic_array_capacity(void* _pArray)
{
    return _pArray == nullptr ? 0 : duckvil_dynamic_array_head(_pArray)->m_ullCapacity;
}

inline bool duckvil_dynamic_array_full(void* _pArray)
{
    return duckvil_dynamic_array_size(_pArray) == duckvil_dynamic_array_capacity(_pArray);
}

#define DUCKVIL_DYNAMIC_ARRAY_HEAD(arr) \
    duckvil_dynamic_array_head(arr)

#define DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) \
    duckvil_dynamic_array_size(arr)

#define DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) \
    duckvil_dynamic_array_capacity(arr)

#define DUCKVIL_DYNAMIC_ARRAY_FULL(arr) \
    duckvil_dynamic_array_full(arr)

static inline void* duckvil_dynamic_array_resize(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, void* _pArray, std::size_t _ullSize, std::size_t _ullAmount)
{
    uint32_t _capacity;
    __duckvil_dynamic_array* _data = DUCKVIL_DYNAMIC_ARRAY_HEAD(_pArray);

    if(_pArray)
    {
        _capacity = _ullAmount;
        _data = (__duckvil_dynamic_array*)_pMemoryInterface->m_fnFreeListReallocate_(_pAllocator, DUCKVIL_DYNAMIC_ARRAY_HEAD(_pArray), (_data->m_ullCapacity * _ullSize) + sizeof(__duckvil_dynamic_array), (_capacity * _ullSize) + sizeof(__duckvil_dynamic_array), alignof(__duckvil_dynamic_array));
    }
    else
    {
        _capacity = _ullAmount;
        _data = (__duckvil_dynamic_array*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, (_capacity * _ullSize) + sizeof(__duckvil_dynamic_array), alignof(__duckvil_dynamic_array));
    }

    if(_data)
    {
        _data->m_ullCapacity = _capacity;

        if(_pArray == nullptr)
        {
            _data->m_ullSize = 0;
        }

        return (uint8_t*)_data + sizeof(__duckvil_dynamic_array);
    }

    return nullptr;
}

inline bool duckvil_dynamic_array_need_grow(void* _pArray, uint32_t _ullN)
{
    return (_pArray == nullptr || duckvil_dynamic_array_size(_pArray) + _ullN >= duckvil_dynamic_array_capacity(_pArray));
}

inline void duckvil_dynamic_array_push_data(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, void** _pArray, void* _pData, std::size_t _ullSize)
{
    if(duckvil_dynamic_array_need_grow(*_pArray, 1))
    {
        uint32_t _capacity = duckvil_dynamic_array_capacity(*_pArray);

        *_pArray = duckvil_dynamic_array_resize(_pMemoryInterface, _pAllocator, *_pArray, _ullSize, _capacity ? _capacity * 2 : 1);
    }

    size_t _offset = DUCKVIL_DYNAMIC_ARRAY_SIZE(*_pArray);

    memcpy(((uint8_t*)*_pArray) + (_offset * _ullSize), _pData, _ullSize);

    DUCKVIL_DYNAMIC_ARRAY_HEAD(*_pArray)->m_ullSize++;
}

inline void duckvil_dynamic_array_set_data_i(void** _ppArray, void* _pData, std::size_t _ullSize, uint32_t _uiOffset)
{
    memcpy(((uint8_t*)*_ppArray) + (_uiOffset * _ullSize), _pData, _ullSize);
}

#define DUCKVIL_DYNAMIC_ARRAY_NEED_GROW(arr, n) \
    duckvil_dynamic_array_need_grow(arr, n)

#define DUCKVIL_DYNAMIC_ARRAY_GROW(memory_interface, allocator, arr) \
    duckvil_dynamic_array_resize(memory_interface, allocator, arr, sizeof(*(arr)), DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) ? DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) * 2 : 1)

#define DUCKVIL_DYNAMIC_ARRAY_PUSH(memory_interface, allocator, arr, val) \
    if(arr == nullptr || (arr != nullptr && DUCKVIL_DYNAMIC_ARRAY_NEED_GROW(arr, 1))) \
    { \
        *((void**)&(arr)) = DUCKVIL_DYNAMIC_ARRAY_GROW(memory_interface, allocator, arr); \
    } \
    (arr)[DUCKVIL_DYNAMIC_ARRAY_SIZE(arr)] = val; \
    DUCKVIL_DYNAMIC_ARRAY_HEAD(arr)->m_ullSize++;

#define DUCKVIL_DYNAMIC_ARRAY_RESERVE(memory_interface, allocator, arr, amount) \
    if(arr == nullptr || amount > DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr)) \
    { \
        *((void**)&(arr)) = duckvil_dynamic_array_resize(memory_interface, allocator, arr, sizeof(*arr), amount); \
    }

#define DUCKVIL_DYNAMIC_ARRAY_EMPTY(arr) \
    (arr != nullptr && DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) == 0)

#define DUCKVIL_DYNAMIC_ARRAY_POP(arr) \
    if(!DUCKVIL_DYNAMIC_ARRAY_EMPTY(arr)) \
    { \
        DUCKVIL_DYNAMIC_ARRAY_HEAD(arr)->m_ullSize -= 1; \
    }

#define DUCKVIL_DYNAMIC_ARRAY_FOR(arr, type, iter_name) \
    for(type* iter_name = arr; iter_name != DUCKVIL_DYNAMIC_ARRAY_BACK(arr); ++iter_name)

#define DUCKVIL_DYNAMIC_ARRAY_BACK(arr) \
    (arr + (DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) ? DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) - 1 : 0))

#define DUCKVIL_DYNAMIC_ARRAY_NEW(memory_interface, allocator, type) \
    (type*)duckvil_dynamic_array_resize(memory_interface, allocator, nullptr, sizeof(type), 0)

#define DUCKVIL_DYNAMIC_ARRAY_CLEAR(arr) \
    if(arr != nullptr) \
    { \
        DUCKVIL_DYNAMIC_ARRAY_HEAD(arr)->m_ullSize = 0; \
    }

#define DUCKVIL_DYNAMIC_ARRAY(type) type*

// TODO: Free

