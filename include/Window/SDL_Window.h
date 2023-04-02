#pragma once

#include "Window/IWindow.h"

#include "Event/BufferedPool.h"

#include "Window/Events/CloseEvent.h"
#include "Window/Events/ResizeEvent.h"
#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"

namespace Duckvil { namespace Window {

    class WindowSDL : public IWindow
    {
    private:
        void* m_pWindow;
        void* m_pContext;

        uint32_t m_uiWidth;
        uint32_t m_uiHeight;

        Event::Pool<Event::mode::buffered>* m_pEvents;

        void (*m_fnProcessEvents)(void* _pData);

    public:
        WindowSDL(Event::Pool<Event::mode::buffered>* _pEventsPool);
        ~WindowSDL();

        bool Create(const char* _sTitle, uint32_t _uiWidth, uint32_t _uiHeight) override;
        void Refresh() const override;

        void PopulateEvents() override;

        void* GetWindow() const override;
        void* GetContext() const override;

        void SetMousePosition(int _iX, int _iY, bool _bGlobal = false) override;

        void SetProcessEventsCallback(void (*_fnProcessEvents)(void* _pData)) override;

    	uint32_t GetWidth() const override { return m_uiWidth; }
    	uint32_t GetHeight() const override { return m_uiHeight; }
    };

}}