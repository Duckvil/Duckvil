#include "Window/SDL_Window.h"

namespace Duckvil { namespace Window {

    WindowSDL::WindowSDL()
    {

    }

    WindowSDL::~WindowSDL()
    {

    }

    bool WindowSDL::Create()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        m_pWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_OPENGL);

        return true;
    }

    void WindowSDL::Refresh() const
    {
        SDL_Event _event;

        while(SDL_PollEvent(&_event))
        {

        }

        SDL_GL_SwapWindow(m_pWindow);
    }

}}