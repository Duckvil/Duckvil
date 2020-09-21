#pragma once

#include "Utils/Utils.h"

#include <filesystem>

namespace Duckvil { namespace PlugNPlay {

    struct __module_information
    {
        __module_information(const Utils::string& _sName) :
            m_sName(_sName)
        {
            m_sPath = DUCKVIL_OUTPUT;
        }

        Utils::string m_sName;
        void* m_pModule;
        Utils::string m_sPath;
    };

    struct module
    {
        bool (*load)(__module_information* _pModule);
        bool (*get)(const __module_information& _module, const char* _sName, void** _pFunction);
        bool (*free)(__module_information* _pModule);
    };

    bool module_init(module* _pModule);

}}