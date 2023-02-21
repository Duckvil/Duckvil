#include "Memory/FreeList.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

void Duckvil::Memory::FreeList::Push(DependencyInjection::IDependencyInjector* _pFAP, const void* _pData)
{
_pFAP->Push(*static_cast<const FreeList*>(_pData));
}

DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Memory");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("FreeList");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Memory;
_type = record_type<Duckvil::Memory::FreeList>(_data, "FreeList");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Memory");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_meta(_data, _type, Duckvil::DependencyInjection::INJECTABLE, true);
_constructor = record_constructor<Duckvil::Memory::FreeList>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, Duckvil::Memory::ftable*, Duckvil::Memory::linear_allocator*, std::size_t>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, Duckvil::Memory::ftable*, Duckvil::Memory::free_list_allocator*, std::size_t>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, Duckvil::Memory::ftable*, Duckvil::Memory::free_list_allocator*>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, Duckvil::Memory::FreeList const&>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::FreeList, Duckvil::Memory::FreeList&&>(_data, _type);
_destructor = record_destructor<Duckvil::Memory::FreeList>(_data, _type);
_function = record_function<Duckvil::Memory::FreeList, Duckvil::Memory::FreeList&, Duckvil::Memory::FreeList const&>(_data, _type, &Duckvil::Memory::FreeList::operator=, "operator=");
_function = record_function<Duckvil::Memory::FreeList, Duckvil::Memory::FreeList&, Duckvil::Memory::FreeList&&>(_data, _type, &Duckvil::Memory::FreeList::operator=, "operator=");
_function = record_function<Duckvil::Memory::FreeList, void, void*>(_data, _type, &Duckvil::Memory::FreeList::Free, "Free");
_function = record_function<Duckvil::Memory::FreeList, Duckvil::Memory::ftable*>(_data, _type, &Duckvil::Memory::FreeList::GetMemoryInterface, "GetMemoryInterface");
_function = record_function<Duckvil::Memory::FreeList, Duckvil::Memory::free_list_allocator*>(_data, _type, &Duckvil::Memory::FreeList::GetAllocator, "GetAllocator");
_function = record_function<void, Duckvil::DependencyInjection::IDependencyInjector*, const void*>(_data, _type, &Duckvil::Memory::FreeList::Push, "Push");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<void*, void*>(_data, _type, &Duckvil::Memory::FreeList::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.push_back("free_list_context");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Memory::free_list_context>(_data, "free_list_context");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Memory");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
_property = record_property<Duckvil::Memory::FreeList>(_data, _type, offsetof(Duckvil::Memory::free_list_context, m_heap), "m_heap");
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Memory::free_list_context, m_bInitialized), "m_bInitialized");
_constructor = record_constructor<Duckvil::Memory::free_list_context>(_data, _type);
_constructor = record_constructor<Duckvil::Memory::free_list_context, Duckvil::Memory::FreeList const&>(_data, _type);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Memory/FreeList.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
