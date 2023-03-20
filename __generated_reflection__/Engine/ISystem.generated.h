#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Engine_ISystem_h_14_REFLECTION_MODULE_DUMMY 
#define Engine_ISystem_h_14_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ISystem*>(_pObject); }
#define Engine_ISystem_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define Engine_ISystem_h_14_GENERATED_BODY \
Engine_ISystem_h_14_REFLECTION_MODULE_DUMMY \
Engine_ISystem_h_14_REFLECTION_MODULE_CASTER

#define Engine_ISystem_h_0_GENERATED_BODY \
Engine_ISystem_h_0_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Engine_ISystem_h

