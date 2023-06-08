#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Engine_UUIDComponent_h_14_REFLECTION_MODULE_DUMMY 
#define Engine_UUIDComponent_h_14_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<UUIDComponent*>(_pObject); }
#define Engine_UUIDComponent_h_14_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_uuid", m_uuid); \
}
#include "Utils/EntitySerializer.h"
#include "Utils/ECS.h"
#include "ECS/Entity.h"
#define Engine_UUIDComponent_h_14_REFLECTION_MODULE_COMPONENT \
static void Serialize(nlohmann::json& _jOut, const ECS::Entity& _entity)\
{\
Utils::init_ecs();\
Utils::serialize_component<UUIDComponent>(_jOut, _entity);\
}\
\
static void Deserialize(const nlohmann::json& _jIn, ECS::Entity& _entity)\
{\
Utils::init_ecs();\
Utils::deserialize_component<UUIDComponent>(_jIn, _entity);\
}\
static bool Has(const ECS::Entity& _entity)\
{\
Utils::init_ecs();\
return _entity.Has<UUIDComponent>();\
}\
\
static uint8_t* Get(const ECS::Entity& _entity)\
{\
Utils::init_ecs();\
return reinterpret_cast<uint8_t*>(_entity.m_entity.get_mut<UUIDComponent>());\
}
#define Engine_UUIDComponent_h_14_GENERATED_BODY \
Engine_UUIDComponent_h_14_REFLECTION_MODULE_DUMMY \
Engine_UUIDComponent_h_14_REFLECTION_MODULE_CASTER \
Engine_UUIDComponent_h_14_REFLECTION_MODULE_RUNTIME_COMPILER \
Engine_UUIDComponent_h_14_REFLECTION_MODULE_COMPONENT

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Engine_UUIDComponent_h

