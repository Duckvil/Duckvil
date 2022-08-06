#include "Serializer/EntitySerializerSystem.h"
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
_namespaces.push_back("Serializer");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("EntitySerializerSystem");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Serializer;
_type = record_type<Duckvil::Serializer::EntitySerializerSystem>(_data, "EntitySerializerSystem");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Serializer");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
record_meta(_data, _type, ReflectionFlags_AutoEventsAdding, true);
record_meta(_data, _type, Duckvil::ReflectionFlags_AutoInstantiate, false);
record_inheritance<ISystem>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Serializer::EntitySerializerSystem, const Memory::FreeList&, Event::Pool<Event::mode::immediate>*, Event::Pool<Event::mode::immediate>*, EntityFactory*>(_data, _type);
record_meta(_data, _type, _constructor, 1, "Engine", true);
record_meta(_data, _type, _constructor, 2, "EntityFactory", true);
_destructor = record_destructor<Duckvil::Serializer::EntitySerializerSystem>(_data, _type);
_function = record_function<Duckvil::Serializer::EntitySerializerSystem, bool>(_data, _type, &Duckvil::Serializer::EntitySerializerSystem::Init, "Init");
_function = record_function<Duckvil::Serializer::EntitySerializerSystem, void, double>(_data, _type, &Duckvil::Serializer::EntitySerializerSystem::Update, "Update");
_function = record_function<Duckvil::Serializer::EntitySerializerSystem, void, const std::filesystem::path&>(_data, _type, &Duckvil::Serializer::EntitySerializerSystem::Load, "Load");
_function = record_function<Duckvil::Serializer::EntitySerializerSystem, void, const std::filesystem::path&>(_data, _type, &Duckvil::Serializer::EntitySerializerSystem::Save, "Save");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Serializer/EntitySerializerSystem.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
