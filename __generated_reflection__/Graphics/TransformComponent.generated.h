#include "Serializer/Runtime/ISerializer.h"

#include "RuntimeReflection/Markers.h"

#define Graphics_TransformComponent_h_23_REFLECTION_MODULE_DUMMY 
#define Graphics_TransformComponent_h_23_REFLECTION_MODULE_CASTER \
public: \
static void* Cast(void* _pObject) { return static_cast<TransformComponent*>(_pObject); }
#define Graphics_TransformComponent_h_23_REFLECTION_MODULE_RUNTIME_COMPILER \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_position", m_position); \
_pSerializer->SerializeProperty("m_rotation", m_rotation); \
_pSerializer->SerializeProperty("m_scale", m_scale); \
}
#include "Utils/EntitySerializer.h"
#include "Utils/ECS.h"
#define Graphics_TransformComponent_h_23_REFLECTION_MODULE_COMPONENT \
static void Serialize(nlohmann::json& _jOut, const Entity& _entity)\
{\
Utils::init_ecs();\
Utils::serialize_component<TransformComponent>(_jOut, _entity);\
}\
\
static void Deserialize(const nlohmann::json& _jIn, Entity& _entity)\
{\
Utils::init_ecs();\
Utils::deserialize_component<TransformComponent>(_jIn, _entity);\
}
#define Graphics_TransformComponent_h_23_GENERATED_BODY \
Graphics_TransformComponent_h_23_REFLECTION_MODULE_DUMMY \
Graphics_TransformComponent_h_23_REFLECTION_MODULE_CASTER \
Graphics_TransformComponent_h_23_REFLECTION_MODULE_RUNTIME_COMPILER \
Graphics_TransformComponent_h_23_REFLECTION_MODULE_COMPONENT

#undef DUCKVIL_CURRENT_FILE_ID
#define DUCKVIL_CURRENT_FILE_ID Graphics_TransformComponent_h

