#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Engine_ScriptComponent_h_67_REFLECTION_MODULE_DUMMY 
#define Engine_ScriptComponent_h_67_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<ScriptComponent*>(_pObject); }
#define Engine_ScriptComponent_h_67_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pScripts", m_pScripts); \
}
#include "Utils/EntitySerializer.h"
#include "Utils/ECS.h"
#include "Engine/Entity.h"
#define Engine_ScriptComponent_h_67_REFLECTION_MODULE_COMPONENT \
static void Serialize(nlohmann::json& _jOut, const Entity& _entity)\
{\
Utils::init_ecs();\
Utils::serialize_component<ScriptComponent>(_jOut, _entity);\
}\
\
static void Deserialize(const nlohmann::json& _jIn, Entity& _entity)\
{\
Utils::init_ecs();\
Utils::deserialize_component<ScriptComponent>(_jIn, _entity);\
}\
static bool Has(const Entity& _entity)\
{\
Utils::init_ecs();\
return _entity.Has<ScriptComponent>();\
}\
\
static uint8_t* Get(const Entity& _entity)\
{\
Utils::init_ecs();\
return reinterpret_cast<uint8_t*>(_entity.m_entity.get_mut<ScriptComponent>());\
}
#define Engine_ScriptComponent_h_67_GENERATED_BODY \
Engine_ScriptComponent_h_67_REFLECTION_MODULE_DUMMY \
Engine_ScriptComponent_h_67_REFLECTION_MODULE_CASTER \
Engine_ScriptComponent_h_67_REFLECTION_MODULE_RUNTIME_COMPILER \
Engine_ScriptComponent_h_67_REFLECTION_MODULE_COMPONENT

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Engine_ScriptComponent_h

