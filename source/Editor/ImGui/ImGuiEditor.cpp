#include "Editor/ImGui/ImGuiEditor.h"

#include "imgui/imgui.h"

#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"

#include "SDL2/include/SDL.h"

namespace Duckvil { namespace Editor { namespace ImGuiEditor {

    void draw_lol()
    {
        ::ImGui::Begin("LOL");

        ::ImGui::End();
    }

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

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)_pWindow->GetWindow(), _pWindow->GetContext());
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui::SetCurrentContext(_ctx);

        _data->_ctx = _ctx;

        // _data->m_aDraws.Allocate(&draw_lol);

        return _data;
    }

    void post_init(void* _pData)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;

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

        ::ImGui::Begin("Hello, world!");

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
    }

    void add_draw(void* _pData, Draw _draw)
    {
        ImGuiEditorData* _data = (ImGuiEditorData*)_pData;
        uint32_t _found = -1;

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); i++)
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

        for(uint32_t i = 0; i < _data->m_aDraws.Size(); i++)
        {
            const Draw& _draw = _data->m_aDraws[i];

            if(_draw.m_pObject->GetObject() == _pObject)
            {
                _data->m_aDraws.Erase(i);

                break;
            }
        }
    }

}}}