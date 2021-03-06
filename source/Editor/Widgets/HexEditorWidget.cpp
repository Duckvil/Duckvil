#include "Editor/Widgets/HexEditorWidget.h"

namespace Duckvil { namespace Editor {

    #ifdef DUCKVIL_MEMORY_DEBUGGER
    void recursive(HexEditorWidget* _pHexWidget, duckvil_memory_debug_info* _pDebugInfo, uint32_t& _index)
    {
        if(ImGui::TreeNodeEx((void*)(uintptr_t)_index, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None, "%s %d", _pDebugInfo->m_aLabel, _index))
        {
            _pHexWidget->m_pSelected = _pDebugInfo;

            if(_pHexWidget->m_pSelected && ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();

                ImGui::Text("Capacity: %d", _pHexWidget->m_pSelected->m_pAllocator->m_ullCapacity);

                if(_pHexWidget->m_pSelected->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_free_list)
                {
                    ImGui::Text("Type: Free list");
                }
                else if(_pHexWidget->m_pSelected->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_linear)
                {
                    ImGui::Text("Type: Linear");
                }
                else if(_pHexWidget->m_pSelected->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_vector)
                {
                    ImGui::Text("Type: Vector");
                }
                else if(_pHexWidget->m_pSelected->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_array)
                {
                    ImGui::Text("Type: Array");
                }
                else if(_pHexWidget->m_pSelected->m_allocatorType == duckvil_memory_allocator_type::duckvil_memory_allocator_type_queue)
                {
                    ImGui::Text("Type: Queue");
                }

                ImGui::EndTooltip();
            }

            for(const auto& a : _pDebugInfo->m_aOther)
            {
                recursive(_pHexWidget, a, _index);
            }

            ImGui::TreePop();
        }

        _index++;
    }
#endif

    void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1)
    {
        ImVec2 backup_pos = ImGui::GetCursorPos();

        if (split_vertically)
        {
            ImGui::SetCursorPosY(backup_pos.y + *size0);
        }
        else
        {
            ImGui::SetCursorPosX(backup_pos.x + *size0);
        }

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f,0.6f,0.6f,0.10f));
        ImGui::Button("##Splitter", ImVec2(!split_vertically ? thickness : -1.0f, split_vertically ? thickness : -1.0f));
        ImGui::PopStyleColor(3);

        ImGui::SetItemAllowOverlap();

        if(ImGui::IsItemActive())
        {
            float mouse_delta = split_vertically ? ImGui::GetIO().MouseDelta.y : ImGui::GetIO().MouseDelta.x;

            if(mouse_delta < min_size0 - *size0)
            {
                mouse_delta = min_size0 - *size0;
            }

            if(mouse_delta > *size1 - min_size1)
            {
                mouse_delta = *size1 - min_size1;
            }

            *size0 += mouse_delta;
            *size1 -= mouse_delta;
        }

        ImGui::SetCursorPos(backup_pos);
    }

    HexEditorWidget::HexEditorWidget(const Memory::FreeList& _heap) :
        m_heap(_heap)
    {
        a1 = 1000;
        a2 = 1000;
#ifdef DUCKVIL_MEMORY_DEBUGGER
        m_pSelected = nullptr;
#endif
    }

    HexEditorWidget::~HexEditorWidget()
    {

    }

    void HexEditorWidget::InitEditor(void* _pImguiContext, const duckvil_frontend_reflection_context& _context)
    {
        ImGui::SetCurrentContext((ImGuiContext*)_pImguiContext);
        RuntimeReflection::make_current(_context);
    }

    void HexEditorWidget::OnDraw()
    {
        uint32_t _index = 0;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        ImGui::Begin("Hex editor");
        {
            float _width = ImGui::GetWindowContentRegionWidth();

            a2 = _width - a1;
            a1 = _width - a2;

            DrawSplitter(0, 8, &a1, &a2, 8, 8);
            ImGui::BeginChild("Allocators tree", ImVec2(a1, 0), true);
            ImGui::Text("Memory used: %d", m_heap.GetAllocator()->m_ullUsed);
            recursive(this, m_pMemoryDebugInfo, _index);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("Hex view", ImVec2(a2, 0), true);

            if(m_pSelected != nullptr)
            {
                switch(m_pSelected->m_allocatorType)
                {
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_array:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__fixed_array_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_vector:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__fixed_vector_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_linear:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__linear_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_free_list:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__free_list_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_queue:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__fixed_queue_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                case duckvil_memory_allocator_type::duckvil_memory_allocator_type_stack:
                    m_hexEditor.DrawContents((uint8_t*)m_pSelected->m_pAllocator + sizeof(Memory::__fixed_stack_allocator), m_pSelected->m_pAllocator->m_ullCapacity);
                    break;
                default:
                    break;
                }
            }

            ImGui::EndChild();
        }
        ImGui::End();
#endif
    }

    void HexEditorWidget::OnEvent(const HexEditorWidgetInitEvent& _event)
    {
#ifdef DUCKVIL_MEMORY_DEBUGGER
        m_pMemoryDebugInfo = _event.m_pDebugInfo;
#endif
    }

}}