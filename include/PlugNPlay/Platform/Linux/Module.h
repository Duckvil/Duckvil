#pragma once

#ifdef DUCKVIL_PLATFORM_LINUX
#include "PlugNPlay/Module.h"

namespace Duckvil { namespace PlugNPlay { namespace Platform {

    bool linux_load(__module_information* _pModule);
    bool linux_get(const __module_information& _pModule, const char* _sName, void** _pFunction);
    bool linux_free(__module_information* _pModule);

}}}
#endif