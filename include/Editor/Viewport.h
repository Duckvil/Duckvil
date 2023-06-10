#pragma once

#include "Graphics/Renderer/Renderer.h"

#include "glm/glm.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

#include "stb/stb_image.h"

#include "flecs/flecs.h"

#include "Graphics/MeshComponent.h"
#include "Graphics/TransformComponent.h"

#include "Utils/Utils.h"

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Editor {

    struct viewport
    {
        uint32_t m_shaderID;
        uint32_t m_fbo;
        uint32_t m_fboTextureObject;
        uint32_t m_entityIdTextureObject;
        uint32_t m_textureID;
        uint32_t m_transformID;
        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::quat m_rotation;
        glm::vec3 m_position;

        uint32_t m_uiWidth;
        uint32_t m_uiHeight;

        flecs::world* m_ecs;
        flecs::query<Graphics::MeshComponent, Graphics::TransformComponent> m_rendererQuery;

        int m_iBackgroundValue = 0;
        bool m_bFirstTime = true;
    };

    static inline void reset(viewport* _pViewport)
    {
        _pViewport->m_projection = glm::perspective(70.f, static_cast<float>(_pViewport->m_uiWidth) / static_cast<float>(_pViewport->m_uiHeight), 0.1f, 1000.f);
    }

    static void recreate_viewport(
        viewport* _pViewport,
        const Memory::ftable* _pMemory,
        Memory::free_list_allocator* _pAllocator,
        Graphics::Renderer::renderer_ftable* _pRenderer,
        Graphics::Renderer::renderer_data* _pRendererData,
        int _uiWidth,
        int _uiHeight
    )
    {
        if(!_pViewport->m_bFirstTime)
        {
            _pRenderer->m_fnDestroyTexture(_pMemory, _pAllocator, _pRendererData, _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_fboTextureObject)[0]);
            _pRenderer->m_fnDestroyTexture(_pMemory, _pAllocator, _pRendererData, _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_entityIdTextureObject)[0]);
            _pRenderer->m_fnDestroyFramebuffer(_pMemory, _pAllocator, _pRendererData, _pViewport->m_fbo);
        }

        _pViewport->m_bFirstTime = false;

        {
            GLfloat _filtes[] = {GL_LINEAR};
            void* _data[] = { 0 };
            GLint _internalFormats[] = { GL_RGBA };
            GLenum _formats[] = { GL_RGBA };
            GLenum _types[] = { GL_UNSIGNED_BYTE };

            _pViewport->m_fboTextureObject =
                _pRenderer->m_fnCreateTextureObject(
                    _pMemory,
                    _pAllocator,
                    _pRendererData,
                    Graphics::Renderer::texture_object_descriptor
                    {
                        GL_TEXTURE_2D,
                        _filtes,
                        _uiWidth, _uiHeight,
                        _data,
                        1,
                        _internalFormats,
                        _formats,
                        _types
                    }
                );
        }

        {
            GLfloat _filtes[] = { GL_LINEAR };
            void* _data[] = { 0 };
            GLint _internalFormats[] = { GL_R32UI };
            GLenum _formats[] = { GL_RED_INTEGER };
            GLenum _types[] = { GL_UNSIGNED_INT };

            _pViewport->m_entityIdTextureObject =
                _pRenderer->m_fnCreateTextureObject(
                    _pMemory,
                    _pAllocator,
                    _pRendererData,
                    Graphics::Renderer::texture_object_descriptor
                    {
                        GL_TEXTURE_2D,
                        _filtes,
                        _uiWidth, _uiHeight,
                        _data,
                        1,
                        _internalFormats,
                        _formats,
                        _types
                    }
                );
        }

        GLenum _attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        GLuint _textures[2] =
        {
            _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_fboTextureObject)[0],
            _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_entityIdTextureObject)[0]
        };

        _pViewport->m_fbo =
            _pRenderer->m_fnCreateFramebuffer(
                _pMemory,
                _pAllocator,
                _pRendererData,
                Graphics::Renderer::framebuffer_descriptor
                {
                    GL_FRAMEBUFFER,
                    _attachments,
                    2,
                    _textures,
                    GL_TEXTURE_2D,
                    _uiWidth, _uiHeight
                }
        );
    }

    static void setup_viewport(
        viewport* _pViewport,
        const Memory::ftable* _pMemory,
        Memory::free_list_allocator* _pAllocator,
        Graphics::Renderer::renderer_ftable* _pRenderer,
        Graphics::Renderer::renderer_data* _pRendererData,
        flecs::world* _pECS)
    {
        Utils::string _cwd = DUCKVIL_CWD;

        ecs_os_set_api_defaults();

        _pViewport->m_ecs = _pECS;

        _pViewport->m_shaderID =
            _pRenderer->m_fnCreateShader(
                _pMemory,
                _pAllocator,
                _pRendererData,
                _cwd / "resource/shader/test.vs",
                _cwd / "resource/shader/test.fs"
            );

        _pViewport->m_rendererQuery = _pECS->query<Graphics::MeshComponent, Graphics::TransformComponent>();

        recreate_viewport(
            _pViewport,
            _pMemory,
            _pAllocator,
            _pRenderer,
            _pRendererData,
            1920, 1080
        );

        int _x, _y, _bytesPerPixels;
        unsigned char* _textureData = stbi_load(_cwd / "resource/texture/test.jpg", &_x, &_y, &_bytesPerPixels, 4);

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

        reset(_pViewport);

        _pViewport->m_position = glm::vec3(0, 0, -5);
        _pViewport->m_rotation = glm::quat(0, 0, 0, 1);
    }

    static void render_viewport(
        viewport* _pViewport,
        const Memory::ftable* _pMemory,
        Memory::free_list_allocator* _pAllocator,
        Graphics::Renderer::renderer_ftable* _pRenderer,
        Graphics::Renderer::renderer_data* _pRendererData)
    {
        {
            ZoneScopedN("BindFramebuffer");

            glActiveTexture( GL_TEXTURE0 + 0 );
            glActiveTexture( GL_TEXTURE0 + 1 );
            Graphics::Renderer::bind_framebuffer(_pMemory, _pRendererData, _pViewport->m_fbo);
        }

        Graphics::Renderer::viewport(_pMemory, _pRendererData, _pViewport->m_uiWidth, _pViewport->m_uiHeight);
        Graphics::Renderer::clear_color(_pMemory, _pRendererData, glm::vec4(0.1f, 0.1f, 0.1f, 1));
        Graphics::Renderer::clear(_pMemory, _pRendererData, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Graphics::Renderer::clear_attachment(_pMemory, _pRendererData, _pRenderer->m_fnGetTextures(_pRendererData, _pViewport->m_entityIdTextureObject)[0], GL_RED_INTEGER, GL_INT, &_pViewport->m_iBackgroundValue);

        {
            ZoneScopedN("BindShader");

            Graphics::Renderer::bind_shader(_pMemory, _pRendererData, _pViewport->m_shaderID);
        }

        {
            ZoneScopedN("ECS Render");

            _pViewport->m_rendererQuery.each([_pViewport, _pMemory, _pRendererData](const Graphics::MeshComponent& _mesh, const Graphics::TransformComponent& _transform)
            {
                const glm::mat4& _model = glm::translate(_transform.m_position) * glm::toMat4(_transform.m_rotation) * glm::scale(_transform.m_scale);

                Graphics::Renderer::bind_texture(_pMemory, _pRendererData, _pViewport->m_textureID, 0);

                const glm::vec3& _position = _pViewport->m_position;
                const glm::quat& _rotation = _pViewport->m_rotation;

                _pViewport->m_view = glm::lookAt(
                        _position,
                        _position + (_rotation * glm::vec3(0, 0, 1)),
                        (_rotation * glm::vec3(0, 1, 0)));

                Graphics::Renderer::set_uniform(
                    _pMemory,
                    _pRendererData,
                    _pViewport->m_transformID,
                    _pViewport->m_projection * _pViewport->m_view * _model
                );

                Graphics::Renderer::draw(_pMemory, _pRendererData, _mesh.m_uiID);
            });
        }
    }

}}