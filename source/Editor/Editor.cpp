#define STB_IMAGE_IMPLEMENTATION
#include "Editor/Editor.h"

#include "imgui/imgui.h"

#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"

#include "SDL2/include/SDL.h"

#include "HotReloader/TrackKeeper.h"

#include "Engine/ISystem.h"

#include "Editor/Widgets/HexEditorWidgetEvent.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Engine/ReflectionFlags.h"

#include "Editor/ReflectionFlag.h"

#include "Editor/Events/CloseWidgetEvent.h"

#include "tracy/Tracy.hpp"

#include "Utils/FunctionArgumentsPusher.h"

#include "Engine/Events/RequestSystemEvent.h"

#include "ProjectManager/ProjectManager.h"

#undef GetObject

namespace Duckvil { namespace Editor {

    void process_sdl_events(void* _pData)
    {
        ImGui_ImplSDL2_ProcessEvent(static_cast<SDL_Event*>(_pData));
    }

    void* init(EditorFTable* _pEditor, const Memory::FreeList& _heap, Window::IWindow* _pWindow, Graphics::Renderer::renderer_ftable* _pRenderer, Graphics::Renderer::renderer_data* _pRendererData, Event::Pool<Event::mode::immediate>* _pRuntimeReflectionEventPool, Event::Pool<Event::mode::immediate>* _pEngineEventPool)
    {
        // ImGuiEditorData* _data = static_cast<ImGuiEditorData*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(ImGuiEditorData), alignof(ImGuiEditorData)));
        ImGuiEditorData* _data = _heap.Allocate<ImGuiEditorData>();

        _data->m_pRenderer = _pRenderer;
        _data->m_pRendererData = _pRendererData;
        _data->m_pRuntimeReflectionEventPool = _pRuntimeReflectionEventPool;
        _data->m_heap = _heap;

#ifdef DUCKVIL_HOT_RELOADING
        // _data->m_aHotDraws = Memory::Vector<HotDraw>(_pMemoryInterface, _pAllocator, 1);
        _heap.Allocate(_data->m_aHotDraws, 1);
#endif
        // _data->m_aDraws = Memory::Vector<Draw>(_pMemoryInterface, _pAllocator, 1);
        // _data->m_aTextures = Memory::Vector<uint32_t>(_pMemoryInterface, _pAllocator, 1);
        _heap.Allocate(_data->m_aDraws, 1);
        _heap.Allocate(_data->m_aTextures, 1);

        _data->m_aTextures.Allocate(0);

        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(_pWindow->GetWindow()), _pWindow->GetContext());

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK)
        {
            // throw std::runtime_error("Failed to init GLEW!");
            throw std::runtime_error((const char*)glewGetErrorString(err));
            // printf("%s\n", glewGetErrorString(err));
        }

        IMGUI_CHECKVERSION();

