#include "Engine/EntityFactory.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(2)
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
_namespaces.push_back("EntityFactory");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil;
_type = record_type<Duckvil::EntityFactory>(_data, "EntityFactory");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 2);
_constructor = record_constructor<Duckvil::EntityFactory, Memory::FreeList const&, Event::Pool<Event::mode::immediate>*, flecs::world&>(_data, _type);
_destructor = record_destructor<Duckvil::EntityFactory>(_data, _type);
_function = record_function<Duckvil::EntityFactory, void>(_data, _type, &Duckvil::EntityFactory::Init, "Init");
_function = record_function<Duckvil::EntityFactory, Duckvil::Entity>(_data, _type, &Duckvil::EntityFactory::Make, "Make");
_function = record_function<Duckvil::EntityFactory, Duckvil::Entity, void(*)(Duckvil::Entity&)>(_data, _type, &Duckvil::EntityFactory::Make, "Make");
_function = record_function<Duckvil::EntityFactory, void, Duckvil::Entity&>(_data, _type, &Duckvil::EntityFactory::Destroy, "Destroy");
_function = record_function<Duckvil::Entity, flecs::entity const&>(_data, _type, &Duckvil::EntityFactory::Clone, "Clone");
_function = record_function<Duckvil::EntityFactory, Duckvil::Entity, size_t>(_data, _type, &Duckvil::EntityFactory::FromID, "FromID");
_function = record_function<Duckvil::EntityFactory, bool, Duckvil::Entity const&>(_data, _type, &Duckvil::EntityFactory::IsValid, "IsValid");
_function = record_function<Duckvil::EntityFactory, Duckvil::Entity, char const*>(_data, _type, &Duckvil::EntityFactory::LookupComponent, "LookupComponent");
_function = record_function<Duckvil::EntityFactory, Event::Pool<Event::mode::immediate>*>(_data, _type, &Duckvil::EntityFactory::GetEventPool, "GetEventPool");
_namespaces.pop_back();
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Engine/EntityFactory.h", 2);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 2;
}
#endif
