#include "Editor/Widgets/RuntimeReflectionWidget.h"

#include "imgui/imgui.h"

#include "RuntimeReflection/Meta.h"
#include "RuntimeReflection/GeneratedMeta.h"

namespace Duckvil { namespace Editor {

    RuntimeReflectionWidget::RuntimeReflectionWidget()
    {

    }

    RuntimeReflectionWidget::RuntimeReflectionWidget(const Memory::FreeList& _heap)
    {
        m_heap = Memory::FreeList(_heap.GetMemoryInterface(), _heap.GetAllocator(), 1024 * 512);
    }

    RuntimeReflectionWidget::~RuntimeReflectionWidget()
    {

    }

    void RuntimeReflectionWidget::InitEditor(void* _pImguiContext)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(_pImguiContext));

        m_aTypes = RuntimeReflection::get_types(m_heap);

        m_uiCurrentItem = 0;
        m_uiCurrentFunctionItem = 0;
    }

    void RuntimeReflectionWidget::OnDraw()
    {
        ImGui::Begin("RuntimeReflectionWidget");

        ImGui::Text("Used %i of %i", m_heap.GetAllocator()->m_ullUsed, m_heap.GetAllocator()->m_ullCapacity);

        ImGui::Separator();

        uint32_t _typesCount = m_aTypes.Size();
        uint32_t _functionsCount = 0;
        uint32_t _propertiesCount = 0;
        uint32_t _metasCount = 0;

        Memory::Stack<const char*> _types;

        m_heap.Allocate(_types, _typesCount);

        for(uint32_t i = 0; i < _typesCount; ++i)
        {
            const RuntimeReflection::__type_t& _type = RuntimeReflection::get_type(m_aTypes[i]);

            _functionsCount += DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data);
            _propertiesCount += DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data);
            _metasCount += DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_metas.m_data);

            _types.Allocate(_type.m_sTypeName);
        }

        ImGui::Combo("Types", &m_uiCurrentItem, (const char**)((uint8_t*)_types.m_pContainer + sizeof(Memory::fixed_stack_allocator)), _typesCount);

        {
            const RuntimeReflection::__type_t& _type = RuntimeReflection::get_type(m_aTypes[m_uiCurrentItem]);
            Utils::string _full("", m_heap.GetMemoryInterface(), m_heap.GetAllocator());

            for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data); ++j)
            {
                Utils::string _namespace(DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j).m_sNamespaceName);

                _full = _full + _namespace + Utils::string("::", m_heap.GetMemoryInterface(), m_heap.GetAllocator());
            }

            ImGui::Text("Namespace: %s", _full.m_sText);

            auto _typeMetas = RuntimeReflection::get_metas(m_heap, m_aTypes[m_uiCurrentItem]);

            for(uint32_t i = 0; i < _typeMetas.Size(); ++i)
            {
                auto _meta = RuntimeReflection::get_meta(m_aTypes[m_uiCurrentItem], _typeMetas[i]);
                auto _key = RuntimeReflection::get_variant_key(m_aTypes[m_uiCurrentItem], _meta.m_key);
                auto _value = RuntimeReflection::get_variant_value(m_aTypes[m_uiCurrentItem], _meta.m_value);

                if(static_cast<uint16_t>(_key.m_variant.m_traits) & static_cast<uint16_t>(RuntimeReflection::property_traits::is_enum))
                {
                    if(_key.m_variant.m_ullTypeID == typeid(Duckvil::RuntimeReflection::GeneratedMeta).hash_code())
                    {
                        Duckvil::RuntimeReflection::GeneratedMeta _m = *static_cast<Duckvil::RuntimeReflection::GeneratedMeta*>(_key.m_variant.m_pData);

                        if(_m == Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID)
                        {
                            if(static_cast<uint16_t>(_value.m_variant.m_traits) & static_cast<uint16_t>(RuntimeReflection::property_traits::is_integral))
                            {
                                ImGui::Text("Recorder ID: %i", *(uint32_t*)_value.m_variant.m_pData);
                            }
                        }
                    }
                }
            }

            {
                auto _functions = RuntimeReflection::get_functions(m_heap, m_aTypes[m_uiCurrentItem]);
                Memory::Stack<const char*> _functionsNames;

                m_heap.Allocate(_functionsNames, _functions.Size());

                for(uint32_t i = 0; i < _functions.Size(); ++i)
                {
                    auto _func = RuntimeReflection::get_function(m_aTypes[m_uiCurrentItem], _functions[i]);

                    _functionsNames.Allocate(_func.m_sFunctionName);
                }

                ImGui::Combo("Functions", &m_uiCurrentFunctionItem, (const char**)((uint8_t*)_functionsNames.m_pContainer + sizeof(Memory::fixed_stack_allocator)), _functions.Size());

                if(!_functions.Empty())
                {
                    auto _functionMetas = RuntimeReflection::get_metas(m_heap, m_aTypes[m_uiCurrentItem], _functions[m_uiCurrentFunctionItem]);

                    for(uint32_t i = 0; i < _functionMetas.Size(); ++i)
                    {
                        auto _meta = RuntimeReflection::get_meta(m_aTypes[m_uiCurrentItem], _functions[m_uiCurrentFunctionItem], _functionMetas[i]);
                        auto _key = RuntimeReflection::get_variant_key(m_aTypes[m_uiCurrentItem], _meta.m_key);
                        auto _value = RuntimeReflection::get_variant_value(m_aTypes[m_uiCurrentItem], _meta.m_value);

                        if(static_cast<uint16_t>(_key.m_variant.m_traits) & static_cast<uint16_t>(RuntimeReflection::property_traits::is_enum))
                        {
                            // if(_key.m_variant.m_ullTypeID == typeid(Duckvil::RuntimeReflection::GeneratedMeta).hash_code())
                            // {
                            //     auto _x = RuntimeReflection::get_type({ "Duckvil", "RuntimeReflection" });
                            //     auto _x2 = RuntimeReflection::get_enum_handle(_x, _key.m_variant.m_ullTypeID);

                            //     if(_x2.m_ID != -1)
                            //     {
                            //         auto _x3 = RuntimeReflection::get_enum_element_handle(_x, _x2, *static_cast<Duckvil::RuntimeReflection::GeneratedMeta*>(_key.m_variant.m_pData));

                            //         ImGui::Text(RuntimeReflection::get_enum_element(_x, _x2, _x3).m_sName);
                            //     }

                            //     // Duckvil::RuntimeReflection::GeneratedMeta _m = *static_cast<Duckvil::RuntimeReflection::GeneratedMeta*>(_key.m_variant.m_pData);

                            //     // if(_m == Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated)
                            //     // {
                            //     //     ImGui::Text("Auto-generated");
                            //     // }
                            // }

                            RuntimeReflection::__duckvil_resource_enum_t _enumHandle = { -1 };
                            RuntimeReflection::__duckvil_resource_ntype_t _typeHandle = { -1 };

                            const auto& _enum = RuntimeReflection::get_enum(_key.m_variant.m_ullTypeID, &_typeHandle, &_enumHandle);

                            if(_enumHandle.m_ID != -1 && _typeHandle.m_ID != -1)
                            {
                                auto _x3 = RuntimeReflection::get_enum_element_handle(_typeHandle, _enumHandle, *static_cast<Duckvil::RuntimeReflection::GeneratedMeta*>(_key.m_variant.m_pData));

                                ImGui::Text(RuntimeReflection::get_enum_element(_typeHandle, _enumHandle, _x3).m_sName);
                            }
                        }
                    }
                }
            }

            {
                auto _properties = RuntimeReflection::get_properties(m_heap, m_aTypes[m_uiCurrentItem]);
                Memory::Stack<const char*> _propertiesNames;

                m_heap.Allocate(_propertiesNames, _properties.Size());

                for(uint32_t i = 0; i < _properties.Size(); ++i)
                {
                    auto _prop = RuntimeReflection::get_property(m_aTypes[m_uiCurrentItem], _properties[i]);

                    _propertiesNames.Allocate(_prop.m_sName);
                }

                ImGui::Combo("Properties", &m_uiCurrentPropertyItem, (const char**)((uint8_t*)_propertiesNames.m_pContainer + sizeof(Memory::fixed_stack_allocator)), _properties.Size());
            }
        }

        ImGui::Separator();

        ImGui::Text("Types: %i", _typesCount);
        ImGui::Text("Functions: %i", _functionsCount);
        ImGui::Text("Properties: %i", _propertiesCount);
        ImGui::Text("Metas: %i", _metasCount);

        ImGui::End();
    }

}}