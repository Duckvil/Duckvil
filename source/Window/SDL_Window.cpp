#include "Window/SDL_Window.h"

#include "SDL2/include/SDL.h"

namespace Duckvil { namespace Window {

    WindowSDL::WindowSDL(Event::Pool<Event::mode::buffered>* _pEventsPool) :
        m_pEvents(_pEventsPool)
    {
        _pEventsPool->Add<CloseEvent>();
        _pEventsPool->Add<ResizeEvent>();
    }

    WindowSDL::~WindowSDL()
    {

    }

    bool WindowSDL::Create(const char* _sTitle, int _iWidth, int _iHeight)
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_pWindow = SDL_CreateWindow(_sTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _iWidth, _iHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        m_pContext = SDL_GL_CreateContext((SDL_Window*)m_pWindow);

        SDL_GL_MakeCurrent((SDL_Window*)m_pWindow, m_pContext);
        SDL_GL_SetSwapInterval(0);

        return true;
    }

    void WindowSDL::Refresh() const
    {
        SDL_Event _event;

        while(SDL_PollEvent(&_event))
        {
            if(_event.type == SDL_QUIT)
            {
                m_pEvents->Broadcast(CloseEvent{ });
            }
            else if(_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                m_pEvents->Broadcast(ResizeEvent{ _event.window.data1, _event.window.data2 });
            }
        }

        SDL_GL_SwapWindow((SDL_Window*)m_pWindow);
    }

    void* WindowSDL::GetWindow() const
    {
        return m_pWindow;
    }

    void* WindowSDL::GetContext() const
    {
        return m_pContext;
    }
}}