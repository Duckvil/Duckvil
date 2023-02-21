#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Engine_EntityFactory_h_28_REFLECTION_MODULE_DEPENDENCY_INJECTION \
public:\
static void Push(DependencyInjection::IDependencyInjector* _pFAP, const void* _pData);
#define Engine_EntityFactory_h_28_REFLECTION_MODULE_DUMMY 
#define Engine_EntityFactory_h_28_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<EntityFactory*>(_pObject); }
#define Engine_EntityFactory_h_28_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define Engine_EntityFactory_h_28_GENERATED_BODY \
Engine_EntityFactory_h_28_REFLECTION_MODULE_DEPENDENCY_INJECTION \
Engine_EntityFactory_h_28_REFLECTION_MODULE_DUMMY \
Engine_EntityFactory_h_28_REFLECTION_MODULE_CASTER \
Engine_EntityFactory_h_28_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Engine_EntityFactory_h

