#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define HotReloader_Network_HotObjectSync_h_23_REFLECTION_MODULE_DUMMY 
#define HotReloader_Network_HotObjectSync_h_23_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<HotObjectSync*>(_pObject); }
#define HotReloader_Network_HotObjectSync_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define HotReloader_Network_HotObjectSync_h_23_GENERATED_BODY \
HotReloader_Network_HotObjectSync_h_23_REFLECTION_MODULE_DUMMY \
HotReloader_Network_HotObjectSync_h_23_REFLECTION_MODULE_CASTER

#define HotReloader_Network_HotObjectSync_h_0_GENERATED_BODY \
HotReloader_Network_HotObjectSync_h_0_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID HotReloader_Network_HotObjectSync_h

