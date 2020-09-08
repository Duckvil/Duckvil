#include "PlugNPlay/Module.h"

#include <Windows.h>

namespace Duckvil { namespace PlugNPlay {

    bool load(__module* _pModule)
    {
        _pModule->m_pModule = LoadLibraryA(_pModule->m_sName);

        if(_pModule->m_pModule == NULL)
        {
            return false;
        }

        return true;
    }

    bool get(const __module& _pModule, const char* _sName, void** _pFunction)
    {
        *_pFunction = GetProcAddress((HMODULE)_pModule.m_pModule, _sName);

        if(*_pFunction == NULL)
        {
            return false;
        }

        return true;
    }

    bool free(__module* _pModule)
    {
        if(FreeLibrary((HMODULE)_pModule->m_pModule) == FALSE)
        {
            return false;
        }

        _pModule->m_pModule = 0;

        return true;
    }

}}