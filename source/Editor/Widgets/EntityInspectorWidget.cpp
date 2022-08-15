#include "Editor/Widgets/EntityInspectorWidget.h"

#include "imgui/imgui.h"

#include "Engine/UUIDComponent.h"

#include "Graphics/TransformComponent.h"

#include "glm/gtx/quaternion.hpp"

#include "RuntimeReflection/Meta.h"

#include "Utils/FunctionArgumentsPusher.h"

#include "RuntimeReflection/ReflectedType.h"

namespace Duckvil { namespace Editor {

    static void draw_vec3(const char* _sLabel, glm::vec3& _value)
    {
        ImGui::DragFloat3(_sLabel, &_value[0], 0.1f);
    }

    EntityInspectorWidget::EntityInspectorWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEditorEventPool) :
        m_heap(_heap)
    {
        _pEditorEventPool->Add(this, &EntityInspectorWidget::OnEvent);

        _heap.Allocate(m_aFunctions, 1);

    }

    EntityInspectorWidget::~EntityInspectorWidget()
    {

    }

    void EntityInspectorWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));

        ecs_os_set_api_defaults();

        const auto& _types = RuntimeReflection::get_types();

        for(uint32_t _i = 0; _i < _types.Size(); _i++)
        {
            RuntimeReflection::ReflectedType _type(_types[_i]);

            const auto& _meta = _type.GetMeta("Component");

            if(_meta.m_ullTypeID != -1 && _meta.m_pData != nullptr)
            {
                Component _component;

                _component.m_sTypeName = _type.GetName();

                m_heap.Allocate(_component.m_aFunctions, 1);

                const auto& _props = _type.GetProperties(m_heap);
                const auto& _funcs = _type.GetFunctions(m_heap);

                for(uint32_t _j = 0; _j < _props.Size(); _j++)
                {
                    const auto& _prop = _props[_j];

                    for(uint32_t _k = 0; _k < _funcs.Size(); _k++)
                    {
                        const auto& _func = _funcs[_k];

                        if(strcmp(_func.GetName(), "Draw") == 0)
                        {
                            const auto& _args = _func.GetArguments(m_heap);

                            if(_args.Size() == 2)
                            {
                                const auto& _arg = _args[1];

                                if(_arg.GetTypeID() == _prop.GetTypeID())
                                {
                                    if(_component.m_aFunctions.Full())
                                    {
                                        _component.m_aFunctions.Resize(_component.m_aFunctions.Size() * 2);
                                    }

                                    Function _aa;

                                    _aa.m_typeHandle = _type.GetHandle();

                                    _aa.m_hasFuncHandle = _type.GetFunctionHandle<const Entity&>("Has");
                                    _aa.m_getFuncHandle = _type.GetFunctionHandle<const Entity&>("Get");

                                    _aa.m_pFunction = _func.GetPointer();
                                    _aa.m_sPropertyName = _prop.GetName();
                                    _aa.m_ullPropAddress = _prop.GetAddress();

                                    _component.m_aFunctions.Allocate(_aa);

                                    RuntimeReflection::ReflectedFunction<void, void*> _initEditorFunc = _type.GetFunction<void, void*>("InitEditor");

                                    if(_initEditorFunc.GetHandle().m_ID == -1)
                                    {
                                        continue;
                                    }

                                    _initEditorFunc.InvokeS(_pImguiContext);

                                    break;
                                }
                            }
                        }
                    }
                }

                if (m_aFunctions.Full())
                {
                    m_aFunctions.Resize(m_aFunctions.Size() * 2);
                }

                m_aFunctions.Allocate(_component);
            }
        }
    }

    void EntityInspectorWidget::OnDraw()
    {
        ImGui::Begin("Entity inspector");

        if(m_selectedEntity.m_entity != 0)
        {
            for(uint32_t _i = 0; _i < m_aFunctions.Size(); ++_i)
            {
                const Component& _c = m_aFunctions[_i];

                if(ImGui::TreeNodeEx(_c.m_sTypeName, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen, _c.m_sTypeName))
                {
                    for(uint32_t _j = 0; _j < _c.m_aFunctions.Size(); ++_j)
                    {
                        const Function& _func = _c.m_aFunctions[_j];

                        bool _has = RuntimeReflection::invoke_static_result<bool, const Entity&>(_func.m_typeHandle, _func.m_hasFuncHandle, m_selectedEntity);

                        if(!_has)
                        {
                            continue;
                        }

                        uint8_t* _componentAddress = RuntimeReflection::invoke_static_result<uint8_t*, const Entity&>(_func.m_typeHandle, _func.m_getFuncHandle, m_selectedEntity);

                        FunctionArgumentsPusher _fap(2);

                        _fap.Push(_func.m_sPropertyName);
                        _fap.Push(_componentAddress + _func.m_ullPropAddress);

                        _fap.Call(_func.m_pFunction);

                        _fap.getCode<void (*)()>()();
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