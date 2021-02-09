#pragma once

namespace Duckvil { namespace Window {

    struct IWindow
    {
        virtual bool Create(const char* _sTitle, int _iWidth, int _iHeight) = 0;
        virtual void Refresh() const = 0;
        virtual void* GetWindow() const = 0;
        virtual void* GetContext() const = 0;
    };

}}