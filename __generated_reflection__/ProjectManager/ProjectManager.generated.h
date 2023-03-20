#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_DUMMY 
#define ProjectManager_ProjectManager_h_64_REFLECTION_MODULE_DUMMY 
#define ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<data*>(_pObject); }
#define ProjectManager_ProjectManager_h_64_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ftable*>(_pObject); }
#define ProjectManager_ProjectManager_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define ProjectManager_ProjectManager_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
}
#define ProjectManager_ProjectManager_h_37_GENERATED_BODY \
ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_DUMMY \
ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_CASTER

#define ProjectManager_ProjectManager_h_64_GENERATED_BODY \
ProjectManager_ProjectManager_h_64_REFLECTION_MODULE_DUMMY \
ProjectManager_ProjectManager_h_64_REFLECTION_MODULE_CASTER

#define ProjectManager_ProjectManager_h_0_GENERATED_BODY \
ProjectManager_ProjectManager_h_0_REFLECTION_MODULE_RUNTIME_COMPILER \
ProjectManager_ProjectManager_h_0_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID ProjectManager_ProjectManager_h

