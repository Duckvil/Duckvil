#pragma once

#include <chrono>

namespace Duckvil {

    struct __time_data
    {
        // std::chrono::steady_clock::time_point m_previousTime;
        long long m_llPreviousTime;
        double m_dDelta;
    };

    struct __time_ftable
    {
        bool (*init)(__time_data* _pData);
        void (*update)(__time_data* _pData);
    };

    __time_ftable time_init();

}