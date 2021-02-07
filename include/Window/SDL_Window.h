#pragma once

#include "SDL2/include/SDL.h"

#include "Window/IWindow.h"

namespace Duckvil { namespace Window {

    class WindowSDL : public IWindow
    {
    private:
        SDL_Window* m_pWindow;

    public:
        WindowSDL();
        ~WindowSDL();

        bool Create() override;
        void Refresh() const override;
    };

}}