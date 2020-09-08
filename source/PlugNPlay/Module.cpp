#include "PlugNPlay/Module.h"

#include <Windows.h>

#include "PlugNPlay/Platform/Windows/Module.h"

namespace Duckvil { namespace PlugNPlay {

    bool module_init(module* _pModule)
    {
#ifdef _WINDOWS
        _pModule->load = &Platform::windows_load;
        _pModule->get = &Platform::windows_get;
        _pModule->free = &Platform::windows_free;
#else
    return false;
#endif

        return true;
    }

}}