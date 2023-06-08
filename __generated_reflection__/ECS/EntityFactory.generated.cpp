#include "ECS/EntityFactory.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(1)
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
_namespaces.push_back("ECS");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("EntityFactory");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::ECS;
_type = record_type<Duckvil::ECS::EntityFactory>(_data, "EntityFactory");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "ECS");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 1);
record_meta(_data, _type, Duckvil::DependencyInjection::INJECTABLE, true);
record_meta(_data, _type, Duckvil::HotReloader::NOT_SERIALIZABLE, true);
_constructor = record_constructor<Duckvil::ECS::EntityFactory, Memory::FreeList const&, Event::Pool<Event::mode::immediate>*, flecs::world&>(_data, _type);
_destructor = record_destructor<Duckvil::ECS::EntityFactory>(_data, _type);
_function = record_function<Duckvil::ECS::EntityFactory, void>(_data, _type, &Duckvil::ECS::EntityFactory::Init, "Init");
_function = record_function<Duckvil::ECS::EntityFactory, Entity>(_data, _type, &Duckvil::ECS::EntityFactory::Make, "Make");
_function = record_function<Duckvil::ECS::EntityFactory, Entity, void(*)(Entity&)>(_data, _type, &Duckvil::ECS::EntityFactory::Make, "Make");
_function = record_function<Duckvil::ECS::EntityFactory, void, Entity&>(_data, _type, &Duckvil::ECS::EntityFactory::Destroy, "Destroy");
_function = record_function<Entity, flecs::entity const&>(_data, _type, &Duckvil::ECS::EntityFactory::Clone, "Clone");
_function = record_function<Duckvil::ECS::EntityFactory, Entity, size_t>(_data, _type, &Duckvil::ECS::EntityFactory::FromID, "FromID");
_function = record_function<Duckvil::ECS::EntityFactory, bool, Entity const&>(_data, _type, &Duckvil::ECS::EntityFactory::IsValid, "IsValid");
_function = record_function<Duckvil::ECS::EntityFactory, Entity, char const*>(_data, _type, &Duckvil::ECS::EntityFactory::LookupComponent, "LookupComponent");
_function = record_function<Duckvil::ECS::EntityFactory, Event::Pool<Event::mode::immediate>*>(_data, _type, &Duckvil::ECS::EntityFactory::GetEventPool, "GetEventPool");
_function = record_function<void*, void*>(_data, _type, &Duckvil::ECS::EntityFactory::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "ECS/EntityFactory.h", 1);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 1;
}
#endif
