#pragma once

#include "Editor/Widget.h"

#include "Engine/ISystem.h"
#include "Engine/ReflectionFlags.h"

#include "ProjectManager/Script.h"

#include "{$headerFile}.generated.h"

namespace {$projectName} {

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_UserSystem)
    class {$scriptName}{$inheritance}
    {
        DUCKVIL_GENERATED_BODY
    private:

    public:
        {$scriptName}();
        ~{$scriptName}();

        {$headerFunctions}
    };

}