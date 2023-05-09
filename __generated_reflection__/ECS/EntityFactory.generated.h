#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define ECS_EntityFactory_h_28_REFLECTION_MODULE_DUMMY 
#define ECS_EntityFactory_h_28_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<EntityFactory*>(_pObject); }
#define ECS_EntityFactory_h_28_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define ECS_EntityFactory_h_28_GENERATED_BODY \
ECS_EntityFactory_h_28_REFLECTION_MODULE_DUMMY \
ECS_EntityFactory_h_28_REFLECTION_MODULE_CASTER \
ECS_EntityFactory_h_28_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID ECS_EntityFactory_h

