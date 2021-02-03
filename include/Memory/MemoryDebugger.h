#pragma once

#include <vector>

#ifdef DUCKVIL_MEMORY_DEBUG
enum duckvil_memory_allocator_type
{
    duckvil_memory_allocator_type_free_list,
    duckvil_memory_allocator_type_linear,
    duckvil_memory_allocator_type_vector,
    duckvil_memory_allocator_type_queue,
    duckvil_memory_allocator_type_stack,
    duckvil_memory_allocator_type_array
};

struct duckvil_memory_debug_info
{
    void* m_pAllocator;
    duckvil_memory_debug_info* m_pParent;
    duckvil_memory_allocator_type m_allocatorType;
    char m_aLabel[32];
    std::vector<duckvil_memory_debug_info*> m_aOther;
};
#endif