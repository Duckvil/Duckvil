#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_DUMMY 
#define HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<RuntimeCompilerSystem*>(_pObject); }
#define HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pFileWatcher", m_pFileWatcher); \
_pSerializer->SerializeProperty("m_userData", m_userData); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_objectsHeap", m_objectsHeap); \
_pSerializer->SerializeProperty("m_compilerTypeHandle", m_compilerTypeHandle); \
_pSerializer->SerializeProperty("m_sModuleName", m_sModuleName); \
_pSerializer->SerializeProperty("m_pCompiler", m_pCompiler); \
_pSerializer->SerializeProperty("m_pEventPool", m_pEventPool); \
_pSerializer->SerializeProperty("m_pRuntimeReflectionEventPool", m_pRuntimeReflectionEventPool); \
_pSerializer->SerializeProperty("m_pThread", m_pThread); \
_pSerializer->SerializeProperty("m_pThreadData", m_pThreadData); \
_pSerializer->SerializeProperty("m_aLoadedModules", m_aLoadedModules); \
_pSerializer->SerializeProperty("m_aReflectedTypes", m_aReflectedTypes); \
_pSerializer->SerializeProperty("m_fnInternalCompilerSetup", m_fnInternalCompilerSetup); \
_pSerializer->SerializeProperty("m_fnInternalCompilerAddFlag2", m_fnInternalCompilerAddFlag2); \
_pSerializer->SerializeProperty("m_eventPool", m_eventPool); \
_pSerializer->SerializeProperty("m_CWD", m_CWD); \
_pSerializer->SerializeProperty("m_bIsSingleModule", m_bIsSingleModule); \
_pSerializer->SerializeProperty("m_pDependencyResolver", m_pDependencyResolver); \
_pSerializer->SerializeProperty("m_aHotObjects", m_aHotObjects); \
_pSerializer->SerializeProperty("m_aModules", m_aModules); \
_pSerializer->SerializeProperty("m_path", m_path); \
ISystem::Serialize(_pSerializer); \
}
#define HotReloader_RuntimeCompilerSystem_h_67_GENERATED_BODY \
HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_DUMMY \
HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_CASTER \
HotReloader_RuntimeCompilerSystem_h_67_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID HotReloader_RuntimeCompilerSystem_h

