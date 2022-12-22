#pragma once

#include <stdint.h>

namespace Duckvil { namespace ScriptEngine {

    struct Language
    {
        virtual bool Setup() = 0;
        virtual bool Init() = 0;
        virtual void Update() = 0;
        virtual bool Shutdown() = 0;

        virtual void* GetType(const char* _sTypeName, const char** _saNamepsace, uint32_t _uiCount) = 0;
        virtual void* CreateObject(void* _pType) = 0;
    };

}}