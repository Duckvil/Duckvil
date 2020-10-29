#include "RuntimeReflection\Recorder.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::RuntimeReflection;
_type = record_type<Duckvil::RuntimeReflection::__recorder_meta_info>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__recorder_meta_info");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeReflection");
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
}
