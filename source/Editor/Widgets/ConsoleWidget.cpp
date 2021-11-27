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

    void ConsoleWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);

        RuntimeReflection::ReflectedType<__logger_channel_data> _loggerType(m_heap);

        Event::Pool<Event::mode::immediate>* aaa = (Event::Pool<Event::mode::immediate>*)RuntimeReflection::get_meta_value_ptr(_loggerType.GetTypeHandle(), "EventPool");

        aaa->Add<__logger_channel_log_info>(this, &ConsoleWidget::OnEvent);
    }

    void ConsoleWidget::OnDraw()
    {
        ImGui::Begin("Console");

        for(const __logger_channel_log_info& _log : m_aLogs)
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
            ImGui::SetScrollHereY(1.f);

            m_bScrollBottom = false;
        }

        ImGui::End();
    }

    void ConsoleWidget::OnEvent(const __logger_channel_log_info& _event)
    {
        if(!(_event.m_pOwner->m_flags & __logger_channel_flags::__logger_flags_editor_console_output))
        {
            return;
        }

        if(m_aLogs.Full())
        {
            m_aLogs.Resize(m_aLogs.Size() * 2);
        }

        m_aLogs.Allocate(_event);
        m_bScrollBottom = true;
    }

}}