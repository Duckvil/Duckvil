#pragma once

#include "Event/BufferedPool.h"

namespace Duckvil { namespace Window {

    struct IWindow
    {
        virtual bool Create(const char* _sTitle, uint32_t _uiWidth, uint32_t _uiHeight) = 0;
        virtual void Refresh() const = 0;
        virtual void* GetWindow() const = 0;
        virtual void* GetContext() const = 0;
        virtual void PopulateEvents() = 0;

        virtual void SetMousePosition(int _iX, int _iY, bool _bGlobal = false) = 0;

        virtual void SetProcessEventsCallback(void (*_fnProcessEvents)(void* _pData)) = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
    };

}}