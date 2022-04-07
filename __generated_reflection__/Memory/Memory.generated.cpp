#include "Memory/Memory.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(20)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<void(*)(allocator*, allocator*, duckvil_memory_allocator_type)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::allocator, m_fnOnAllocate), "m_fnOnAllocate");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<void(*)(allocator*, allocator*)>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::allocator, m_fnOnDeallocate), "m_fnOnDeallocate");
#endif
#ifdef DUCKVIL_MEMORY_DEBUGGER
_property = record_property<duckvil_memory_debug_info*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::allocator, m_pDebugData), "m_pDebugData");
#endif
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::allocator, m_ullCapacity), "m_ullCapacity");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::allocator, m_ullUsed), "m_ullUsed");
_type = record_type<Duckvil::Memory::linear_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "linear_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_type = record_type<Duckvil::Memory::fixed_stack_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "fixed_stack_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_stack_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::fixed_queue_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "fixed_queue_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_queue_allocator, m_ullBlockSize), "m_ullBlockSize");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_queue_allocator, m_ullTail), "m_ullTail");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_queue_allocator, m_ullHead), "m_ullHead");
_type = record_type<Duckvil::Memory::fixed_array_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "fixed_array_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_array_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::__free_list_node>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__free_list_node");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
_type = record_type<Duckvil::Memory::free_list_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "free_list_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<__free_list_node*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::free_list_allocator, m_pHead), "m_pHead");
_type = record_type<Duckvil::Memory::fixed_vector_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "fixed_vector_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::fixed_vector_allocator, m_ullBlockSize), "m_ullBlockSize");
_type = record_type<Duckvil::Memory::byte_buffer_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "byte_buffer_allocator");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
record_inheritance<allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::byte_buffer_allocator, m_ullPosition), "m_ullPosition");
_type = record_type<Duckvil::Memory::ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ftable");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Memory");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 20);
_property = record_property<ftable::_basic_allocate>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnBasicAllocate), "m_fnBasicAllocate");
_property = record_property<ftable::_basic_free>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnBasicFree), "m_fnBasicFree");
_property = record_property<ftable::_calculate_aligned_pointer>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculateAlignedPointer), "m_fnCalculateAlignedPointer");
_property = record_property<ftable::_calculate_padding>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculatePadding), "m_fnCalculatePadding");
_property = record_property<ftable::_calculate_padding_h>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculatePaddingH), "m_fnCalculatePaddingH");
_property = record_property<ftable::_calculate_aligned_pointer_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculateAlignedPointer_), "m_fnCalculateAlignedPointer_");
_property = record_property<ftable::_calculate_padding_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculatePadding_), "m_fnCalculatePadding_");
_property = record_property<ftable::_calculate_padding_h_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnCalculatePaddingH_), "m_fnCalculatePaddingH_");
_property = record_property<ftable::_linear_allocate>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocate), "m_fnLinearAllocate");
_property = record_property<ftable::_linear_clear>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearClear), "m_fnLinearClear");
_property = record_property<ftable::_fixed_stack_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackAllocate_), "m_fnFixedStackAllocate_");
_property = record_property<ftable::_fixed_stack_allocate_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackAllocateSize_), "m_fnFixedStackAllocateSize_");
_property = record_property<ftable::_fixed_stack_top_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackTop_), "m_fnFixedStackTop_");
_property = record_property<ftable::_fixed_stack_pop_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackPop_), "m_fnFixedStackPop_");
_property = record_property<ftable::_fixed_stack_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackEmpty_), "m_fnFixedStackEmpty_");
_property = record_property<ftable::_fixed_stack_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackFull_), "m_fnFixedStackFull_");
_property = record_property<ftable::_fixed_stack_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackClear_), "m_fnFixedStackClear_");
_property = record_property<ftable::_fixed_stack_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackSize_), "m_fnFixedStackSize_");
_property = record_property<ftable::_fixed_stack_capacity_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedStackCapacity_), "m_fnFixedStackCapacity_");
_property = record_property<ftable::_fixed_queue_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueAllocate_), "m_fnFixedQueueAllocate_");
_property = record_property<ftable::_fixed_queue_allocate_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueAllocateSize_), "m_fnFixedQueueAllocateSize_");
_property = record_property<ftable::_fixed_queue_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueBegin_), "m_fnFixedQueueBegin_");
_property = record_property<ftable::_fixed_queue_pop_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueuePop_), "m_fnFixedQueuePop_");
_property = record_property<ftable::_fixed_queue_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueEmpty_), "m_fnFixedQueueEmpty_");
_property = record_property<ftable::_fixed_queue_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueFull_), "m_fnFixedQueueFull_");
_property = record_property<ftable::_fixed_queue_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueClear_), "m_fnFixedQueueClear_");
_property = record_property<ftable::_fixed_queue_resize_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueResize_), "m_fnFixedQueueResize_");
_property = record_property<ftable::_fixed_queue_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueSize_), "m_fnFixedQueueSize_");
_property = record_property<ftable::_fixed_queue_capacity_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedQueueCapacity_), "m_fnFixedQueueCapacity_");
_property = record_property<ftable::_fixed_array_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayAllocate_), "m_fnFixedArrayAllocate_");
_property = record_property<ftable::_fixed_array_allocate_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayAllocateSize_), "m_fnFixedArrayAllocateSize_");
_property = record_property<ftable::_fixed_array_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayBegin_), "m_fnFixedArrayBegin_");
_property = record_property<ftable::_fixed_array_back_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayBack_), "m_fnFixedArrayBack_");
_property = record_property<ftable::_fixed_array_at_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayAt_), "m_fnFixedArrayAt_");
_property = record_property<ftable::_fixed_array_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayEmpty_), "m_fnFixedArrayEmpty_");
_property = record_property<ftable::_fixed_array_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayFull_), "m_fnFixedArrayFull_");
_property = record_property<ftable::_fixed_array_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArraySize_), "m_fnFixedArraySize_");
_property = record_property<ftable::_fixed_array_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedArrayClear_), "m_fnFixedArrayClear_");
_property = record_property<ftable::_free_list_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocate_), "m_fnFreeListAllocate_");
_property = record_property<ftable::_free_list_allocate_d_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocate_D_), "m_fnFreeListAllocate_D_");
_property = record_property<ftable::_free_list_reallocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListReallocate_), "m_fnFreeListReallocate_");
_property = record_property<ftable::_free_list_free_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListFree_), "m_fnFreeListFree_");
_property = record_property<ftable::_free_list_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListClear_), "m_fnFreeListClear_");
_property = record_property<ftable::_fixed_vector_allocate_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorAllocate_), "m_fnFixedVectorAllocate_");
_property = record_property<ftable::_fixed_vector_allocate_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorAllocateSize_), "m_fnFixedVectorAllocateSize_");
_property = record_property<ftable::_fixed_vector_begin_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorBegin_), "m_fnFixedVectorBegin_");
_property = record_property<ftable::_fixed_vector_back_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorBack_), "m_fnFixedVectorBack_");
_property = record_property<ftable::_fixed_vector_end_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorEnd_), "m_fnFixedVectorEnd_");
_property = record_property<ftable::_fixed_vector_at_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorAt_), "m_fnFixedVectorAt_");
_property = record_property<ftable::_fixed_vector_empty_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorEmpty_), "m_fnFixedVectorEmpty_");
_property = record_property<ftable::_fixed_vector_full_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorFull_), "m_fnFixedVectorFull_");
_property = record_property<ftable::_fixed_vector_clear_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorClear_), "m_fnFixedVectorClear_");
_property = record_property<ftable::_fixed_vector_resize_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorResize_), "m_fnFixedVectorResize_");
_property = record_property<ftable::_fixed_vector_size_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorSize_), "m_fnFixedVectorSize_");
_property = record_property<ftable::_fixed_vector_capacity_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorCapacity_), "m_fnFixedVectorCapacity_");
_property = record_property<ftable::_fixed_vector_erase_>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFixedVectorErase_), "m_fnFixedVectorErase_");
_property = record_property<ftable::_byte_buffer_will_fit>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferWillFit_), "m_fnByteBufferWillFit_");
_property = record_property<ftable::_byte_buffer_clear>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferClear_), "m_fnByteBufferClear_");
_property = record_property<ftable::_byte_buffer_resize>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferResize_), "m_fnByteBufferResize_");
_property = record_property<ftable::_byte_buffer_seek_to_begin>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferSeekToBegin_), "m_fnByteBufferSeekToBegin_");
_property = record_property<ftable::_byte_buffer_seek_to_end>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferSeekToEnd_), "m_fnByteBufferSeekToEnd_");
_property = record_property<ftable::_byte_buffer_advance>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferAdvance_), "m_fnByteBufferAdvance_");
_property = record_property<ftable::_byte_buffer_write>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferWrite_), "m_fnByteBufferWrite_");
_property = record_property<ftable::_byte_buffer_read>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnByteBufferRead_), "m_fnByteBufferRead_");
_property = record_property<ftable::_linear_allocate_linear_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateLinearAllocator), "m_fnLinearAllocateLinearAllocator");
_property = record_property<ftable::_linear_allocate_fixed_stack_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateFixedStackAllocator), "m_fnLinearAllocateFixedStackAllocator");
_property = record_property<ftable::_linear_allocate_fixed_array_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateFixedArrayAllocator), "m_fnLinearAllocateFixedArrayAllocator");
_property = record_property<ftable::_linear_allocate_free_list_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateFreeListAllocator), "m_fnLinearAllocateFreeListAllocator");
_property = record_property<ftable::_linear_allocate_fixed_vector_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateFixedVectorAllocator), "m_fnLinearAllocateFixedVectorAllocator");
_property = record_property<ftable::_linear_allocate_byte_buffer_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnLinearAllocateByteBufferAllocator), "m_fnLinearAllocateByteBufferAllocator");
_property = record_property<ftable::_free_list_allocate_fixed_queue_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateFixedQueueAllocator), "m_fnFreeListAllocateFixedQueueAllocator");
_property = record_property<ftable::_free_list_allocate_free_list_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateFreeListAllocator), "m_fnFreeListAllocateFreeListAllocator");
_property = record_property<ftable::_free_list_allocate_fixed_vector_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateFixedVectorAllocator), "m_fnFreeListAllocateFixedVectorAllocator");
_property = record_property<ftable::_free_list_allocate_fixed_stack_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateFixedStackAllocator), "m_fnFreeListAllocateFixedStackAllocator");
_property = record_property<ftable::_free_list_allocate_fixed_array_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateFixedArrayAllocator), "m_fnFreeListAllocateFixedArrayAllocator");
_property = record_property<ftable::_free_list_allocate_byte_buffer_allocator>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::Memory::ftable, m_fnFreeListAllocateByteBufferAllocator), "m_fnFreeListAllocateByteBufferAllocator");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Memory/Memory.h", 20);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 20;
}
#endif
