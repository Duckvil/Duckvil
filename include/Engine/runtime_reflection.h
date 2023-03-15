#pragma once

#include "PlugNPlay/Module.h"

#include "Engine/Duckvil.h"

namespace Duckvil {

    bool init_runtime_reflection_module(__data* _pData, PlugNPlay::__module* _pModule);
    void init_runtime_reflection(__data* _pData, const PlugNPlay::__module& _module);

}