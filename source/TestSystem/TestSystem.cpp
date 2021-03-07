#include "TestSystem/TestSystem.h"

#include <cstdio>

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace LOL {

    Test::Test()
    {
        m_iA = 20;
    }

    Test::Test(const Memory::FreeList& _heap, const duckvil_frontend_reflection_context& _runtimeReflectionContext, const logger_context& _loggerContext)
    {
        RuntimeReflection::make_current(_runtimeReflectionContext);
        logger_make_current(_loggerContext);
    }

    Test::~Test()
    {

    }

    bool Test::Init()
    {
        m_iA = 100;

        return true;
    }

    void Test::Update()
    {
        // DUCKVIL_LOG_INFO(m_pLogger, m_pLoggerData, "axxxa %i", m_iA);
        // printf("%i\n", m_iA);
        // printf("AAAAAA\n");

        // DUCKVIL_LOG_INFO("axxxa %i", m_iA);
    }

    void Test::InitEditor(void* _pImguiContext, const duckvil_frontend_reflection_context& _context)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
    }

    void Test::OnDraw()
    {
        ImGui::Begin("XXX");

        ImGui::End();
    }

    void Test::OnEvent(const test& _event)
    {
        printf("AAAAA\n");
    }

    void Test::OnEvent(const test2& _event)
    {
        printf("AAAAA2\n");
    }

    void Test::OnEvent(const HotReloader::HotReloadedEvent& _event)
    {
        printf("AAAAA3\n");
    }

}}