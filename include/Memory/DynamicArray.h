#pragma once

#include <cstddef>

#include "Memory/Internal/FreeListAllocator.h"

#define _inline 			static inline

#if ( defined _WIN32 || defined _WIN64 )
#define _force_inline 		static __forceinline
#elif ( defined __APPLE__ || defined _APPLE )
#define _force_inline 		static __attribute__((always_inline))
#else
#define _force_inline 		_inline
#endif

struct __duckvil_dynamic_array
{
    std::size_t m_ullSize;
    std::size_t m_ullCapacity;
};

#define DUCKVIL_DYNAMIC_ARRAY_HEAD(arr) \
    ((__duckvil_dynamic_array*)((uint8_t*)arr - sizeof(__duckvil_dynamic_array)))

#define DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) \
    DUCKVIL_DYNAMIC_ARRAY_HEAD(arr)->m_ullSize

#define DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) \
    DUCKVIL_DYNAMIC_ARRAY_HEAD(arr)->m_ullCapacity

#define DUCKVIL_DYNAMIC_ARRAY_FULL(arr) \
    (DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) == DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr))

_inline void* duckvil_dynamic_array_resize(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, void* _pArray, std::size_t _ullSize, std::size_t _ullAmount)
{
    std::size_t _capacity;
    __duckvil_dynamic_array* _data = 0;

    if(_pArray)
    {
        _capacity = _ullAmount;
        _data = (__duckvil_dynamic_array*)_pMemoryInterface->m_fnFreeListReallocate_(_pAllocator, DUCKVIL_DYNAMIC_ARRAY_HEAD(_pArray), _capacity * _ullSize + sizeof(__duckvil_dynamic_array), _capacity * _ullSize + sizeof(__duckvil_dynamic_array), alignof(__duckvil_dynamic_array));
    }
    else
    {
        _capacity = 0;
        _data = (__duckvil_dynamic_array*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _capacity * _ullSize + sizeof(__duckvil_dynamic_array), alignof(__duckvil_dynamic_array));
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

#define DUCKVIL_DYNAMIC_ARRAY_NEED_GROW(arr, n) \
    (arr == nullptr || DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) + n >= DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr))

#define DUCKVIL_DYNAMIC_ARRAY_GROW(memory_interface, allocator, arr) \
    duckvil_dynamic_array_resize(memory_interface, allocator, arr, sizeof(*(arr)), DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) ? DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr) * 2 : 1)

#define DUCKVIL_DYNAMIC_ARRAY_PUSH(memory_interface, allocator, arr, val) \
    if(arr == nullptr || (arr != nullptr && DUCKVIL_DYNAMIC_ARRAY_NEED_GROW(arr, 1))) \
    { \
        *((void**)&(arr)) = DUCKVIL_DYNAMIC_ARRAY_GROW(memory_interface, allocator, arr);\
    } \
    (arr)[DUCKVIL_DYNAMIC_ARRAY_SIZE(arr)] = val; \
    DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) += 1;

#define DUCKVIL_DYNAMIC_ARRAY_RESERVE(memory_interface, allocator, arr, amount) \
    if(arr == nullptr || amount > DUCKVIL_DYNAMIC_ARRAY_CAPACITY(arr)) \
    { \
        *((void**)&(arr)) = duckvil_dynamic_array_resize(memory_interface, allocator, arr, sizeof(*arr), amount) \
    }

#define DUCKVIL_DYNAMIC_ARRAY_EMPTY(arr) \
    (arr != nullptr && DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) == 0)

#define DUCKVIL_DYNAMIC_ARRAY_POP(arr) \
    if(arr != nullptr && !DUCKVIL_DYNAMIC_ARRAY_EMPTY(arr)) \
    { \
        DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) -= 1; \
    }

#define DUCKVIL_DYNAMIC_ARRAY_FOR(arr, type, iter_name) \
    for(type* iter_name = arr; iter_name != DUCKVIL_DYNAMIC_ARRAY_BACK(arr); ++iter_name)

#define DUCKVIL_DYNAMIC_ARRAY_BACK(arr) \
    (arr + (DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) ? DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) - 1 : 0))

#define DUCKVIL_DYNAMIC_ARRAY_NEW(memory_interface, allocator, type) \
    (type*)duckvil_dynamic_array_resize(memory_interface, allocator, nullptr, sizeof(type), 0)

#define DUCKVIL_DYNAMIC_ARRAY_CLEAR(arr) \
    DUCKVIL_DYNAMIC_ARRAY_SIZE(arr) = 0

#define DUCKVIL_DYNAMIC_ARRAY(type) type*

// TODO: Free

