#pragma once

#include "Utils/Macro.h"

#include "Memory/SlotArray.h"

#include "Graphics/Renderer/CommandBuffer.h"

#include "Window/IWindow.h"

#include "glew/include/GL/glew.h"

#include "glm/glm.hpp"

#include "flecs/flecs.h"

#include "Utils/Graphics.h"

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
        renderer_op_code_clear_attachment,
        renderer_op_code_viewport,

        renderer_op_code_read_pixel,
        renderer_op_code_fbo_read_pixel,

        renderer_op_code_frame_buffer_reader_read,
        renderer_op_code_frame_buffer_reader_update,

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
        GLint* m_internalFormats;
        GLenum* m_formats;
        GLenum* m_types;
    };

    struct framebuffer_descriptor
    {
        GLenum m_target;
        GLenum* m_aAttachments;
        uint32_t m_uiCount;
        GLuint* m_aTextures;
        GLenum m_textureTarget;
        GLsizei m_width;
        GLsizei m_height;
    };

    struct vertex_buffer_object_descriptor
    {
        uint32_t m_uiTypeSize; // Size of single vertex
        const void* m_pData; // Pointer to data
        uint16_t m_usNumber; // Count of single vertex type
        GLenum m_target;
        uint32_t m_uiCount;
        GLenum m_type;

        vertex_buffer_object_descriptor(GLenum _target, uint32_t _uiTypeSize, const void* _pData, uint32_t _uiCount, uint16_t _usNumber = 1) :
            m_target(_target),
            m_uiTypeSize(_uiTypeSize),
            m_pData(_pData),
            m_usNumber(_usNumber),
            m_uiCount(_uiCount)
        {

        }

        template <typename Type>
        vertex_buffer_object_descriptor(GLenum _target, Type* _pData, uint32_t _uiCount, uint16_t _usNumber = 1) :
            vertex_buffer_object_descriptor(
                _target,
                static_cast<uint32_t>(sizeof(Type) / _usNumber),
                static_cast<void*>(_pData),
                _uiCount,
                _usNumber
            )
        {

        }

        template <typename Type>
        vertex_buffer_object_descriptor(GLenum _target, const std::vector<Type>& _pData, uint16_t _usNumber = 1) :
            vertex_buffer_object_descriptor(
                _target,
                static_cast<uint32_t>(sizeof(Type) / _usNumber),
                &_pData[0],
                _pData.size(),
                _usNumber
            )
        {

        }
    };

    struct vertex_array_object_descriptor
    {
        uint32_t m_uiVBO_Count; // Number of vertex buffer objects
        vertex_buffer_object_descriptor* m_aVBO; // Array of vertex buffer objects
        // uint32_t m_uiCount; // Count of vertices
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

    using frame_buffer_reader = Duckvil::Utils::Graphics::FrameBufferReader;

    DUCKVIL_SLOT_ARRAY_DECLARE(shader);
    DUCKVIL_SLOT_ARRAY_DECLARE(texture);
    DUCKVIL_SLOT_ARRAY_DECLARE(texture_object);
    DUCKVIL_SLOT_ARRAY_DECLARE(framebuffer);
    DUCKVIL_SLOT_ARRAY_DECLARE(vertex_array_object);
    DUCKVIL_SLOT_ARRAY_DECLARE(uniform);
    DUCKVIL_SLOT_ARRAY_DECLARE(frame_buffer_reader);

    struct renderer_data
    {
        DUCKVIL_SLOT_ARRAY(shader) m_shader;
        DUCKVIL_SLOT_ARRAY(texture) m_texture;
        DUCKVIL_SLOT_ARRAY(texture_object) m_textureObject;
        DUCKVIL_SLOT_ARRAY(framebuffer) m_fbo;
        DUCKVIL_SLOT_ARRAY(vertex_array_object) m_vao;
        DUCKVIL_SLOT_ARRAY(uniform) m_uniform;
        DUCKVIL_SLOT_ARRAY(frame_buffer_reader) m_frameBufferReader;

        command_buffer m_pCommandBuffer;
        Memory::free_list_allocator* m_pAllocator;

        flecs::world* m_ecs;
    };

    struct renderer_ftable
    {
        bool (*m_fnInit)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, renderer_data* _pData);
        void (*m_fnUpdate)(const Memory::ftable* _pMemoryInterface, renderer_data* _pData);
        void (*m_fnSubmitCommandBuffer)(const Memory::ftable* _pMemoryInterface, renderer_data* _pData);

        void (*m_fnBindAsRenderTarget)();

        uint32_t (*m_fnCreateShader)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const char* _sVertexFilename, const char* _sFragmentFilename);
        uint32_t (*m_fnCreateFrameBufferReader)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiTargetWidth, uint32_t _uiTargetHeight, frame_buffer_reader::Format _format, frame_buffer_reader::Type _type);
    	uint32_t (*m_fnCreateTexture)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_descriptor& _descriptor);
        uint32_t (*m_fnCreateTextureObject)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_object_descriptor& _descriptor);
        uint32_t (*m_fnCreateFramebuffer)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const framebuffer_descriptor& _descriptor);
        uint32_t (*m_fnCreateVAO)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const vertex_array_object_descriptor& _descriptor);

        void (*m_fnDestroyTexture)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID);
        void (*m_fnDestroyFramebuffer)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID);

        uint32_t (*m_fnGetUniformLocation)(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID, const char* _sName);

        void* (*m_fnGetTexture)(renderer_data* _pData, uint32_t _uiID);
        GLuint* (*m_fnGetTextures)(renderer_data* _pData, uint32_t _uiID);
    };

    static inline void bind_shader(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _shaderID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_shader,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _shaderID);
            }
        );
    }

    static inline void bind_texture(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _textureID, uint32_t _unit)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_texture,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureID);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _unit);
            }
        );
    }

    static inline void bind_texture_object(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _textureObjectID, uint32_t _textureID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_texture_object,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureObjectID);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _textureID);
            }
        );
    }

    static inline void bind_framebuffer(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _framebufferID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_bind_framebuffer,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _framebufferID);
            }
        );
    }

    static inline void read_pixel_colors(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t m_uiAttachmentIndex, uint32_t _uiX, uint32_t _uiY, void (*_fnCallback)(uint32_t _uiValue))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_read_pixel,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), m_uiAttachmentIndex);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiX);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiY);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _fnCallback);
            }
        );
    }

    static inline void read_pixel(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t m_uiAttachmentIndex, uint32_t _uiX, uint32_t _uiY, uint32_t _uiFBO, void (*_fnCallback)(uint32_t _uiValue))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_fbo_read_pixel,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), m_uiAttachmentIndex);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiX);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiY);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiFBO);
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _fnCallback);
            }
        );
    }

    static inline void draw(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _vaoID)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_draw,
            {
                Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _vaoID);
            }
        );
    }

    static inline void clear_color(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, const glm::vec4& _color = glm::vec4(0, 0, 0, 0))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear_color,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _color);
        });
    }

    static inline void clear(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _mask)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _mask);
        });
    }

    static inline void clear_attachment(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiTextureID, GLenum _format, GLenum _type, void* _pValue)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_clear_attachment,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiTextureID);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _format);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _type);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _pValue);
        });
    }

    static inline void viewport(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiWidth, uint32_t _uiHeight)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_viewport,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiWidth);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHeight);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const int& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_int);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const float& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_float);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec2& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec2);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec3& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec3);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::vec4& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_vec4);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void set_uniform(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiLocation, const glm::mat4& _value)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_set_uniform,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiLocation);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), uniform_type_mat4);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _value);
        });
    }

    static inline void frame_buffer_reader_read(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiHandle, uint32_t _uiFBOHandle, uint32_t _uiSourceWidth, uint32_t _uiSourceHeight, uint8_t _u8Attachment)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_frame_buffer_reader_read,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHandle);
			Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiFBOHandle);
			Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiSourceWidth);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiSourceHeight);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _u8Attachment);
        });
    }

    static inline void frame_buffer_reader_update(const Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiHandle, void (*_fnCallback)(void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_frame_buffer_reader_update,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHandle);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _fnCallback);
        });
    }

    /*static inline void pixels_read(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiHandle)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_pixels_read,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHandle);
        });
    }

    static inline void pixels_read(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiHandle, uint32_t _uiFBO, uint32_t _uiWidth, uint32_t _uiHeight)
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_pixels_read_fbo,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHandle);
			Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiFBO);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiWidth);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHeight);
        });
    }

    static inline void pixels_process(Memory::ftable* _pMemoryInterface, renderer_data* _pData, uint32_t _uiHandle, void (*_fnCallback)(void* _pUserData, void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset))
    {
        DUCKVIL_RENDERER_PUSH_COMMAND(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), renderer_op_code_pixels_process,
        {
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _uiHandle);
            Duckvil::Graphics::Renderer::command_buffer_write(_pMemoryInterface, _pData->m_pAllocator, (&_pData->m_pCommandBuffer), _fnCallback);
        });
    }*/

}}}

DUCKVIL_EXPORT const Duckvil::Graphics::Renderer::renderer_ftable* duckvil_graphics_renderer_init();