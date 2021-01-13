#include "RuntimeReflection\Recorder.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(3)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
_type = record_type<duckvil_recorderd_types>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "duckvil_recorderd_types");
_recordedTypes.push_back(_type);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<Duckvil::RuntimeReflection::__duckvil_resource_type_t*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_aTypes), "m_aTypes");
_property = record_property<size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_ullCount), "m_ullCount");
_property = record_property<const char*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_sFile), "m_sFile");
_property = record_property<uint32_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(duckvil_recorderd_types, m_uiRecorderID), "m_uiRecorderID");
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__recorder_meta_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__recorder_meta_info");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 3);
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeID), "m_ullKeyTypeID");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullKeyTypeSize), "m_ullKeyTypeSize");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucKeyTypeAlignment), "m_ucKeyTypeAlignment");
_property = record_property<__traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_keyTraits), "m_keyTraits");
_property = record_property<const void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pKeyData), "m_pKeyData");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeID), "m_ullValueTypeID");
_property = record_property<std::size_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ullValueTypeSize), "m_ullValueTypeSize");
_property = record_property<uint8_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_ucValueTypeAlignment), "m_ucValueTypeAlignment");
_property = record_property<__traits>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_valueTraits), "m_valueTraits");
_property = record_property<const void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeReflection::__recorder_meta_info, m_pValueData), "m_pValueData");
Duckvil::RuntimeReflection::__duckvil_resource_type_t* _types = new Duckvil::RuntimeReflection::__duckvil_resource_type_t[_recordedTypes.size()];
for(size_t i = 0; i < _recordedTypes.size(); i++) { _types[i] = _recordedTypes[i]; }
return duckvil_recorderd_types { _types, _recordedTypes.size(), "RuntimeReflection/Recorder.h", 3 };
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 3;
}
#endif
