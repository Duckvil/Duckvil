#include "PlugNPlay/Platform/Linux/Module.h"

#ifdef DUCKVIL_PLATFORM_LINUX
#include <dlfcn.h>

#include <cstring>
#include <cstddef>

namespace Duckvil { namespace PlugNPlay { namespace Platform {

    bool linux_load(__module_information* _pModule)
    {
        char buffer[256] = { 0 };
        std::size_t len = strlen(DUCKVIL_OUTPUT);

        strcpy(buffer, DUCKVIL_OUTPUT);
        strcpy(buffer + len, "/");
        len++;
        strcpy(buffer + len, _pModule->m_sName);
        len += strlen(_pModule->m_sName);
        strcpy(buffer + len, ".so");

        _pModule->m_pModule = dlopen(buffer, RTLD_LAZY);

        if(_pModule->m_pModule == nullptr)
        {
            return false;
        }

        return true;
    }

    bool linux_get(const __module_information& _pModule, const char* _sName, void** _pFunction)
    {
        *_pFunction = dlsym(_pModule.m_pModule, _sName);

        if(*_pFunction == nullptr)
        {
            return false;
        }

        return true;
    }

    bool linux_free(__module_information* _pModule)
    {
        if(dlclose(_pModule->m_pModule) != 0)
        {
            return false;
        }

        _pModule->m_pModule = 0;

        return true;
    }

}}}
#endif