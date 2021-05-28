#pragma once

#include "Graphics/Renderer/Renderer.h"

#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

#include "stb/stb_image.h"

namespace Duckvil { namespace Editor {

    struct viewport
    {
        uint32_t m_shaderID;
        uint32_t m_meshID;
        uint32_t m_fbo;
        uint32_t m_fboTextureObject;
        uint32_t m_textureID;
        uint32_t m_transformID;
        glm::mat4 m_transform;
        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::quat m_rotation;
        glm::vec3 m_position;
    };

    static void setup_viewport(
        viewport* _pViewport,
        Memory::ftable* _pMemory,
        Memory::free_list_allocator* _pAllocator,
        Graphics::Renderer::renderer_ftable* _pRenderer,
        Graphics::Renderer::renderer_data* _pRendererData)
    {
        _pViewport->m_shaderID =
            _pRenderer->m_fnCreateShader(
                _pMemory,
                _pAllocator,
                _pRendererData,
                "F:/Projects/C++/Duckvil/resource/shader/test.vs",
                "F:/Projects/C++/Duckvil/resource/shader/test.fs"
            );

        glm::vec3 _vertices[] =
        {
            glm::vec3(-0.5, -0.5, 0),
            glm::vec3(0, 0.5, 0),
            glm::vec3(0.5, -0.5, 0)
        };

        glm::vec2 _texCoords[] =
        {
            glm::vec2(0, 0),
            glm::vec2(0.5, 1),
            glm::vec2(1, 0)
        };

        uint32_t _indices[] =
        {
            0, 1, 2
        };

        Graphics::Renderer::vertex_buffer_object_descriptor _desc[] =
        {
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _vertices, 3),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ARRAY_BUFFER, _texCoords, 2),
            Graphics::Renderer::vertex_buffer_object_descriptor(GL_ELEMENT_ARRAY_BUFFER, _indices)
        };

        _pViewport->m_meshID = _pRenderer->m_fnCreateVAO(
            _pMemory,
            _pAllocator,
            _pRendererData,
            Graphics::Renderer::vertex_array_object_descriptor
            {
                3,
                _desc,
                3
            }
        );

        GLfloat _filtes[1] = { GL_LINEAR };
        void* _data[1] = { 0 };

        _pViewport->m_fboTextureObject =
            _pRenderer->m_fnCreateTextureObject(
                _pMemory,
                _pAllocator,
                _pRendererData,
                Graphics::Renderer::texture_object_descriptor
            {
                GL_TEXTURE_2D,
                _filtes,
                1920, 1080,
                _data,
                1
            });

        GLenum _attachments[1] = { GL_COLOR_ATTACHMENT0 };

        _pViewport->m_fbo =
            _pRenderer->m_fnCreateFramebuffer(
                _pMemory,
                _pAllocator,
                _pRendererData,
                Graphics::Renderer::framebuffer_descriptor
                {
                    GL_DRAW_FRAMEBUFFER,
                    _attachments,
                    1,
                    _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_fboTextureObject),
                    GL_TEXTURE_2D
                }
        );

        int _x, _y, _bytesPerPixels;
        unsigned char* _textureData = stbi_load("F:/Projects/C++/Duckvil/resource/texture/test.jpg", &_x, &_y, &_bytesPerPixels, 4);

        _pViewport->m_textureID =
            _pRenderer->m_fnCreateTexture(
                _pMemory,
                _pAllocator,
                _pRendererData,
                Graphics::Renderer::texture_descriptor
                {
                    GL_TEXTURE_2D,
                    GL_LINEAR,
                    _x,
                    _y,
                    _textureData
                }
            );

        stbi_image_free(_textureData);

        _pViewport->m_transformID =
            _pRenderer->m_fnGetUniformLocation(
                _pMemory,
                _pAllocator,
                _pRendererData,
                _pViewport->m_shaderID,
                "transform"
            );

        _pViewport->m_transform = glm::translate(glm::vec3(0, 0, 0)) * glm::toMat4(glm::quat(0, 0, 0, 1)) * glm::scale(glm::vec3(1, 1, 1));
        _pViewport->m_projection = glm::perspective(70.f, 1920.f / 1080.f, 0.1f, 1000.f);

        _pViewport->m_position = glm::vec3(0, 0, -5);
        _pViewport->m_rotation = glm::quat(0, 0, 0, 1);
    }

    static void render_viewport(
        viewport* _pViewport,
        Memory::ftable* _pMemory,
        Memory::free_list_allocator* _pAllocator,
        Graphics::Renderer::renderer_ftable* _pRenderer,
        Graphics::Renderer::renderer_data* _pRendererData)
    {
        Graphics::Renderer::bind_framebuffer(_pMemory, _pRendererData, _pViewport->m_fbo);
        Graphics::Renderer::viewport(_pMemory, _pRendererData, 1920, 1080);
        Graphics::Renderer::clear_color(_pMemory, _pRendererData, glm::vec4(0, 0, 0, 1));
        Graphics::Renderer::clear(_pMemory, _pRendererData, GL_COLOR_BUFFER_BIT);
        Graphics::Renderer::bind_shader(_pMemory, _pRendererData, _pViewport->m_shaderID);
        Graphics::Renderer::set_uniform(_pMemory, _pRendererData, _pViewport->m_transformID, _pViewport->m_projection * glm::lookAt(_pViewport->m_position, _pViewport->m_position + (_pViewport->m_rotation * glm::vec3(0, 0, 1)), (_pViewport->m_rotation * glm::vec3(0, 1, 0))) * _pViewport->m_transform);
        Graphics::Renderer::bind_texture(_pMemory, _pRendererData, _pViewport->m_textureID, 0);
        Graphics::Renderer::draw(_pMemory, _pRendererData, _pViewport->m_meshID);
    }

}}