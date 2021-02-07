#pragma once

namespace Duckvil { namespace Window {

    struct IWindow
    {
        virtual bool Create() = 0;
        virtual void Refresh() const = 0;
    };

}}