#pragma once

#include "Utils/Macro.h"

#include "Memory/SlotArray.h"

#include "Graphics/Renderer/CommandBuffer.h"

#include "Window/IWindow.h"

#include "glew/include/GL/glew.h"

#include "glm/glm.hpp"

#include "flecs/flecs.h"

#define DUCKVIL_RENDERER_PUSH_COMMAND(memory, allocator, cb, op_code, ...) \
    Duckvil::Graphics::Renderer::command_buffer_write(memory, allocator, cb, op_code); \
    __VA_ARGS__ \
    cb->m_uiCommandCount++

namespace Duckvil { namespace Graphics { namespace Renderer {

    enum renderer_op_code
    {
        renderer_op_code_bind_shader = 0,
        renderer_op_code_bind_texture,
        renderer_op_code_bind_texture_object,
        renderer_op_code_bind_framebuffer,

        renderer_op_code_draw,

        renderer_op_code_set_uniform,

        renderer_op_code_clear_color,
        renderer_op_code_clear,
        renderer_op_code_viewport,

        renderer_op_code_none
    };

    enum uniform_type
    {
        uniform_type_int,
        uniform_type_float,
        uniform_type_vec2,
        uniform_type_vec3,
        uniform_type_vec4,
        uniform_type_mat4
    };

    struct texture_descriptor
    {
        GLenum m_target;
        GLfloat m_filter;
        GLsizei m_width;
        GLsizei m_height;
        void* m_pData;
    };

    struct texture_object_descriptor
    {
        GLenum m_target;
        GLfloat* m_filter;
        GLsizei m_width;
        GLsizei m_height;
        void** m_pData;
        uint32_t m_uiCount;
    };

    struct framebuffer_descriptor
    {
        GLenum m_target;
        GLenum* m_aAttachments;
        uint32_t m_uiCount;
        GLuint* m_aTextures;
        GLenum m_textureTarget;
    };

    struct vertex_buffer_object_descriptor
    {
        uint32_t m_uiTypeSize; // Size of single vertex
        void* m_pData; // Pointer to data
        uint16_t m_usNumber; // Count of single vertex type
        GLenum m_target;

        vertex_buffer_object_descriptor(GLenum _target, uint32_t _uiTypeSize, void* _pData, uint16_t _usNumber = 0) :
            m_target(_target),
            m_uiTypeSize(_uiTypeSize),
            m_pData(_pData),
            m_usNumber(_usNumber)
        {

        }

        template <typename Type>
        vertex_buffer_object_descriptor(GLenum _target, Type* _pData, uint16_t _usNumber = 0) :
            m_target(_target),
            m_uiTypeSize(sizeof(Type)),
            m_pData(_pData),
            m_usNumber(_usNumber)
        {

        }
    };

    struct vertex_array_object_descriptor
    {
        uint32_t m_uiVBO_Count; // Number of vertex buffer objects
        vertex_buffer_object_descriptor* m_aVBO; // Array of vertex buffer objects
        uint32_t m_uiCount; // Count of vertices
    };

    typedef uint32_t shader;
    typedef uint32_t texture;
    typedef uint32_t uniform;

    struct texture_object
    {
        GLuint* m_pTexture;
        uint32_t m_uiTextureCount;
    };

    struct framebuffer
    {
        GLuint m_FBO;
    };

    struct vertex_array_object
    {
        GLuint m_vao;
        uint32_t m_uiDrawCount;
    };

    DUCKVIL_SLOT_ARRAY_DECLARE(shader);
    DUCKVIL_SLOT_ARRAY_DECLARE(texture);
    DUCKVIL_SLOT_ARRAY_DECLARE(texture_object);
    DUCKVIL_SLOT_ARRAY_DECLARE(framebuffer);
    DUCKVIL_SLOT_ARRAY_DECLARE(vertex_array_object);
    DUCKVIL_SLOT_ARRAY_DECLARE(uniform);

    struct renderer_data
    {
        DUCKVIL_SLOT_ARRAY(shader) m_shader;
        DUCKVIL_SLOT_ARRAY(texture) m_texture;
        DUCKVIL_SLOT_ARRAY(texture_object) m_textureObject;
        DUCKVIL_SLOT_ARRAY(framebuffer) m_fbo;
        DUCKVIL_SLOT_ARRAY(vertex_array_object) m_vao;
        DUCKVIL_SLOT_ARRAY(uniform) m_uniform;

        command_buffer m_pCommandBuffer;
        Memory::free_list_allocator* m_pAllocator;

        flecs::world* m_ecs;
    };

    struct renderer_ftable
    {
        bool (*m_fnInit)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, renderer_data* _pData);
        void (*m_fnUpdate)(Memory::ftable* _pMemoryInterface, renderer_data* _pData);
        void (*m_fnSubmitCommandBuffer)(Memory::ftable* _pMemoryInterface, renderer_data* _pData);

        void (*m_fnBindAsRenderTarget)();

        uint32_t (*m_fnCreateShader)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const char* _sVertexFilename, const char* _sFragmentFilename);
        uint32_t (*m_fnCreateTexture)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_descriptor& _descriptor);
        uint32_t (*m_fnCreateTextureObject)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_object_descriptor& _descriptor);
        uint32_t (*m_fnCreateFramebuffer)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const framebuffer_descriptor& _descriptor);
        uint32_t (*m_fnCreateVAO)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const vertex_array_object_descriptor& _descriptor);

        uint32_t (*m_fnGetUniformLocation)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID, const char* _sName);

        void* (*m_fnGetTexture)(renderer_data* _pData, uint32_t _uiID);
        GLuint* (*m_fnGetTextures)(renderer_data* _pData, uint32_t _uiID);
    };

    static inline void bind_shader(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _shaderID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_shader,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _shaderID);
            }
        );
    }

    static inline void bind_texture(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _textureID, uint32_t _unit)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_texture,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureID);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _unit);
            }
        );
    }

    static inline void bind_texture_object(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _textureObjectID, uint32_t _textureID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_texture_object,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureObjectID);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureID);
            }
        );
    }

    static inline void bind_framebuffer(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _framebufferID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_framebuffer,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _framebufferID);
            }
        );
    }

    static inline void draw(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _vaoID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_draw,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _vaoID);
            }
        );
    }

    static inline void clear_color(Memory::ftable* _pMemoryInterface, renderer_data* _pData, const glm::vec4& _color = glm::vec4(0, 0, 0, 0))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear_color,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _color);
        });
    }

    static inline void clear(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _mask)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _mask);
        });
    }

    static inline void viewport(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiWidth, uint32_t _uiHeight)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_viewport,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiWidth);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHeight);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const int& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_int);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const float& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_float);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec2& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec2);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec3& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec3);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec4& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec4);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::mat4& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_mat4);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

}}}

DUCKVIL_EXPORT Duckvil::Graphics::Renderer::renderer_ftable* duckvil_graphics_renderer_init();