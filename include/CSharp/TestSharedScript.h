#pragma once

#include "CSharp/SharedScript.h"

#include "CSharp/CSharp.h"

#include "RuntimeReflection/Markers.h"

#include "CSharp/TestSharedScript.generated.h"

namespace Sandbox {

    class TestSharedScript : public Duckvil::CSharp::SharedScript
    {
        DUCKVIL_CS_FUNCTION(void,Say,{int a;})
        DUCKVIL_CS_FUNCTION(void,Initialize,{})

        DUCKVIL_GENERATED_BODY
    private:

    public:
        TestSharedScript();
        ~TestSharedScript();

        DUCKVIL_VARIABLE(CSharp::ReflectionFlags::EXPOSE_TO_CSHARP)
        int _an_a;

        bool Init() override;

        DUCKVIL_FUNCTION(CSharp::ReflectionFlags::EXPOSE_TO_CSHARP)
        void HelloFrom(int a);

        
    };

}