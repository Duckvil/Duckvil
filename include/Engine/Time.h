#pragma once

#include <chrono>

namespace Duckvil {

    struct __time_data
    {
        typedef long long long_long; // TODO: My parser is fucked up, so I have to typedef this

        long_long m_llPreviousTime;
        double m_dDelta;
    };

    struct __time_ftable
    {
        bool (*init)(__time_data* _pData);
        void (*update)(__time_data* _pData);
    };

    __time_ftable time_init();

}