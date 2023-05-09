#include "Editor/Widgets/EntityInspectorWidget.h"

#include "imgui/imgui.h"

#include "Engine/UUIDComponent.h"

#include "Graphics/TransformComponent.h"

#include "glm/gtx/quaternion.hpp"

#include "RuntimeReflection/Meta.h"

#include "Utils/RuntimeDependencyInjector.h"

#include "RuntimeReflection/ReflectedType.h"

#include "flecs/flecs.h"

#include "Engine/ScriptComponent.h"
#include "Engine/UUIDComponent.h"

#include "Graphics/MeshComponent.h"

#include "Engine/Events/RequestSystemEvent.h"

#include "Engine/Events/ComponentAttachedEvent.h"

namespace Duckvil { namespace Editor {

    static void draw_vec3(const char* _sLabel, glm::vec3& _value)
    {
        ImGui::DragFloat3(_sLabel, &_value[0], 0.1f);
    }

    EntityInspectorWidget::EntityInspectorWidget(const Memory::FreeList& _heap, Event::Pool<Event::mode::immediate>* _pEditorEventPool, ECS::EntityFactory* _pEntityFactory, Event::Pool<Event::mode::immediate>* _pEngineEventPool) :
        m_heap(_heap),
        m_pEntityFactory(_pEntityFactory),
        m_pEngineEventPool(_pEngineEventPool)
    {
        _pEditorEventPool->Add<EntitySelectedEvent>(this, &EntityInspectorWidget::OnEvent);
        _pEngineEventPool->Add<HotReloader::SwapEvent>(this, &EntityInspectorWidget::OnEvent);

        _heap.Allocate(m_aFunctions, 1);
        _heap.Allocate(m_aScripts, 1);

        ecs_os_init();

        _pEntityFactory->RegisterComponent<ScriptComponent>();
        _pEntityFactory->RegisterComponent<UUIDComponent>();
        _pEntityFactory->RegisterComponent<Graphics::TransformComponent>();
        _pEntityFactory->RegisterComponent<Graphics::MeshComponent>();

        m_iCurrentComponentItem = -1;
    }

    EntityInspectorWidget::~EntityInspectorWidget()
    {

    }

