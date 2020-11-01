#pragma once

#include "Duckvil/ISystem.h"
#include "Duckvil/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"

namespace Duckvil { namespace LOL {

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_EngineSystem)
    class Test : public ISystem
    {
    private:
        int m_iA;

    public:
        Test(int a);
        ~Test();

        bool Init();
        void Update();
    };

}}