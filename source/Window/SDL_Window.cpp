#include "Window/SDL_Window.h"

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

        m_pWindow = SDL_CreateWindow(_sTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _iWidth, _iHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

        SDL_GL_SwapWindow(m_pWindow);
    }

}}