    void EntityInspectorWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));

        m_bPopupOpened = false;

        ecs_os_set_api_defaults();

        const auto& _types = RuntimeReflection::get_types();

        for(uint32_t _i = 0; _i < _types.Size(); _i++)
        {
            RuntimeReflection::ReflectedType _type(_types[_i]);

            const auto& _meta = _type.GetMeta("Component");

            if(_meta.m_ullTypeID != -1 && _meta.m_pData != nullptr)
            {
                Component _component;

                _component.m_sTypeName = _type.GetFullName();
                _component.m_typeHandle = _type.GetHandle();

                m_heap.Allocate(_component.m_aFunctions, 1);

                const auto& _props = _type.GetProperties(m_heap);
                const auto& _funcs = _type.GetFunctions(m_heap);

                for(uint32_t _j = 0; _j < _props.Size(); _j++)
                {
                    const auto& _prop = _props[_j];

                    for(uint32_t _k = 0; _k < _funcs.Size(); _k++)
                    {
                        const auto& _func = _funcs[_k];

                        if(strcmp(_func.GetName(), "Draw") != 0)
                        {
                            continue;
                        }

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

                                    _aa.m_hasFuncHandle = _type.GetFunctionHandle<const ECS::Entity&>("Has");
                                    _aa.m_getFuncHandle = _type.GetFunctionHandle<const ECS::Entity&>("Get");

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

                if(m_aFunctions.Full())
                {
                    m_aFunctions.Resize(m_aFunctions.Size() * 2);
                }

                m_aFunctions.Allocate(_component);

                m_aComponents.push_back(m_aFunctions[m_aFunctions.Size() - 1].m_sTypeName.m_sText);
            }
            else if(_type.Inherits<NativeScriptBase>())
            {
                const auto& _drawFunc = _type.GetFunctionHandle<const char***, uint32_t*, void*>("GetScripts");

                if(_drawFunc.m_ID == -1)
                {
                    Script _script;

                    _script.m_typeHandle = _type.GetHandle();
                    _script.m_sTypeName = _type.GetFullName();

                    if(m_aScripts.Full())
                    {
                        m_aScripts.Resize(m_aScripts.Size() * 2);
                    }

                    m_aScripts.Allocate(_script);

                    m_aScriptNames.push_back(m_aScripts[m_aScripts.Size() - 1].m_sTypeName.m_sText);
                }
                else
                {
                    const auto& _rFunc = RuntimeReflection::get_function(_type.GetHandle(), _drawFunc);
                    auto _pFunc = static_cast<RuntimeReflection::__proxy_static_function<void, const char***, uint32_t*, void*>*>(_rFunc.m_pFunction);

                    const char** _scripts;
                    uint32_t _count = 0;

                    RequestSystemEvent _e;

                    _e.m_typeHandle = RuntimeReflection::get_type("CSharp", { "Duckvil", "CSharp" });

                    m_pEngineEventPool->Broadcast(_e);

                    _pFunc->Invoke(&_scripts, &_count, _e.m_pRequestedSystem);

                    for(uint32_t _k = 0; _k < _count; ++_k)
                    {
                        Script _script;

                        _script.m_typeHandle = _type.GetHandle();
                        _script.m_sTypeName = _scripts[_k];

                        if(m_aScripts.Full())
                        {
                            m_aScripts.Resize(m_aScripts.Size() * 2);
                        }

                        m_aScripts.Allocate(_script);

                        m_aScriptNames.push_back(m_aScripts[m_aScripts.Size() - 1].m_sTypeName.m_sText);
                    }

                    delete[] _scripts;
                }
            }
        }

        static RuntimeDependencyInjector _fap(2);

        m_ppLabel = new const char*();
        m_ppAddress = new uint8_t*();
        m_pFunction = new const void*();

        _fap.Push(m_ppLabel);
        _fap.Push(m_ppAddress);

        *m_pFunction = nullptr;

        _fap.Call(m_pFunction);

        m_fnGeneratedFunction = _fap.getCode<void (*)()>();
    }

    void EntityInspectorWidget::OnDraw()
    {
        ImGui::Begin("Entity inspector");

        if(m_pEntityFactory->IsValid(m_selectedEntity) && m_selectedEntity.m_entity != 0)
        {
            bool _anyItemHovered = false;

            for(uint32_t _i = 0; _i < m_aFunctions.Size(); ++_i)
            {
                const Component& _c = m_aFunctions[_i];

                if(!m_selectedEntity.Has(m_pEntityFactory->LookupComponent(_c.m_sTypeName.m_sText)))
                {
                    continue;
                }

                if(ImGui::TreeNodeEx(_c.m_sTypeName.m_sText, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen, _c.m_sTypeName.m_sText))
                {
                    for(uint32_t _j = 0; _j < _c.m_aFunctions.Size(); ++_j)
                    {
                        const Function& _func = _c.m_aFunctions[_j];

                        bool _has = RuntimeReflection::invoke_static_result<bool, const ECS::Entity&>(_func.m_typeHandle, _func.m_hasFuncHandle, m_selectedEntity);

                        if(!_has)
                        {
                            continue;
                        }

                        uint8_t* _componentAddress = RuntimeReflection::invoke_static_result<uint8_t*, const ECS::Entity&>(_func.m_typeHandle, _func.m_getFuncHandle, m_selectedEntity);

                        RuntimeDependencyInjector _fap(2);

                        _fap.Push(_func.m_sPropertyName);
                        _fap.Push(_componentAddress + _func.m_ullPropAddress);

                        _fap.Call(_func.m_pFunction);

                        _fap.getCode<void (*)()>()();

                        // *m_ppLabel = _func.m_sPropertyName;
                        // *m_ppAddress = _componentAddress + _func.m_ullPropAddress;

                        // *m_pFunction = _func.m_pFunction;

                        // m_fnGeneratedFunction();
                    }

                    const auto& a = RuntimeReflection::get_type(_c.m_typeHandle);

                    if(strcmp(a.m_sTypeName, "ScriptComponent") == 0)
                    {
                        const auto& _scriptComponent = m_selectedEntity.Get<ScriptComponent>();
                        auto _size = Memory::fixed_vector_size(m_heap.GetMemoryInterface(), _scriptComponent.m_pScripts) / 8;

                        for(uint32_t _i = 0; _i < _size; ++_i)
                        {
                            const auto& _scriptKeeper = *static_cast<HotReloader::ITrackKeeper**>(Memory::fixed_vector_at(m_heap.GetMemoryInterface(), _scriptComponent.m_pScripts, _i));
                            const auto& _script = static_cast<NativeScriptBase*>(_scriptKeeper->GetObject());
                        	auto _type = RuntimeReflection::ReflectedType(_script->m_typeHandle);
                            Utils::string _name = _type.GetFullName();

                            if(ImGui::TreeNodeEx(_scriptKeeper, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen, _name.m_sText))
                            {
                                // Expose some things...

                                auto _drawDetailsHandle = RuntimeReflection::get_function_handle<>(_script->m_typeHandle, "DrawDetails");

                                if(_drawDetailsHandle.m_ID != -1)
                                {
                                    RuntimeReflection::invoke_member(_script->m_typeHandle, _drawDetailsHandle, _script);
                                }

                                ImGui::TreePop();
                            }

                            const auto& _id = std::string("##script_") + std::to_string(_i);

                            if(ImGui::IsItemHovered())
                            {
                                _anyItemHovered = true;

                                if(ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
                                {
                                    ImGui::OpenPopup(_id.c_str());
                                }

                                ImGui::BeginTooltip();

                                ImGui::Text("%lf", _script->m_dDelta);

                                ImGui::EndTooltip();
                            }

                            if(ImGui::BeginPopup(_id.c_str()))
                            {
                                if(ImGui::Button("Remove script"))
                                {
                                    ImGui::CloseCurrentPopup();

                                    HotReloader::ITrackKeeper* _tScript = *Memory::fixed_vector_at<HotReloader::ITrackKeeper*>(m_heap.GetMemoryInterface(), _scriptComponent.m_pScripts, _i);

                                    reinterpret_cast<NativeScriptBase*>(_tScript->GetObject())->~NativeScriptBase();
                                    _tScript->~ITrackKeeper();

                                    Memory::fixed_vector_erase(m_heap.GetMemoryInterface(), _scriptComponent.m_pScripts, _i);
                                }

                                ImGui::EndPopup();
                            }
                        }

                        if(ImGui::Button("Add script"))
                        {
                            ImGui::OpenPopup("##addScriptPopup");
                        }
                    }

                    if(ImGui::BeginPopup("##addScriptPopup"))
                    {
                        ImGui::ListBox("Scripts", &m_iCurrentScriptItem, m_aScriptNames.data(), m_aScriptNames.size());

                        if(ImGui::Button("Add"))
                        {
                            ImGui::CloseCurrentPopup();

                            auto _c = m_selectedEntity.GetMut<ScriptComponent>();
                            const auto& _selectedScript = m_aScripts[m_iCurrentScriptItem];

                            if(m_heap.GetMemoryInterface()->m_fnFixedVectorSize_(_c->m_pScripts) >= m_heap.GetMemoryInterface()->m_fnFixedVectorCapacity_(_c->m_pScripts))
                            {
                                m_heap.GetMemoryInterface()->m_fnFixedVectorResize_(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), &_c->m_pScripts, m_heap.GetMemoryInterface()->m_fnFixedVectorCapacity_(_c->m_pScripts) * 2);
                            }

                            auto _s = RuntimeReflection::create(m_heap, _selectedScript.m_typeHandle, true);
                            auto _s2 = *static_cast<HotReloader::ITrackKeeper**>(m_heap.GetMemoryInterface()->m_fnFixedVectorAllocate_(_c->m_pScripts, &_s, 8, 8));
                            auto _o = static_cast<NativeScriptBase*>(_s2->GetObject());

                            _o->m_typeHandle = _selectedScript.m_typeHandle;

                            _o->SetEntity(m_selectedEntity);

                            const auto& _createScriptFunc = RuntimeReflection::get_function_handle<const char*, void*>(_selectedScript.m_typeHandle, "CreateScript");

                            if(_createScriptFunc.m_ID != -1)
                            {
                                RequestSystemEvent _e;

                                _e.m_typeHandle = RuntimeReflection::get_type("CSharp", { "Duckvil", "CSharp" });

                                m_pEngineEventPool->Broadcast(_e);

                                RuntimeReflection::invoke_member<const char*, void*>(_selectedScript.m_typeHandle, _createScriptFunc, _o, m_aScriptNames[m_iCurrentScriptItem], _e.m_pRequestedSystem);
                            }

                            _o->Init();
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::TreePop();
                }
            }

            if(!_anyItemHovered && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup("##entityInspectorPopup");

                m_bPopupOpened = true;
            }
        }
        else
        {
            // if(m_bPopupOpened)
            // {
            //     ImGui::CloseCurrentPopup();
            // }
        }

        if(ImGui::BeginPopup("##entityInspectorPopup"))
        {
            if(ImGui::Button("Add component"))
            {
                ImGui::OpenPopup("##entityInspectorAddComponentPopup");
            }

            if(ImGui::BeginPopup("##entityInspectorAddComponentPopup"))
            {
                ImGui::ListBox("Components", &m_iCurrentComponentItem, m_aComponents.data(), m_aComponents.size());

                if(ImGui::Button("Add"))
                {
                    if(m_selectedEntity.m_entity != 0)
                    {
                        const Component& _component = m_aFunctions[m_iCurrentComponentItem];
                        ECS::Entity _e = m_pEntityFactory->LookupComponent(m_aComponents[m_iCurrentComponentItem]);

                        m_selectedEntity.Add(_e);

                        m_pEntityFactory->GetEventPool()->Broadcast(ComponentAttachedEvent{ m_selectedEntity, _component.m_typeHandle });
                    }

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void EntityInspectorWidget::OnEvent(const EntitySelectedEvent& _event)
    {
        m_selectedEntity = _event.m_entity;
    }

    void EntityInspectorWidget::OnEvent(const HotReloader::SwapEvent& _event)
    {
        printf("aaaaa");
    }

    void EntityInspectorWidget::OnEvent(const ProjectManager::OnLoadEvent& _event)
    {

    }

}}