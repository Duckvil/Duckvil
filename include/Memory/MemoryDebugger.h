#pragma once

#include <vector>

namespace Duckvil { namespace Memory {

    struct allocator;

}}

#ifdef DUCKVIL_MEMORY_DEBUGGER
enum duckvil_memory_allocator_type
{
    duckvil_memory_allocator_type_unknown,
    duckvil_memory_allocator_type_free_list,
    duckvil_memory_allocator_type_linear,
    duckvil_memory_allocator_type_vector,
    duckvil_memory_allocator_type_queue,
    duckvil_memory_allocator_type_stack,
    duckvil_memory_allocator_type_array
};

struct duckvil_memory_debug_info
{
    Duckvil::Memory::allocator* m_pAllocator;
    duckvil_memory_debug_info* m_pParent;
    duckvil_memory_allocator_type m_allocatorType;
    char m_aLabel[128] = { 0 };
    std::vector<duckvil_memory_debug_info*> m_aOther;
};
#define DUCKVIL_DEBUG_MEMORY(allocator, label) strcpy(allocator->m_pDebugData->m_aLabel, "MEMORY_LABEL(" ## __FILE__ ## "): " ## label)
#else
#define DUCKVIL_DEBUG_MEMORY
#endif