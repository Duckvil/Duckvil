#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_DUMMY 
#define ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_DUMMY 
#define ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<data*>(_pObject); }
#define ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ftable*>(_pObject); }
#define ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_pRCS", m_pRCS); \
_pSerializer->SerializeProperty("m_fnUpdateProject", m_fnUpdateProject); \
_pSerializer->SerializeProperty("m_pEngineEventPool", m_pEngineEventPool); \
_pSerializer->SerializeProperty("m_projectManagerEventPool", m_projectManagerEventPool); \
_pSerializer->SerializeProperty("m_loadedProject", m_loadedProject); \
_pSerializer->SerializeProperty("m_bLoaded", m_bLoaded); \
_pSerializer->SerializeProperty("m_dOneSecond", m_dOneSecond); \
_pSerializer->SerializeProperty("m_pRuntimeCompilerSystem", m_pRuntimeCompilerSystem); \
_pSerializer->SerializeProperty("m_fnRuntimeCompilerUpdate", m_fnRuntimeCompilerUpdate); \
_pSerializer->SerializeProperty("m_objectsHeap", m_objectsHeap); \
_pSerializer->SerializeProperty("m_pDependencyResolver", m_pDependencyResolver); \
_pSerializer->SerializeProperty("m_bSimulating", m_bSimulating); \
_pSerializer->SerializeProperty("m_bPaused", m_bPaused); \
}
#define ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_fnUpdate", m_fnUpdate); \
}
#define ProjectManager_ProjectManager_h_37_GENERATED_BODY \
ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_DUMMY \
ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_CASTER \
ProjectManager_ProjectManager_h_37_REFLECTION_MODULE_RUNTIME_COMPILER

#define ProjectManager_ProjectManager_h_69_GENERATED_BODY \
ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_DUMMY \
ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_CASTER \
ProjectManager_ProjectManager_h_69_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID ProjectManager_ProjectManager_h

