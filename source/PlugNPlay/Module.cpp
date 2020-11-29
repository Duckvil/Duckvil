#include "PlugNPlay/Module.h"

#include "PlugNPlay/Platform/Windows/Module.h"
#include "PlugNPlay/Platform/Linux/Module.h"

namespace Duckvil { namespace PlugNPlay {

    bool module_init(__module* _pModule)
    {
#if defined(DUCKVIL_PLATFORM_WINDOWS)
        _pModule->load = &Platform::windows_load;
        _pModule->get = &Platform::windows_get;
        _pModule->free = &Platform::windows_free;

        return true;
#elif defined(DUCKVIL_PLATFORM_LINUX)
        _pModule->load = &Platform::linux_load;
        _pModule->get = &Platform::linux_get;
        _pModule->free = &Platform::linux_free;

        return true;
#else
        return false;
#endif
    }

}}