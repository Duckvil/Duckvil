#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Editor_Widgets_ContentExplorerWidget_h_15_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ContentExplorerWidget*>(_pObject); }
#define Editor_Widgets_ContentExplorerWidget_h_15_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pEngineEventPool", m_pEngineEventPool); \
_pSerializer->SerializeProperty("m_sPath", m_sPath); \
_pSerializer->SerializeProperty("m_bLoaded", m_bLoaded); \
_pSerializer->SerializeProperty("m_bCreateDirectoryPopup", m_bCreateDirectoryPopup); \
_pSerializer->SerializeProperty("m_sSelectedPath", m_sSelectedPath); \
}
#define Editor_Widgets_ContentExplorerWidget_h_15_GENERATED_BODY \
Editor_Widgets_ContentExplorerWidget_h_15_REFLECTION_MODULE_CASTER \
Editor_Widgets_ContentExplorerWidget_h_15_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Editor_Widgets_ContentExplorerWidget_h
