#pragma once

namespace Duckvil { namespace PlugNPlay {

    struct __module_information
    {
        __module_information(const char* _sName) :
            m_sName(_sName)
        {

        }

        const char* m_sName;
        void* m_pModule;
    };

    struct module
    {
        bool (*load)(__module_information* _pModule);
        bool (*get)(const __module_information& _pModule, const char* _sName, void** _pFunction);
        bool (*free)(__module_information* _pModule);
    };

    bool module_init(module* _pModule);

}}