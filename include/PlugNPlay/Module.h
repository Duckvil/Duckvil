#pragma once

#include "Utils/Utils.h"
#include "Utils/Macro.h"

#include <filesystem>

namespace Duckvil { namespace PlugNPlay {

    struct __module_information
    {
        __module_information()
        {
            m_pModule = nullptr;
        }

        __module_information(const Utils::string& _sName) :
            m_sName(_sName)
        {
#ifndef DUCKVIL_RUNTIME_COMPILE
#ifdef DUCKVIL_OUTPUT
            m_sPath = DUCKVIL_OUTPUT;
#else
            static_assert(false, "DUCKVIL_OUTPUT is not specified!");
#endif
#else
#ifdef DUCKVIL_OUTPUT
            m_sPath = DUCKVIL_TO_STRING(DUCKVIL_OUTPUT);
#else
            static_assert(false, "DUCKVIL_OUTPUT is not specified!");
#endif
#endif

            m_pModule = nullptr;
        }

        __module_information(const Utils::string& _sName, const Utils::string& _sPath) :
            m_sName(_sName),
            m_sPath(_sPath)
        {
            m_pModule = nullptr;
        }

        ~__module_information()
        {

        }

        Utils::string m_sName;
        void* m_pModule;
        Utils::string m_sPath;
    };

    struct __module
    {
        bool (*load)(__module_information* _pModule);
        bool (*get)(const __module_information& _module, const char* _sName, void** _pFunction);
        bool (*free)(__module_information* _pModule);
    };

    bool module_init(__module* _pModule);
    
    template <typename Type>
    bool get_variable(const __module& _module, const __module_information& _module_info, const char* _sName, Type* _pVariable)
    {
        Type* _temp = nullptr;
        bool result = _module.get(_module_info, _sName, (void**)&_temp);

        *_pVariable = *_temp;

        return result;
    }

}}