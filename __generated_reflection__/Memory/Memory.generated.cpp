#include "Memory/Memory.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(18)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::__allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__allocator, m_ullCapacity), "m_ullCapacity");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__allocator, m_ullUsed), "m_ullUsed");
_property = record_property<uint8_t*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__allocator, m_pMemory), "m_pMemory");
_type = record_type<Duckvil::Memory::__linear_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__linear_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_type = record_type<Duckvil::Memory::__fixed_stack_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__fixed_stack_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_stack_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::__fixed_queue_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__fixed_queue_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_queue_allocator, m_ullBlockSize), "m_ullBlockSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_queue_allocator, m_ullTail), "m_ullTail");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_queue_allocator, m_ullHead), "m_ullHead");
_type = record_type<Duckvil::Memory::__fixed_array_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__fixed_array_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_array_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::__free_list_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__free_list_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__free_list_allocator, m_pHead), "m_pHead");
_type = record_type<Duckvil::Memory::__fixed_vector_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__fixed_vector_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__allocator>(_pData), __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::__fixed_vector_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::IMemory>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "IMemory");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 18);
_property = record_property<IMemory::_basic_allocate>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnBasicAllocate), "m_fnBasicAllocate");
_property = record_property<IMemory::_calculate_aligned_pointer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculateAlignedPointer), "m_fnCalculateAlignedPointer");
_property = record_property<IMemory::_calculate_padding>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculatePadding), "m_fnCalculatePadding");
_property = record_property<IMemory::_calculate_padding_h>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculatePaddingH), "m_fnCalculatePaddingH");
_property = record_property<IMemory::_calculate_aligned_pointer_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculateAlignedPointer_), "m_fnCalculateAlignedPointer_");
_property = record_property<IMemory::_calculate_padding_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculatePadding_), "m_fnCalculatePadding_");
_property = record_property<IMemory::_calculate_padding_h_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnCalculatePaddingH_), "m_fnCalculatePaddingH_");
_property = record_property<IMemory::_linear_allocate>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnLinearAllocate), "m_fnLinearAllocate");
_property = record_property<IMemory::_linear_clear>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnLinearClear), "m_fnLinearClear");
_property = record_property<IMemory::_linear_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnLinearAllocate_), "m_fnLinearAllocate_");
_property = record_property<IMemory::_linear_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnLinearClear_), "m_fnLinearClear_");
_property = record_property<IMemory::_fixed_stack_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackAllocate_), "m_fnFixedStackAllocate_");
_property = record_property<IMemory::_fixed_stack_top_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackTop_), "m_fnFixedStackTop_");
_property = record_property<IMemory::_fixed_stack_pop_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackPop_), "m_fnFixedStackPop_");
_property = record_property<IMemory::_fixed_stack_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackEmpty_), "m_fnFixedStackEmpty_");
_property = record_property<IMemory::_fixed_stack_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackFull_), "m_fnFixedStackFull_");
_property = record_property<IMemory::_fixed_stack_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedStackClear_), "m_fnFixedStackClear_");
_property = record_property<IMemory::_fixed_queue_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueueAllocate_), "m_fnFixedQueueAllocate_");
_property = record_property<IMemory::_fixed_queue_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueueBegin_), "m_fnFixedQueueBegin_");
_property = record_property<IMemory::_fixed_queue_pop_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueuePop_), "m_fnFixedQueuePop_");
_property = record_property<IMemory::_fixed_queue_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueueEmpty_), "m_fnFixedQueueEmpty_");
_property = record_property<IMemory::_fixed_queue_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueueFull_), "m_fnFixedQueueFull_");
_property = record_property<IMemory::_fixed_queue_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedQueueClear_), "m_fnFixedQueueClear_");
_property = record_property<IMemory::_fixed_array_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayAllocate_), "m_fnFixedArrayAllocate_");
_property = record_property<IMemory::_fixed_array_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayBegin_), "m_fnFixedArrayBegin_");
_property = record_property<IMemory::_fixed_array_back_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayBack_), "m_fnFixedArrayBack_");
_property = record_property<IMemory::_fixed_array_at_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayAt_), "m_fnFixedArrayAt_");
_property = record_property<IMemory::_fixed_array_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayEmpty_), "m_fnFixedArrayEmpty_");
_property = record_property<IMemory::_fixed_array_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayFull_), "m_fnFixedArrayFull_");
_property = record_property<IMemory::_fixed_array_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArraySize_), "m_fnFixedArraySize_");
_property = record_property<IMemory::_fixed_array_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedArrayClear_), "m_fnFixedArrayClear_");
_property = record_property<IMemory::_free_list_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFreeListAllocate_), "m_fnFreeListAllocate_");
_property = record_property<IMemory::_free_list_allocate_d_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFreeListAllocate_D_), "m_fnFreeListAllocate_D_");
_property = record_property<IMemory::_free_list_reallocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFreeListReallocate_), "m_fnFreeListReallocate_");
_property = record_property<IMemory::_free_list_free_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFreeListFree_), "m_fnFreeListFree_");
_property = record_property<IMemory::_free_list_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFreeListClear_), "m_fnFreeListClear_");
_property = record_property<IMemory::_fixed_vector_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorAllocate_), "m_fnFixedVectorAllocate_");
_property = record_property<IMemory::_fixed_vector_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorBegin_), "m_fnFixedVectorBegin_");
_property = record_property<IMemory::_fixed_vector_back_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorBack_), "m_fnFixedVectorBack_");
_property = record_property<IMemory::_fixed_vector_at_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorAt_), "m_fnFixedVectorAt_");
_property = record_property<IMemory::_fixed_vector_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorEmpty_), "m_fnFixedVectorEmpty_");
_property = record_property<IMemory::_fixed_vector_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorFull_), "m_fnFixedVectorFull_");
_property = record_property<IMemory::_fixed_vector_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorClear_), "m_fnFixedVectorClear_");
_property = record_property<IMemory::_fixed_vector_resize_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorResize_), "m_fnFixedVectorResize_");
_property = record_property<IMemory::_fixed_vector_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorSize_), "m_fnFixedVectorSize_");
_property = record_property<IMemory::_fixed_vector_capacity_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorCapacity_), "m_fnFixedVectorCapacity_");
_property = record_property<IMemory::_fixed_vector_erase_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnFixedVectorErase_), "m_fnFixedVectorErase_");
_property = record_property<IMemory::_allocate_linear_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateLinearAllocator), "m_fnAllocateLinearAllocator");
_property = record_property<IMemory::_allocate_fixed_stack_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateFixedStackAllocator), "m_fnAllocateFixedStackAllocator");
_property = record_property<IMemory::_allocate_fixed_queue_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateFixedQueueAllocator), "m_fnAllocateFixedQueueAllocator");
_property = record_property<IMemory::_allocate_fixed_array_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateFixedArrayAllocator), "m_fnAllocateFixedArrayAllocator");
_property = record_property<IMemory::_allocate_free_list_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateFreeListAllocator), "m_fnAllocateFreeListAllocator");
_property = record_property<IMemory::_allocate_fixed_vector_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::IMemory, m_fnAllocateFixedVectorAllocator), "m_fnAllocateFixedVectorAllocator");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "Memory/Memory.h", 18 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 18;
}
#endif
