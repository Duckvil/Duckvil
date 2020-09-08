#pragma once

namespace Duckvil { namespace PlugNPlay {

    struct __module
    {
        __module(const char* _sName) :
            m_sName(_sName)
        {

        }

        const char* m_sName;
        void* m_pModule;
    };

    bool load(__module* _pModule);
    bool get(const __module& _pModule, const char* _sName, void** _pFunction);
    bool free(__module* _pModule);

}}