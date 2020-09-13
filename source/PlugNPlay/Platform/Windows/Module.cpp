#include "PlugNPlay/Platform/Windows/Module.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>

#include <cstddef>

#include "Utils/Utils.h"

namespace Duckvil { namespace PlugNPlay { namespace Platform {

    bool windows_load(__module_information* _pModule)
    {
        Utils::string _buffer;
        
        Utils::join(_buffer, DUCKVIL_OUTPUT, "/", _pModule->m_sName, ".dll");

        _pModule->m_pModule = LoadLibraryA(_buffer.m_sText);

        if(_pModule->m_pModule == nullptr)
        {
            return false;
        }

        return true;
    }

    bool windows_get(const __module_information& _pModule, const char* _sName, void** _pFunction)
    {
        *_pFunction = GetProcAddress((HMODULE)_pModule.m_pModule, _sName);

        if(*_pFunction == nullptr)
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
#endif