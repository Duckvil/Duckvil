#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Graphics_TransformComponent_h_25_REFLECTION_MODULE_DUMMY 
#define Graphics_TransformComponent_h_25_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<TransformComponent*>(_pObject); }
#define Graphics_TransformComponent_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define Graphics_TransformComponent_h_25_GENERATED_BODY \
Graphics_TransformComponent_h_25_REFLECTION_MODULE_DUMMY \
Graphics_TransformComponent_h_25_REFLECTION_MODULE_CASTER

#define Graphics_TransformComponent_h_0_GENERATED_BODY \
Graphics_TransformComponent_h_0_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Graphics_TransformComponent_h

