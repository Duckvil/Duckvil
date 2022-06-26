#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define RayTracer_RayTracer_h_47_REFLECTION_MODULE_DUMMY 
#define RayTracer_RayTracer_h_47_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<RayTracer*>(_pObject); }
#define RayTracer_RayTracer_h_47_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("image_width", image_width); \
_pSerializer->SerializeProperty("image_height", image_height); \
_pSerializer->SerializeProperty("image_data", image_data); \
_pSerializer->SerializeProperty("image_texture", image_texture); \
_pSerializer->SerializeProperty("_module", _module); \
_pSerializer->SerializeProperty("_kernelModule", _kernelModule); \
_pSerializer->SerializeProperty("m_pDeviceMemory", m_pDeviceMemory); \
_pSerializer->SerializeProperty("elapsed_time_ms", elapsed_time_ms); \
_pSerializer->SerializeProperty("elapsed_time_ms2", elapsed_time_ms2); \
_pSerializer->SerializeProperty("elapsed_time_ms3", elapsed_time_ms3); \
_pSerializer->SerializeProperty("m_dpObjectStream", m_dpObjectStream); \
_pSerializer->SerializeProperty("m_vObjectData", m_vObjectData); \
_pSerializer->SerializeProperty("m_dpLightStream", m_dpLightStream); \
_pSerializer->SerializeProperty("m_vLightData", m_vLightData); \
_pSerializer->SerializeProperty("m_camera", m_camera); \
_pSerializer->SerializeProperty("m_dpCamera", m_dpCamera); \
_pSerializer->SerializeProperty("m_bRender", m_bRender); \
_pSerializer->SerializeProperty("m_f3Object1Position", m_f3Object1Position); \
_pSerializer->SerializeProperty("m_pObject1Offset", m_pObject1Offset); \
_pSerializer->SerializeProperty("m_f3CameraUp", m_f3CameraUp); \
_pSerializer->SerializeProperty("m_f3CameraForward", m_f3CameraForward); \
_pSerializer->SerializeProperty("m_f3CameraPosition", m_f3CameraPosition); \
_pSerializer->SerializeProperty("m_pWindowEventPool", m_pWindowEventPool); \
_pSerializer->SerializeProperty("m_setMousePosition", m_setMousePosition); \
_pSerializer->SerializeProperty("m_fCameraUp", m_fCameraUp); \
_pSerializer->SerializeProperty("m_fCameraRight", m_fCameraRight); \
ISystem::Serialize(_pSerializer); \
}
#define RayTracer_RayTracer_h_47_GENERATED_BODY \
RayTracer_RayTracer_h_47_REFLECTION_MODULE_DUMMY \
RayTracer_RayTracer_h_47_REFLECTION_MODULE_CASTER \
RayTracer_RayTracer_h_47_REFLECTION_MODULE_RUNTIME_COMPILER

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID RayTracer_RayTracer_h

