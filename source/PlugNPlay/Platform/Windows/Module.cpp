#include "PlugNPlay/Platform/Windows/Module.h"

#include <Windows.h>

namespace Duckvil { namespace PlugNPlay { namespace Platform {

    bool windows_load(__module_information* _pModule)
    {
        _pModule->m_pModule = LoadLibraryA(_pModule->m_sName);

        if(_pModule->m_pModule == NULL)
        {
            return false;
        }

        return true;
    }

    bool windows_get(const __module_information& _pModule, const char* _sName, void** _pFunction)
    {
        *_pFunction = GetProcAddress((HMODULE)_pModule.m_pModule, _sName);

        if(*_pFunction == NULL)
        {
            return false;
        }

        return true;
    }

    bool windows_free(__module_information* _pModule)
    {
        if(FreeLibrary((HMODULE)_pModule->m_pModule) == FALSE)
        {
            return false;
        }

        _pModule->m_pModule = 0;

        return true;
    }

}}}