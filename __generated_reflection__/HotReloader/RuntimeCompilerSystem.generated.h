#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<reflection_module*>(_pObject); } \
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
_pSerializer->SerializeProperty("m_pReflectionGenerator", m_pReflectionGenerator); \
_pSerializer->SerializeProperty("m_pAST_FTable", m_pAST_FTable); \
_pSerializer->SerializeProperty("m_pLexerFTable", m_pLexerFTable); \
_pSerializer->SerializeProperty("m_pThread", m_pThread); \
_pSerializer->SerializeProperty("m_pThreadData", m_pThreadData); \
_pSerializer->SerializeProperty("m_aRecordedTypes", m_aRecordedTypes); \
_pSerializer->SerializeProperty("m_aHotObjects", m_aHotObjects); \
_pSerializer->SerializeProperty("m_aModules", m_aModules); \
_pSerializer->SerializeProperty("m_pRuntimeCompiler", m_pRuntimeCompiler); \
_pSerializer->SerializeProperty("m_pObject", m_pObject); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
_pSerializer->SerializeProperty("m_generateCustomFunctionHandle", m_generateCustomFunctionHandle); \
_pSerializer->SerializeProperty("m_processAST_FunctionHandle", m_processAST_FunctionHandle); \
_pSerializer->SerializeProperty("m_clearFunctionHandle", m_clearFunctionHandle); \
}
