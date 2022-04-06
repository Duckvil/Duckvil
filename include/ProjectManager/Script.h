#pragma once

namespace Duckvil { namespace Project {

    struct Script
    {
        virtual bool Init() { return true; }
        virtual void Update() { }
    };

}}