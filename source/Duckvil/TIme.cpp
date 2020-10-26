#include "Duckvil/Time.h"

namespace Duckvil {

    bool time_init(__time_data* _pData)
    {
        _pData->m_llPreviousTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        return true;
    }

    void time_update(__time_data* _pData)
    {
        long long _now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        long long _durationTime = _now - _pData->m_llPreviousTime;

        _pData->m_llPreviousTime = _now;
        _pData->m_dDelta = _durationTime * 0.000000001;
    }

    __time_ftable time_init()
    {
        __time_ftable _ftable;

        _ftable.init = &time_init;
        _ftable.update = &time_update;

        return _ftable;
    }

}