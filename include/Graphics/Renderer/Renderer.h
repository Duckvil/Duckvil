#pragma once

#include "Utils/Macro.h"

#include "Memory/SlotArray.h"

#include "Graphics/Renderer/CommandBuffer.h"

#include "Window/IWindow.h"

#define DUCKVIL_RENDERER_PUSH_COMMAND(memory, allocator, cb, op_code, ...) \
    Duckvil::Graphics::Renderer::command_buffer_write(memory, allocator, cb, op_code); \
    __VA_ARGS__ \
    cb->m_uiCommandCount++

namespace Duckvil { namespace Graphics { namespace Renderer {

    enum renderer_op_code
    {
        renderer_op_code_bind_shader = 0,
        renderer_op_code_clear_color,

        renderer_op_code_none
    };

    typedef uint32_t shader;

    DUCKVIL_SLOT_ARRAY_DECLARE(shader);

    struct renderer_data
    {
        DUCKVIL_SLOT_ARRAY(shader) m_shader;
        command_buffer m_pCommandBuffer;
        Memory::free_list_allocator* m_pAllocator;

        uint32_t m_shaderID;
    };

    struct renderer_ftable
    {
        bool (*m_fnInit)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, renderer_data* _pData);
        void (*m_fnUpdate)(Memory::ftable* _pMemoryInterface, renderer_data* _pData);
        void (*m_fnSubmitCommandBuffer)(Memory::ftable* _pMemoryInterface, renderer_data* _pData);

        uint32_t (*m_fnCreateShader)(const char* _sVertexFilename, const char* _sFragmentFilename);
    };

    static inline void bind_shader(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _shaderID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_shader,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _shaderID);
            }
        );
    }

    static inline void clear_color(Memory::ftable* _pMemoryInterface, renderer_data* _pData)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear_color);
    }

}}}

DUCKVIL_EXPORT Duckvil::Graphics::Renderer::renderer_ftable* duckvil_graphics_renderer_init();