#include "Serializer/EntitySerializerSystem.h"

#include "Engine/UUIDComponent.h"

#include "Graphics/TransformComponent.h"

#include "Utils/Function.h"

#include <fstream>

namespace Duckvil { namespace Serializer {

    template <typename ComponentType>
    static void SerializeComponent(nlohmann::json& _jOut, const Entity& _entity)
    {
        if(_entity.Has<ComponentType>())
        {
            const auto& _typeHandle = RuntimeReflection::get_type<ComponentType>();
            const RuntimeReflection::__type_t& _type = RuntimeReflection::get_type(_typeHandle);
            const auto& _combined = RuntimeReflection::combine(_type);

            const ComponentType& _component = _entity.Get<ComponentType>();

            nlohmann::json _jComponent;
            std::vector<uint8_t> _data;

            _data << _component;

            _jComponent["name"] = _combined;
            _jComponent["data"] = _data;

            _jOut.push_back(_jComponent);
        }
    }

    static void SerializeEntity(nlohmann::json& _jOut, const Entity& _entity)
    {
        const auto& _types = RuntimeReflection::get_types();

        for(uint32_t _i = 0; _i < _types.Size(); ++_i)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<nlohmann::json&, const Entity&>({ _i }, "Serialize");

            if(_funcHandle.m_ID == -1)
            {
                continue;
            }

            RuntimeReflection::invoke_static<nlohmann::json&, const Entity&>({ _i }, _funcHandle, _jOut, _entity);
        }
    }

    nlohmann::json EntitySerializerSystem::Serialize(const Entity& _entity)
    {
        nlohmann::json _jEntity;

        _jEntity["components"] = nlohmann::json::array();

        const UUIDComponent& _uuid = _entity.Get<UUIDComponent>();
        std::vector<char> _bytes(16);

        _uuid.m_uuid.GetBytes(&_bytes[0]);

        _jEntity["uuid"] = _bytes;

        SerializeEntity(_jEntity["components"], _entity);

        return _jEntity;
    }

    template <typename ComponentType>
    static void DeserializeComponent(const nlohmann::json& _jIn, Entity& _entity)
    {
        const auto& _typeHandle = RuntimeReflection::get_type<ComponentType>();
        const RuntimeReflection::__type_t& _type = RuntimeReflection::get_type(_typeHandle);
        const auto& _combined = RuntimeReflection::combine(_type);

        if(_jIn["name"].get<std::string>() == _combined)
        {
            ComponentType _component;

            auto _a = _jIn["data"].get<std::vector<uint8_t>>();

            _a >> _component;

            _entity.Add(
                _component
            );
        }
    }

    static void DeserializeEntity(const nlohmann::json& _jIn, Entity& _entity)
    {
        const auto& _types = RuntimeReflection::get_types();

        for(uint32_t _i = 0; _i < _types.Size(); ++_i)
        {
            const auto& _funcHandle = RuntimeReflection::get_function_handle<const nlohmann::json&, Entity&>({ _i }, "Deserialize");

            if(_funcHandle.m_ID == -1)
            {
                continue;
            }

            RuntimeReflection::invoke_static<const nlohmann::json&, Entity&>({ _i }, _funcHandle, _jIn, _entity);
        }
    }

    void EntitySerializerSystem::Deserialize(const nlohmann::json& _jEntity, Entity& _entity)
    {
        std::vector<char> _uuidBytes(16);
        _jEntity["uuid"].get_to(_uuidBytes);
        UUID _uuid(_uuidBytes.data());

        _entity.Add(
            UUIDComponent{ _uuid }
        );

        for(const auto& _jComponent : _jEntity["components"])
        {
            DeserializeEntity(_jComponent, _entity);
        }
    }

    EntitySerializerSystem::EntitySerializerSystem(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEngineEventPool, Event::Pool<Event::mode::immediate>* _pEntityFactoryEventPool, EntityFactory* _pEntityFactory) :
        m_pEngineEventPool(_pEngineEventPool),
        m_pEntityFactoryEventPool(_pEntityFactoryEventPool),
        m_pEntityFactory(_pEntityFactory)
    {
        _heap.Allocate(m_entities, 1);

        _pEntityFactoryEventPool->AddA<EntityCreatedEvent>(
            [this](const EntityCreatedEvent& _event)
            {
                if(m_entities.Full())
                {
                    m_entities.Resize(m_entities.Size() * 2);
                }

                m_entities.Allocate(_event.m_entity);
            }
        );

        ecs_os_set_api_defaults();
    }

    EntitySerializerSystem::~EntitySerializerSystem()
    {

    }

    bool EntitySerializerSystem::Init()
    {
        return true;
    }

    void EntitySerializerSystem::Update(double _dDelta)
    {

    }

    void EntitySerializerSystem::Load(const std::filesystem::path& _sFile, void (*_fn)(Entity& _entity))
    {
        std::ifstream _jFile(_sFile);

        _jFile >> m_j;

        _jFile.close();

        for(const auto& _jEntity : m_j)
        {
            Entity _entity = m_pEntityFactory->Make();

            Deserialize(_jEntity, _entity);

            if(_fn)
            {
                _fn(_entity);
            }
        }
    }

    void EntitySerializerSystem::Save(const std::filesystem::path& _sFile)
    {
        for(uint32_t _i = 0; _i < m_entities.Size(); ++_i)
        {
            const auto& _e = m_entities[_i];
            nlohmann::json _jEntity = Serialize(_e);

            m_j.push_back(_jEntity);
        }

        std::ofstream _jFile(_sFile, std::ios::trunc);

        _jFile << m_j;

        _jFile.close();
    }

}}