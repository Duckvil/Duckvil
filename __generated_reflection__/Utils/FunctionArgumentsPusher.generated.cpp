#include "Utils/FunctionArgumentsPusher.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
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
_namespaces.push_back("FunctionArgumentsPusher");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::FunctionArgumentsPusher>(_data, "FunctionArgumentsPusher");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
record_inheritance<Xbyak::CodeGenerator>(_data, _type, __protection::__protection_public);
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_uiIndex), "m_uiIndex");
_property = record_property<bool[4]>(_data, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_aFloats), "m_aFloats");
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::FunctionArgumentsPusher, m_uiCount), "m_uiCount");
_constructor = record_constructor<Duckvil::FunctionArgumentsPusher, uint32_t>(_data, _type);
_destructor = record_destructor<Duckvil::FunctionArgumentsPusher>(_data, _type);
_function = record_function<Duckvil::FunctionArgumentsPusher, void, int const&>(_data, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, size_t const&>(_data, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, float const&>(_data, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, double const&>(_data, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, bool const&>(_data, _type, &Duckvil::FunctionArgumentsPusher::Push, "Push");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, void const*>(_data, _type, &Duckvil::FunctionArgumentsPusher::Call, "Call");
_function = record_function<Duckvil::FunctionArgumentsPusher, void, void const**>(_data, _type, &Duckvil::FunctionArgumentsPusher::Call, "Call");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/FunctionArgumentsPusher.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
