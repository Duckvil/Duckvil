#pragma once

#include "SDL2/include/SDL.h"

#include "Window/IWindow.h"

#include "Event/BufferedPool.h"

#include "Window/Events/CloseEvent.h"
#include "Window/Events/ResizeEvent.h"

namespace Duckvil { namespace Window {

    class WindowSDL : public IWindow
    {
    private:
        SDL_Window* m_pWindow;

        Event::Pool<Event::mode::buffered>* m_pEvents;

    public:
        WindowSDL(Event::Pool<Event::mode::buffered>* _pEventsPool);
        ~WindowSDL();

        bool Create(const char* _sTitle, int _iWidth, int _iHeight) override;
        void Refresh() const override;
    };

}}