#include "Editor/Widgets/ConsoleWidget.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/ReflectedType.h"
#include "RuntimeReflection/Meta.h"

#include "Event/ImmediatePool.h"

namespace Duckvil { namespace Editor {

    ConsoleWidget::ConsoleWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {
        m_heap.Allocate(m_aLogs, 1);
        m_bScrollBottom = false;
    }

    ConsoleWidget::~ConsoleWidget()
    {

    }

    void ConsoleWidget::InitEditor(void* _pImguiContext, const duckvil_frontend_reflection_context& _context)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
        RuntimeReflection::make_current(_context);

        RuntimeReflection::ReflectedType<Logger::__data> _loggerType(m_heap);

        Event::Pool<Event::mode::immediate>* aaa = (Event::Pool<Event::mode::immediate>*)RuntimeReflection::get_meta_value_ptr(_loggerType.GetTypeHandle(), "EventPool");

        aaa->Add<Logger::__log_info>(this, &ConsoleWidget::OnEvent);
    }

    void ConsoleWidget::OnDraw()
    {
        ImGui::Begin("Console");

        for(const Logger::__log_info& _log : m_aLogs)
        {
            ImGui::Text("%s", _log.m_sMessage);

            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                std::string _file = "code -g ";
                _file.append(_log.m_sFile);
                _file.append(":").append(std::to_string(_log.m_uiLine));

                system(_file.c_str());
            }

            if(ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Line: %d", _log.m_uiLine);
                ImGui::Text("File: %s", _log.m_sFile);
                ImGui::EndTooltip();
            }
        }

        if(m_bScrollBottom)
        {
            ImGui::SetScrollHere(1.f);

            m_bScrollBottom = false;
        }

        ImGui::End();
    }

    void ConsoleWidget::OnEvent(const Logger::__log_info& _event)
    {
        if(m_aLogs.Full())
        {
            m_aLogs.Resize(m_aLogs.Size() * 2);
        }

        m_aLogs.Allocate(_event);
        m_bScrollBottom = true;
    }

}}