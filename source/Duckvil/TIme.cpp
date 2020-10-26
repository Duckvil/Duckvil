#include "Duckvil/Time.h"

namespace Duckvil {

    bool time_init(__time_data* _pData)
    {
        _pData->m_previousTime = std::chrono::high_resolution_clock::now();

        return true;
    }

    void time_update(__time_data* _pData)
    {
        std::chrono::steady_clock::time_point _now = std::chrono::high_resolution_clock::now();
        auto& _durationTime = std::chrono::duration<double, std::milli>(_now - _pData->m_previousTime);

        _pData->m_previousTime = _now;
        _pData->m_dDelta = _durationTime.count();
    }

    __time_ftable time_init()
    {
        __time_ftable _ftable;

        _ftable.init = &time_init;
        _ftable.update = &time_update;

        return _ftable;
    }

}