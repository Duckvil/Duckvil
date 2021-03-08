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

namespace Duckvil { namespace Editor {

    void* init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Window::IWindow* _pWindow)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(ImGuiEditorData), alignof(ImGuiEditorData));

        _data->m_aDraws = Memory::Vector<Draw>(_pMemoryInterface, _pAllocator, 1);

        SDL_GL_MakeCurrent((SDL_Window*)_pWindow->GetWindow(), _pWindow->GetContext());

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK)
        {
            // throw std::exception("Failed to init GLEW!\n");
            // printf("%s\n", glewGetErrorString(err));
        }

        IMGUI_CHECKVERSION();

        ImGuiContext* _ctx = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)_pWindow->GetWindow(), _pWindow->GetContext());
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui::SetCurrentContext(_ctx);

        _data->_ctx = _ctx;

        return _data;
    }

    void post_init(const Memory::FreeList& _heap, void* _pData)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;

        _data->m_pEditorEvents = Event::Pool<Event::mode::immediate>(_heap, g_duckvilFrontendReflectionContext.m_pReflection, g_duckvilFrontendReflectionContext.m_pReflectionData);

        auto _types = RuntimeReflection::get_types(_heap);

        for(uint32_t i = 0; i < _types.Size(); ++i)
        {
            RuntimeReflection::__duckvil_resource_type_t _typeHandle = _types[i];
            RuntimeReflection::ReflectedType<> _type(_heap, _typeHandle);

            if(RuntimeReflection::inherits<Editor::Widget>(_typeHandle) && !RuntimeReflection::inherits<ISystem>(_typeHandle))
            {
                const auto& _res = RuntimeReflection::get_constructors(_heap, _typeHandle);

                for(uint32_t i = 0; i < _res.Size(); ++i)
                {
                    const auto& _res2 = RuntimeReflection::get_arguments(_heap, _typeHandle, { i });

                    for(uint32_t j = 0; j < _res2.Size(); ++j)
                    {
                        const auto& _lol = RuntimeReflection::get_argument(_heap, _typeHandle, { i }, { j });

                        printf("AAAAA\n");
                    }
                }

                void* _object = _type.Create<const Memory::FreeList&>(_heap);
                Editor::Widget* _widget = (Editor::Widget*)_type.InvokeStatic<void*, void*>("Cast", _object);
                RuntimeReflection::__duckvil_resource_type_t _trackKeeperHandle = RuntimeReflection::get_type<HotReloader::TrackKeeper>();
                HotReloader::TrackKeeper* _trackKeeper = (HotReloader::TrackKeeper*)RuntimeReflection::create(_heap, g_duckvilFrontendReflectionContext.m_pReflectionData, _trackKeeperHandle, _object, _typeHandle);

                auto _lol = _type.GetFunctionCallback<Editor::Widget>("OnDraw")->m_fnFunction;
                auto _lol2 = _type.GetFunctionCallback<Editor::Widget, void*>("InitEditor")->m_fnFunction;

                if(_data->m_aDraws.Full())
                {
                    _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
                }

                _data->m_aDraws.Allocate(Editor::Draw { _lol, _lol2, _trackKeeper });

                if(_type.GetFunctionHandle<const HexEditorWidgetInitEvent&>("OnEvent").m_ID != -1)
                {
                    _data->m_pEditorEvents.Add<HexEditorWidgetInitEvent>(_object, _typeHandle);
                }
            }
        }

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
        {
            const Draw& _widget = _data->m_aDraws[i];
            Widget* _pWidget = (Widget*)_widget.m_pObject->GetObject();

            (_pWidget->*_widget.m_fnInit)(_data->_ctx);
        }
    }

    void render(void* _pData, Window::IWindow* _pWindow)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame((SDL_Window*)_pWindow->GetWindow());

        ::ImGui::NewFrame();

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

                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        ::ImGui::End();

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
        {
            const Draw& _widget = _data->m_aDraws[i];
            Widget* _pWidget = (Widget*)_widget.m_pObject->GetObject();

            (_pWidget->*_widget.m_fnDraw)();
        }

        ::ImGui::Render();

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());

        SDL_Window* _window = SDL_GL_GetCurrentWindow();
        SDL_GLContext _ctx = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        SDL_GL_MakeCurrent(_window, _ctx);
    }

    void add_draw(void* _pData, Draw _draw)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;
        uint32_t _found = -1;

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
        {
            const Draw& _draw2 = _data->m_aDraws[i];

            if(_draw2.m_pObject == _draw.m_pObject)
            {
                _found = i;

                break;
            }
        }

        if(_found != -1)
        {
            Draw& _draw2 = _data->m_aDraws[_found];

            _draw2.m_fnDraw = _draw.m_fnDraw;
            _draw2.m_fnInit = _draw.m_fnInit;

            return;
        }

        if(_data->m_aDraws.Full())
        {
            _data->m_aDraws.Resize(_data->m_aDraws.Size() * 2);
        }

        _data->m_aDraws.Allocate(_draw);
    }

    void remove_draw(void* _pData, void* _pObject)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); ++i)
        {
            const Draw& _draw = _data->m_aDraws[i];

            if(_draw.m_pObject->GetObject() == _pObject)
            {
                _data->m_aDraws.Erase(i);

                break;
            }
        }
    }

}}

Duckvil::Editor::EditorFTable* duckvil_editor_init(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Editor::EditorFTable* _result = (Duckvil::Editor::EditorFTable*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Editor::EditorFTable), alignof(Duckvil::Editor::EditorFTable));

    _result->m_fnInit = &Duckvil::Editor::init;
    _result->m_fnRender = &Duckvil::Editor::render;

    _result->m_fnAddDraw = &Duckvil::Editor::add_draw;
    _result->m_fnRemoveDraw = &Duckvil::Editor::remove_draw;

    _result->m_fnPostInit = &Duckvil::Editor::post_init;

    return _result;
}