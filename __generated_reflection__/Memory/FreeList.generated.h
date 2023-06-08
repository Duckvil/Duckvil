#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Memory_FreeList_h_25_REFLECTION_MODULE_DEPENDENCY_INJECTION \
public:\
static void Push(DependencyInjection::IDependencyInjector* _pFAP, const void* _pData);
#define Memory_FreeList_h_25_REFLECTION_MODULE_DUMMY 
#define Memory_FreeList_h_25_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<FreeList*>(_pObject); }
#define Memory_FreeList_h_25_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pContainer", m_pContainer); \
}
#define Memory_FreeList_h_25_GENERATED_BODY \
Memory_FreeList_h_25_REFLECTION_MODULE_DEPENDENCY_INJECTION \
Memory_FreeList_h_25_REFLECTION_MODULE_DUMMY \
Memory_FreeList_h_25_REFLECTION_MODULE_CASTER \
Memory_FreeList_h_25_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Memory_FreeList_h

