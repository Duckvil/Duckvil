#include "CSharp/TestSharedScript.h"

namespace Sandbox {

    TestSharedScript::TestSharedScript()
    {
        
    }

    TestSharedScript::~TestSharedScript()
    {

    }

    void TestSharedScript::HelloFrom(int _a)
    {
        printf("Called from C++! %i %i\n", _a, _an_a);

        _an_a++;
        a(_an_a);
    }

    bool TestSharedScript::Init()
    {
        Initialize();

        return true;
    }

}