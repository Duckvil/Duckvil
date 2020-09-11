#pragma once

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include "PlugNPlay/Module.h"

namespace Duckvil { namespace PlugNPlay { namespace Platform {

    bool windows_load(__module_information* _pModule);
    bool windows_get(const __module_information& _pModule, const char* _sName, void** _pFunction);
    bool windows_free(__module_information* _pModule);

}}}
#endif