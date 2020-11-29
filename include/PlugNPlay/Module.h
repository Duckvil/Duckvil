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

        __module_information(const Utils::string& _sName, const Utils::string& _sPath) :
            m_sName(_sName),
            m_sPath(_sPath)
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