        ImGuiContext* _ctx = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(_pWindow->GetWindow()), _pWindow->GetContext());
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui::SetCurrentContext(_ctx);

        _data->_ctx = _ctx;

        _pWindow->SetProcessEventsCallback(&process_sdl_events);

        _data->m_pEditorEvents = Event::Pool<Event::mode::immediate>(_heap, g_duckvilFrontendReflectionContext.m_pReflection, g_duckvilFrontendReflectionContext.m_pReflectionData);

        _pRuntimeReflectionEventPool->Add(Utils::lambda([_data, _pEngineEventPool, _pEditor](const RuntimeReflection::TrackedObjectCreatedEvent& _event)
        {
            RuntimeReflection::ReflectedType<> _type(_data->m_heap, _event.m_pTrackKeeper->GetTypeHandle());
            RuntimeReflection::__variant _spawnableVariant = _type.GetMetaVariant(ReflectionFlag::Spwanable);

            if(!_type.Inherits<Widget>() || _spawnableVariant.m_ullTypeID != -1)
            {
                return;
            }

            const auto& _isHot = RuntimeReflection::get_object_meta(_event.m_pTrackKeeper->GetObject(), "IsHot").m_variant;

            if(_isHot.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_isHot.m_pData))
            {
                return;
            }

            if(_data->m_aHotDraws.Full())
            {
                _data->m_aHotDraws.Resize(_data->m_aHotDraws.Size() * 2);
            }

            _data->m_aHotDraws.Allocate(
                Editor::HotDraw
                {
                    _type.GetFunctionCallback<Widget>("OnDraw")->m_fnFunction,
                    _type.GetFunctionCallback<Widget, void*>("InitEditor")->m_fnFunction,
                    _event.m_pTrackKeeper, _event.m_pTrackKeeper->GetTypeHandle()
                }
            );

            if(_type.GetFunctionHandle<const HexEditorWidgetInitEvent&>("OnEvent").m_ID != -1)
            {
                _data->m_pEditorEvents.Add<HexEditorWidgetInitEvent>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_event.m_pTrackKeeper), _event.m_pTrackKeeper->GetTypeHandle());
            }

            // _pEngineEventPool->AddA<HotReloader::SwapEvent>([_data, _pEditor](const HotReloader::SwapEvent& _event)
            // {
            //     // if(_event.m_stage == HotReloader::HotReloadedEvent::stage_after_swap)
            //     if(RuntimeReflection::inherits<Widget>(_event.m_pTrackKeeper->GetTypeHandle()))
            //     {
            //         _pEditor->m_fnHotReloadInit(_data, _event);
            //     }
            // });
        }));

        _pRuntimeReflectionEventPool->Add(Utils::lambda([_data, _pEngineEventPool, _pEditor](const RuntimeReflection::ObjectCreatedEvent& _event)
        {
            RuntimeReflection::ReflectedType<> _type(_data->m_heap, { RuntimeReflection::get_type(_event.m_ullTypeID) });
            RuntimeReflection::__variant _spawnableVariant = _type.GetMetaVariant(ReflectionFlag::Spwanable);

            if(!_type.Inherits<Widget>() || _spawnableVariant.m_ullTypeID != -1)
            {
                return;
            }

            const auto& _isHot = RuntimeReflection::get_object_meta(_event.m_pObject, "IsHot").m_variant;

            if(_isHot.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_isHot.m_pData))
            {
                return;
            }

            if(_data->m_aDraws.Full())
            {
                _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
            }

            _data->m_aDraws.Allocate(
                Editor::Draw
                {
                    _type.GetFunctionCallback<Widget>("OnDraw")->m_fnFunction,
                    _type.GetFunctionCallback<Widget, void*>("InitEditor")->m_fnFunction,
                    _event.m_pObject, _type.GetTypeHandle()
                }
            );

            if(_type.GetFunctionHandle<const HexEditorWidgetInitEvent&>("OnEvent").m_ID != -1)
            {
                _data->m_pEditorEvents.Add<HexEditorWidgetInitEvent>(_event.m_pObject, _type.GetTypeHandle());
            }
        }));

        _pEngineEventPool->AddA<HotReloader::SwapEvent>([_data, _pEditor](const HotReloader::SwapEvent& _event)
        {
            // if(_event.m_stage == HotReloader::HotReloadedEvent::stage_after_swap)
            if(RuntimeReflection::inherits<Widget>(/*_type.GetTypeHandle()*/ _event.m_pTrackKeeper->GetTypeHandle()))
            {
                _pEditor->m_fnHotReloadInit(_data, _event);
            }
        });

        return _data;
    }

    void post_init(void* _pData, EditorFTable* _pEditor, Event::Pool<Event::mode::immediate>* _pEventPool, HotReloader::RuntimeCompilerSystem* _pHotReloader)
    {
        ImGuiEditorData* _data = static_cast<ImGuiEditorData*>(_pData);

        // auto _types = RuntimeReflection::get_types(_data->m_heap);

//         for(uint32_t i = 0; i < _types.Size(); ++i)
//         {
//             RuntimeReflection::__duckvil_resource_type_t _typeHandle = _types[i];
//             RuntimeReflection::ReflectedType<> _type(_data->m_heap, _typeHandle);
//             RuntimeReflection::__variant _spawnableVariant = RuntimeReflection::get_meta(_typeHandle, ReflectionFlag::Spwanable);

//             if(RuntimeReflection::inherits<Widget>(_typeHandle) && !RuntimeReflection::inherits<ISystem>(_typeHandle) && _spawnableVariant.m_ullTypeID == -1)
//             {
// #ifdef DUCKVIL_HOT_RELOADING
//                 void* _object = _type.CreateTracked<const Memory::FreeList&>(_data->m_heap);

//                 if(_data->m_aHotDraws.Full())
//                 {
//                     _data->m_aHotDraws.Resize(_data->m_aHotDraws.Size() * 2);
//                 }

//                 _data->m_aHotDraws.Allocate(
//                     Editor::HotDraw
//                     {
//                         _type.GetFunctionCallback<Widget>("OnDraw")->m_fnFunction,
//                         _type.GetFunctionCallback<Widget, void*>("InitEditor")->m_fnFunction,
//                         DUCKVIL_TRACK_KEEPER_CAST(Widget, _object), _typeHandle
//                     }
//                 );
// #else
//                 void* _object = _type.Create<const Memory::FreeList&>(_data->m_heap);

//                 if(_data->m_aDraws.Full())
//                 {
//                     _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
//                 }

//                 _data->m_aDraws.Allocate(
//                     Editor::Draw
//                     {
//                         _type.GetFunctionCallback<Widget>("OnDraw")->m_fnFunction,
//                         _type.GetFunctionCallback<Widget, void*>("InitEditor")->m_fnFunction,
//                         _object, _typeHandle
//                     }
//                 );
// #endif

//                 if(_type.GetFunctionHandle<const HexEditorWidgetInitEvent&>("OnEvent").m_ID != -1)
//                 {
//                     _data->m_pEditorEvents.Add<HexEditorWidgetInitEvent>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_object), _typeHandle);
//                 }

//                 _pEventPool->AddA<HotReloader::SwapEvent>([_data, _pEditor](const HotReloader::SwapEvent& _event)
//                 {
//                     // if(_event.m_stage == HotReloader::HotReloadedEvent::stage_after_swap)
//                     if(RuntimeReflection::inherits<Widget>(_event.m_pTrackKeeper->GetTypeHandle()))
//                     {
//                         _pEditor->m_fnHotReloadInit(_data, _event);
//                     }
//                 });
//             }
//         }

#ifdef DUCKVIL_HOT_RELOADING
        for(uint32_t i = 0; i < _data->m_aHotDraws.Size(); ++i)
        {
            const HotDraw& _widget = _data->m_aHotDraws[i];
            Widget* _pWidget = (Widget*)DUCKVIL_TRACK_KEEPER_GET_OBJECT(_widget.m_pTrackKeeper);

            if(RuntimeReflection::get_type(_widget.m_typeHandle).m_ullTypeID == RuntimeReflection::get_type(RuntimeReflection::get_type("ViewportWidget", { "Duckvil", "Editor" })).m_ullTypeID)
            {
                const auto& _func = RuntimeReflection::get_function_handle<Graphics::Renderer::renderer_ftable*, Graphics::Renderer::renderer_data*>(_widget.m_typeHandle, "SetRenderer");
                const auto& _func2 = RuntimeReflection::get_function_handle<Event::Pool<Event::mode::buffered>*>(_widget.m_typeHandle, "SetEventPool");
                const auto& _func3 = RuntimeReflection::get_function_handle<flecs::world*>(_widget.m_typeHandle, "SetECS");

                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func, _pWidget, _data->m_pRenderer, _data->m_pRendererData);
                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func2, _pWidget, _data->m_pWindowEventPool);
                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func3, _pWidget, _data->m_pECS);
            }

            (_pWidget->*_widget.m_fnInit)(_data->_ctx);
        }
