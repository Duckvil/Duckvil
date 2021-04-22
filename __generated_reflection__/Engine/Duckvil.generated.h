#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<__ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_type", m_type); \
_pSerializer->SerializeProperty("m_pTrackKeeper", m_pTrackKeeper); \
_pSerializer->SerializeProperty("m_bIsHot", m_bIsHot); \
_pSerializer->SerializeProperty("m_fnUpdateCallback", m_fnUpdateCallback); \
_pSerializer->SerializeProperty("m_fnInitCallback", m_fnInitCallback); \
_pSerializer->SerializeProperty("m_bRunning", m_bRunning); \
_pSerializer->SerializeProperty("m_pMemory", m_pMemory); \
_pSerializer->SerializeProperty("m_pHeap", m_pHeap); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_objectsHeap", m_objectsHeap); \
_pSerializer->SerializeProperty("m_eventsHeap", m_eventsHeap); \
_pSerializer->SerializeProperty("m_ullLastTimeUsed", m_ullLastTimeUsed); \
_pSerializer->SerializeProperty("_loggerFTable", _loggerFTable); \
_pSerializer->SerializeProperty("_loggerData", _loggerData); \
_pSerializer->SerializeProperty("m_pRuntimeReflection", m_pRuntimeReflection); \
_pSerializer->SerializeProperty("m_pRuntimeReflectionRecorder", m_pRuntimeReflectionRecorder); \
_pSerializer->SerializeProperty("m_pRuntimeReflectionData", m_pRuntimeReflectionData); \
_pSerializer->SerializeProperty("m_aRecordedTypes", m_aRecordedTypes); \
_pSerializer->SerializeProperty("m_aLoadedModules", m_aLoadedModules); \
_pSerializer->SerializeProperty("m_uiLoadedModulesCount", m_uiLoadedModulesCount); \
_pSerializer->SerializeProperty("m_pRuntimeCompiler", m_pRuntimeCompiler); \
_pSerializer->SerializeProperty("m_fnRuntimeCompilerUpdate", m_fnRuntimeCompilerUpdate); \
_pSerializer->SerializeProperty("m_fnRuntimeCompilerInit", m_fnRuntimeCompilerInit); \
_pSerializer->SerializeProperty("m_pLoggerChannelData", m_pLoggerChannelData); \
_pSerializer->SerializeProperty("m_pLoggerChannel", m_pLoggerChannel); \
_pSerializer->SerializeProperty("m_pThread", m_pThread); \
_pSerializer->SerializeProperty("m_pThreadData", m_pThreadData); \
_pSerializer->SerializeProperty("m_time", m_time); \
_pSerializer->SerializeProperty("m_timeData", m_timeData); \
_pSerializer->SerializeProperty("m_dOneSecond", m_dOneSecond); \
_pSerializer->SerializeProperty("m_eventPool", m_eventPool); \
_pSerializer->SerializeProperty("m_windowEventPool", m_windowEventPool); \
_pSerializer->SerializeProperty("m_aEngineSystems", m_aEngineSystems); \
_pSerializer->SerializeProperty("m_pWindow", m_pWindow); \
_pSerializer->SerializeProperty("m_pEditor", m_pEditor); \
_pSerializer->SerializeProperty("m_pEditorData", m_pEditorData); \
}
