#include "RayTracer/Objects.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Graphics");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("RayTracer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("Camera");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Graphics::RayTracer;
_type = record_type<Duckvil::Graphics::RayTracer::Camera>(_data, "Camera");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3Position), "m_f3Position");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3LookAt), "m_f3LookAt");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3Up), "m_f3Up");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_fLength), "m_fLength");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_fHorizontalSize), "m_fHorizontalSize");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_fAspectRatio), "m_fAspectRatio");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3AlignmentVector), "m_f3AlignmentVector");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3ProjectionScreenU), "m_f3ProjectionScreenU");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3ProjectionScreenV), "m_f3ProjectionScreenV");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Camera, m_f3ProjectionScreenCentre), "m_f3ProjectionScreenCentre");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Graphics::RayTracer::ObjectType>(_data, _ntype, "ObjectType");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::ObjectType::sphere, "sphere");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::ObjectType::plane, "plane");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::ObjectType::mesh, "mesh");
_enum = record_enum<Duckvil::Graphics::RayTracer::MaterialType>(_data, _ntype, "MaterialType");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::MaterialType::none, "none");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::MaterialType::specular, "specular");
_namespaces.push_back("ObjectStream");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::ObjectStream>(_data, "ObjectStream");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::ObjectStream, m_uiSize), "m_uiSize");
_namespaces.pop_back();
_namespaces.push_back("ObjectChunk");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::ObjectChunk>(_data, "ObjectChunk");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<ObjectType>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::ObjectChunk, m_type), "m_type");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::ObjectChunk, m_uiSize), "m_uiSize");
_property = record_property<MaterialType>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::ObjectChunk, m_material), "m_material");
_namespaces.pop_back();
_namespaces.push_back("Sphere");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::Sphere>(_data, "Sphere");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Sphere, m_f3Color), "m_f3Color");
_namespaces.pop_back();
_namespaces.push_back("Plane");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::Plane>(_data, "Plane");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::Plane, m_f3Color), "m_f3Color");
_namespaces.pop_back();
_namespaces.push_back("SpecularMaterial");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::SpecularMaterial>(_data, "SpecularMaterial");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::SpecularMaterial, m_fReflectivity), "m_fReflectivity");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::SpecularMaterial, m_fShininess), "m_fShininess");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::SpecularMaterial, m_f3Color), "m_f3Color");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::SpecularMaterial, m_fTranslucency), "m_fTranslucency");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::SpecularMaterial, m_fIOR), "m_fIOR");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Graphics::RayTracer::LightType>(_data, _ntype, "LightType");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Graphics::RayTracer::LightType::point_light, "point_light");
_namespaces.push_back("LightStream");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::LightStream>(_data, "LightStream");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::LightStream, m_uiSize), "m_uiSize");
_namespaces.pop_back();
_namespaces.push_back("LightChunk");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::LightChunk>(_data, "LightChunk");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<LightType>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::LightChunk, m_type), "m_type");
_namespaces.pop_back();
_namespaces.push_back("PointLight");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Graphics::RayTracer::PointLight>(_data, "PointLight");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Graphics");
record_namespace(_data, _type, "RayTracer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::PointLight, m_f3Color), "m_f3Color");
_property = record_property<float3>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::PointLight, m_f3Location), "m_f3Location");
_property = record_property<float>(_data, _type, offsetof(Duckvil::Graphics::RayTracer::PointLight, m_fIntensity), "m_fIntensity");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "RayTracer/Objects.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
