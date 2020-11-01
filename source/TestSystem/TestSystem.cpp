#include "TestSystem/TestSystem.h"

namespace Duckvil { namespace LOL {

    Test::Test(int a) :
        m_iA(a)
    {

    }

    Test::~Test()
    {

    }

    bool Test::Init()
    {
        return true;
    }

    void Test::Update()
    {
        // DUCKVIL_LOG_INFO_(m_pLogger, m_pLoggerData, "AAAAAA");
        // printf("%i\n", m_iA);
    }

}}