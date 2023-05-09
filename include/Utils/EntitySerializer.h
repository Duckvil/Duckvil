#pragma once

#include "nlohmann/json.hpp"

#include "ECS/Entity.h"

#include "RuntimeReflection/RuntimeReflection.h"

namespace Duckvil { namespace Utils {

    template <typename DataType>
    std::vector<uint8_t>& operator<<(std::vector<uint8_t>& _message, const DataType& _data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t _s = _message.size();

        _message.resize(_s + sizeof(DataType));

        std::memcpy(_message.data() + _s, &_data, sizeof(DataType));

        return _message;
    }

    template <typename DataType>
    std::vector<uint8_t>& operator>>(std::vector<uint8_t>& _message, DataType& _data)
    {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t _s = _message.size() - sizeof(DataType);

        std::memcpy(&_data, _message.data() + _s, sizeof(DataType));

        _message.resize(_s);

        return _message;
    }

    template <typename ComponentType>
    void serialize_component(nlohmann::json& _jOut, const ECS::Entity& _entity)
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

    template <typename ComponentType>
    void deserialize_component(const nlohmann::json& _jIn, ECS::Entity& _entity)
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

}}