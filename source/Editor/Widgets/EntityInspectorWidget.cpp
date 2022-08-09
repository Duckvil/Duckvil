#include "Editor/Widgets/EntityInspectorWidget.h"

#include "imgui/imgui.h"

#include "Engine/UUIDComponent.h"

#include "Graphics/TransformComponent.h"

#include "glm/gtx/quaternion.hpp"

#include "RuntimeReflection/Meta.h"

namespace Duckvil { namespace Editor {

    static void draw_vec3(const char* _sLabel, glm::vec3& _value)
    {
        ImGui::DragFloat3(_sLabel, &_value[0], 0.1f);
    }

    EntityInspectorWidget::EntityInspectorWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEditorEventPool) :
        m_heap(_heap)
    {
        _pEditorEventPool->Add(this, &EntityInspectorWidget::OnEvent);

        _heap.Allocate(m_aComponentTypeHandles, 1);
    }

    EntityInspectorWidget::~EntityInspectorWidget()
    {

    }

    void EntityInspectorWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));

        const auto& _types = RuntimeReflection::get_types();

        for(uint32_t _i = 0; _i < _types.Size(); _i++)
        {
            const auto& _type = _types[_i];
            const auto& _meta = RuntimeReflection::get_meta(_type, "Component");

            if(_meta.m_ullTypeID != -1 && _meta.m_pData != nullptr)
            {
                m_aComponentTypeHandles.Allocate(_type);
            }
        }
    }

    void EntityInspectorWidget::OnDraw()
    {
        ImGui::Begin("Entity inspector");

        if(m_selectedEntity.m_entity != 0)
        {
            if(m_selectedEntity.Has<UUIDComponent>())
            {
                ImGui::Text("UUID: %llu", m_selectedEntity.Get<UUIDComponent>().m_uuid.Hash());
            }
            else
            {
                ImGui::Text("UUID: NONE");
            }

            for(uint32_t _i = 0; _i < m_aComponentTypeHandles.Size(); _i++)
            {
                const auto& _typeHandle = m_aComponentTypeHandles[_i];
                const auto& _props = RuntimeReflection::get_properties(m_heap, _typeHandle);
                const RuntimeReflection::__type_t& _type = RuntimeReflection::get_type(_typeHandle);

                const auto& _hasFuncHandle = RuntimeReflection::get_function_handle<const Entity&>(_typeHandle, "Has");
                bool _has = RuntimeReflection::invoke_static_result<bool, const Entity&>(_typeHandle, _hasFuncHandle, m_selectedEntity);

                if(!_has)
                {
                    continue;
                }

                if(ImGui::TreeNodeEx(_type.m_sTypeName, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen, _type.m_sTypeName))
                {
                    const auto& _getFuncHandle = RuntimeReflection::get_function_handle<const Entity&>(_typeHandle, "Get");
                    uint8_t* _componentAddress = RuntimeReflection::invoke_static_result<uint8_t*, const Entity&>(_typeHandle, _getFuncHandle, m_selectedEntity);

                    for(uint32_t _j = 0; _j < _props.Size(); _j++)
                    {
                        const auto& _propHandle = _props[_j];
                        const auto& _prop = RuntimeReflection::get_property(_typeHandle, _propHandle);

                        if(_prop.m_ullTypeID == typeid(glm::vec3).hash_code())
                        {
                            uint8_t* _address = _componentAddress + _prop.m_ullAddress;

                            draw_vec3(_prop.m_sName, *reinterpret_cast<glm::vec3*>(_address));
                        }
                        else if(_prop.m_ullTypeID == typeid(glm::quat).hash_code())
                        {
                            uint8_t* _address = _componentAddress + _prop.m_ullAddress;

                            auto& _quat = *reinterpret_cast<glm::quat*>(_address);
                            glm::vec3 _rotation = glm::eulerAngles(_quat);

                            _rotation = glm::degrees(_rotation);

                            draw_vec3(_prop.m_sName, _rotation);

                            _rotation = glm::radians(_rotation);

                            _quat = glm::quat(_rotation);
                        }
                    }

                    ImGui::TreePop();
                }
            }
        }

        ImGui::End();
    }

    void EntityInspectorWidget::OnEvent(const EntitySelectedEvent& _event)
    {
        m_selectedEntity = _event.m_entity;
    }

}}