#endif

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
        {
            const Draw& _widget = _data->m_aDraws[i];
            Widget* _pWidget = static_cast<Widget*>(_widget.m_pObject);

            if(RuntimeReflection::get_type(_widget.m_typeHandle).m_ullTypeID == RuntimeReflection::get_type(RuntimeReflection::get_type("ViewportWidget", { "Duckvil", "Editor" })).m_ullTypeID)
            {
                const auto& _func = RuntimeReflection::get_function_handle<Graphics::Renderer::renderer_ftable*, Graphics::Renderer::renderer_data*>(_widget.m_typeHandle, "SetRenderer");
                const auto& _func2 = RuntimeReflection::get_function_handle<Event::Pool<Event::mode::buffered>*>(_widget.m_typeHandle, "SetEventPool");
                const auto& _func3 = RuntimeReflection::get_function_handle<flecs::world*>(_widget.m_typeHandle, "SetECS");

                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func, _pWidget, _data->m_pRenderer, _data->m_pRendererData);
                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func2, _pWidget, _data->m_pWindowEventPool);
                RuntimeReflection::invoke_member(_widget.m_typeHandle, _func3, _pWidget, _data->m_pECS);
            }

            (_pWidget->*_widget.m_fnInit)(_data->_ctx);
        }

        _data->m_pEditorEvents.AddA<SpwanWidgetEvent>([_data, _pEventPool](const SpwanWidgetEvent& _event)
        {
            RuntimeReflection::ReflectedType<> _type(_data->m_heap, _event.m_typeHandle);
            RuntimeReflection::__variant _spawnableVariant = RuntimeReflection::get_meta(_event.m_typeHandle, ReflectionFlag::Spwanable);

            if(_spawnableVariant.m_ullTypeID == -1 || (_spawnableVariant.m_ullTypeID == typeid(bool).hash_code() && !*static_cast<bool*>(_spawnableVariant.m_pData)))
            {
                return;
            }

            // void* _object = _type.Create<const Memory::FreeList&>(_data->m_heap);
            void* _object = nullptr;
            auto _constructors = RuntimeReflection::get_constructors(_data->m_heap, _event.m_typeHandle);

            for(const auto& _constructorHandle : _constructors)
            {
                const auto& _constructor = RuntimeReflection::get_constructor(_event.m_typeHandle, _constructorHandle);
                uint32_t _constructorArgumentsCount = DUCKVIL_SLOT_ARRAY_SIZE(_constructor.m_arguments);

                if(_constructorArgumentsCount > 0)
                {
                    FunctionArgumentsPusher c(5 + _constructorArgumentsCount);

                    c.Push(_data->m_heap.GetMemoryInterface());
                    c.Push(_data->m_heap.GetAllocator());
                    c.Push(RuntimeReflection::get_current().m_pReflection);
                    c.Push(RuntimeReflection::get_current().m_pReflectionData);
                    c.Push(false);

                    for(uint32_t i = 0; i < _constructorArgumentsCount; ++i)
                    {
                        const RuntimeReflection::__argument_t& _argument = DUCKVIL_SLOT_ARRAY_GET(_constructor.m_arguments, i);

                        if(typeid(Memory::FreeList).hash_code() == _argument.m_ullTypeID)
                        {
                            c.Push(_data->m_heap);
                        }

                        if(typeid(HotReloader::RuntimeCompilerSystem*).hash_code() == _argument.m_ullTypeID)
                        {
                            RequestSystemEvent _rEvent;

                            _rEvent.m_typeHandle = RuntimeReflection::get_type<HotReloader::RuntimeCompilerSystem>();

                            _pEventPool->Broadcast(_rEvent);

                            c.Push(_rEvent.m_pRequestedSystem);
                        }

                        if(typeid(const Event::Pool<Event::mode::immediate>*).hash_code() == _argument.m_ullTypeID)
                        {
                            auto _argumentMeta = RuntimeReflection::get_argument_meta_variant(_type.GetTypeHandle(), _constructorHandle, i, "Editor");

                            if(_argumentMeta.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_argumentMeta.m_pData))
                            {
                                c.Push(&_data->m_pEditorEvents);
                            }
                            else
                            {
                                _argumentMeta = RuntimeReflection::get_argument_meta_variant(_type.GetTypeHandle(), _constructorHandle, i, "Engine");

                                if(_argumentMeta.m_ullTypeID == typeid(bool).hash_code() && *static_cast<bool*>(_argumentMeta.m_pData))
                                {
                                    c.Push(_pEventPool);
                                }
                            }
                        }

                        if(typeid(ProjectManager::ftable*).hash_code() == _argument.m_ullTypeID)
                        {
                            RequestSystemEvent _rEvent;

                            _rEvent.m_typeHandle = RuntimeReflection::get_type<ProjectManager::ftable>();

                            _pEventPool->Broadcast(_rEvent);

                            c.Push(_rEvent.m_pRequestedSystem);
                        }

                        if(typeid(ProjectManager::data*).hash_code() == _argument.m_ullTypeID)
                        {
                            RequestSystemEvent _rEvent;

                            _rEvent.m_typeHandle = RuntimeReflection::get_type<ProjectManager::data>();

                            _pEventPool->Broadcast(_rEvent);

                            c.Push(_rEvent.m_pRequestedSystem);
                        }
                    }

                    // const auto& _h = RuntimeReflection::get_constructor_handle<const Memory::FreeList&>(_type.GetTypeHandle());
                    const auto& _hc = RuntimeReflection::get_constructor(_type.GetTypeHandle(), _constructorHandle);

                    c.Call(_hc.m_pData);

                    _object = c.getCode<void*(*)()>()();
                }
            }

            if(_data->m_aDraws.Full())
            {
                _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
            }

            _data->m_aDraws.Allocate(
                Editor::Draw
                {
                    _type.GetFunctionCallback<Widget>("OnDraw")->m_fnFunction,
                    _type.GetFunctionCallback<Widget, void*>("InitEditor")->m_fnFunction,
                    _object, _event.m_typeHandle
                }
            );
        });

        _data->m_pEditorEvents.AddA<CloseWidgetEvent>([_data](const CloseWidgetEvent& _event)
        {
            for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
            {
                Draw& _draw = _data->m_aDraws[i];

                if(_draw.m_typeHandle.m_ID == _event.m_typeHandle.m_ID)
                {
                    _data->m_aDraws.Erase(i);

                    // _data->m_heap.Free(_event.m_pObject);
                    RuntimeReflection::destroy(_data->m_heap, _event.m_typeHandle, false, _event.m_pObject);

                    break;
                }
            }
        });
    }

    void hot_reload_init(ImGuiEditorData* _pData, const HotReloader::SwapEvent& _event)
    {
#ifdef DUCKVIL_HOT_RELOADING
        uint32_t _index = -1;

        RuntimeReflection::ReflectedType<> _type(_pData->m_heap, _event.m_pTrackKeeper->GetTypeHandle());
        RuntimeReflection::__duckvil_resource_function_t _onDrawFunctionHandle = _type.GetFunctionHandle("OnDraw");
        RuntimeReflection::__duckvil_resource_function_t _initEditorFunctionHandle = _type.GetFunctionHandle<void*>("InitEditor");
        //Editor::Widget* _widgetInheritance = (Editor::Widget*)_type.InvokeStatic<void*, void*>("Cast", DUCKVIL_TRACK_KEEPER_GET_OBJECT(_event.m_pTrackKeeper));

        for(uint32_t i = 0; i < _pData->m_aHotDraws.Size(); ++i)
        {
            HotDraw& _widget = _pData->m_aHotDraws[i];

            if(_widget.m_typeHandle.m_ID == _type.GetTypeHandle().m_ID)
            {
                _index = i;

                break;
            }
        }

        if(_index == -1)
        {
            if(_pData->m_aHotDraws.Full())
            {
                _pData->m_aHotDraws.Resize(_pData->m_aHotDraws.Size() * 2);
            }

            _pData->m_aHotDraws.Allocate(
                {
                    _type.GetFunctionCallback<Widget>(_onDrawFunctionHandle)->m_fnFunction,
                    _type.GetFunctionCallback<Widget, void*>(_initEditorFunctionHandle)->m_fnFunction,
                    _event.m_pTrackKeeper,
                    // _widgetInheritance,
                    _event.m_pTrackKeeper->GetTypeHandle()
                });
        }
        else
        {
            if(_onDrawFunctionHandle.m_ID != -1 && _initEditorFunctionHandle.m_ID != -1)
            {
                HotDraw& _widget = _pData->m_aHotDraws[_index];

                _widget.m_fnDraw = _type.GetFunctionCallback<Widget>(_onDrawFunctionHandle)->m_fnFunction;
                _widget.m_fnInit = _type.GetFunctionCallback<Widget, void*>(_initEditorFunctionHandle)->m_fnFunction;
                // _widget.m_pObject = _widgetInheritance;

                ((Editor::Widget*)DUCKVIL_TRACK_KEEPER_GET_OBJECT(_event.m_pTrackKeeper)->*_widget.m_fnInit)(ImGui::GetCurrentContext());
            }
            else
            {
                _pData->m_aHotDraws.Erase(_index);
            }
        }
#endif
    }

    void render(void* _pData, Window::IWindow* _pWindow)
    {
        ImGuiEditorData* _data = static_cast<ImGuiEditorData*>(_pData);

        {
            ZoneScopedN("ImGuiNewFrame");

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(_pWindow->GetWindow()));

            ImGui::NewFrame();
        }

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size, ImGuiCond_Always);

        if(ImGui::Begin("EditorMainMenuBar", 0,
            ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoDocking))
        {
            ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");

			ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("Project"))
                {
                    if(ImGui::MenuItem("New"))
                    {
                        _data->m_pEditorEvents.Broadcast(SpwanWidgetEvent{ RuntimeReflection::get_type("NewProjectWidget", { "Duckvil", "Editor" }).m_ID });
                    }

                    if(ImGui::MenuItem("Load"))
                    {
                        _data->m_pEditorEvents.Broadcast(SpwanWidgetEvent{ RuntimeReflection::get_type("LoadProjectWidget", { "Duckvil", "Editor" }).m_ID });
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        ImGui::End();

        {
            ZoneScopedN("DrawHotWidgets");

#ifdef DUCKVIL_HOT_RELOADING
            for(uint32_t i = 0; i < _data->m_aHotDraws.Size(); ++i)
            {
                const HotDraw& _widget = _data->m_aHotDraws[i];

                ((Widget*)DUCKVIL_TRACK_KEEPER_GET_OBJECT(_widget.m_pTrackKeeper)->*_widget.m_fnDraw)();
            }
#endif
        }

        {
            ZoneScopedN("DrawWidgets");

            for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
            {
                const Draw& _widget = _data->m_aDraws[i];

                (static_cast<Widget*>(_widget.m_pObject)->*_widget.m_fnDraw)();
            }
        }

        {
            ZoneScopedN("ImGuiRender");

            ImGui::Render();
        }

        {
            ZoneScopedN("ImGui_ImplOpenGL3_RenderDrawData");

            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
        }

        SDL_Window* _window = SDL_GL_GetCurrentWindow();
        SDL_GLContext _ctx = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        {
            ZoneScopedN("SDL_GL_MakeCurrent");

            SDL_GL_MakeCurrent(_window, _ctx);
        }
    }

//     void add_hot_draw(void* _pData, HotDraw _draw)
//     {
// #ifdef DUCKVIL_HOT_RELOADING
//         ImGuiEditorData* _data = (ImGuiEditorData*)_pData;
//         uint32_t _found = -1;

//         for(uint32_t i = 0; i < _data->m_aHotDraws.Size(); ++i)
//         {
//             const HotDraw& _draw2 = _data->m_aHotDraws[i];

//             if(_draw2.m_pTrackKeeper == _draw.m_pTrackKeeper)
//             {
//                 _found = i;

//                 break;
//             }
//         }

//         if(_found != -1)
//         {
//             HotDraw& _draw2 = _data->m_aHotDraws[_found];

//             _draw2.m_fnDraw = _draw.m_fnDraw;
//             _draw2.m_fnInit = _draw.m_fnInit;

//             return;
//         }

//         if(_data->m_aHotDraws.Full())
//         {
//             _data->m_aHotDraws.Resize(_data->m_aHotDraws.Size() * 2);
//         }

//         _data->m_aHotDraws.Allocate(_draw);
// #endif
//     }

//     void add_draw(void* _pData, Draw _draw)
//     {
//         ImGuiEditorData* _data = static_cast<ImGuiEditorData*>(_pData);
//         uint32_t _found = -1;

//         for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
//         {
//             const Draw& _draw2 = _data->m_aDraws[i];

//             if(_draw2.m_pObject == _draw.m_pObject)
//             {
//                 _found = i;

//                 break;
//             }
//         }

//         if(_found != -1)
//         {
//             Draw& _draw2 = _data->m_aDraws[_found];

//             _draw2.m_fnDraw = _draw.m_fnDraw;
//             _draw2.m_fnInit = _draw.m_fnInit;

//             return;
//         }

//         if(_data->m_aDraws.Full())
//         {
//             _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
//         }

//         _data->m_aDraws.Allocate(_draw);
//     }

    // void remove_draw(void* _pData, void* _pObject)
    // {
    //     ImGuiEditorData* _data = (ImGuiEditorData*)_pData;

    //     for(uint32_t i = 0; i < _data->m_aHotDraws.Size(); ++i)
    //     {
    //         const HotDraw& _draw = _data->m_aHotDraws[i];

    //         if(_draw.m_pObject->GetObject() == _pObject)
    //         {
    //             _data->m_aHotDraws.Erase(i);

    //             break;
    //         }
    //     }
    // }

    void set_window_event_pool(void* _pData, Event::Pool<Event::mode::buffered>* _pWindowEventPool)
    {
        (static_cast<ImGuiEditorData*>(_pData))->m_pWindowEventPool = _pWindowEventPool;
    }

}}

Duckvil::Editor::EditorFTable* duckvil_editor_init()
{
    static Duckvil::Editor::EditorFTable _ftable = { 0 };

    _ftable.m_fnInit = &Duckvil::Editor::init;
    _ftable.m_fnRender = &Duckvil::Editor::render;

    // _ftable.m_fnAddHotDraw = &Duckvil::Editor::add_hot_draw;
    // _ftable.m_fnAddDraw = &Duckvil::Editor::add_draw;
    // _result->m_fnRemoveDraw = &Duckvil::Editor::remove_draw;

    _ftable.m_fnSetWindowEventPool = &Duckvil::Editor::set_window_event_pool;

    _ftable.m_fnPostInit = &Duckvil::Editor::post_init;
    _ftable.m_fnHotReloadInit = &Duckvil::Editor::hot_reload_init;

    return &_ftable;
}