#include "Graphics/Renderer/Renderer.h"

#include <fstream>
#include <string>

#include "SDL2/include/SDL.h"

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Graphics { namespace Renderer {

    static void check_shader_error(GLuint _uiShader, GLuint _uiFlag, bool _bIsProgram, const char* _sErrorMessage)
    {
        GLint _success = 0;
        GLchar _error[1024] = { 0 };

        if(_bIsProgram)
        {
            glGetProgramiv(_uiShader, _uiFlag, &_success);
        }
        else
        {
            glGetShaderiv(_uiShader, _uiFlag, &_success);
        }

        if(_success == GL_FALSE)
        {
            if(_bIsProgram)
            {
                glGetProgramInfoLog(_uiShader, sizeof(_error), nullptr, _error);
            }
            else
            {
                glGetShaderInfoLog(_uiShader, sizeof(_error), nullptr, _error);
            }

            printf("%s: %s\n", _sErrorMessage, _error);
        }
    }

    static std::string load_shader(const char* _sFilename)
    {
        std::ifstream _file;

        _file.open(_sFilename);

        std::string _output;
        std::string _line;

        if(_file.is_open())
        {
            while(_file.good())
            {
                getline(_file, _line);

                _output.append(_line + "\n");
            }
        }

        return _output;
    }

    static GLuint create_shader(const std::string& _sSource, GLenum _shaderType)
    {
        GLuint _shader = glCreateShader(_shaderType);

        if(_shader == 0)
        {
            printf("Error: Shader creation failed!\n");
        }

        const char* _source = _sSource.c_str();
        int _length = _sSource.length();

        glShaderSource(_shader, 1, &_source, &_length);
        glCompileShader(_shader);

        check_shader_error(_shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!");

        return _shader;
    }

    uint32_t impl_renderer_create_shader(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const char* _sVertexFilename, const char* _sFragmentFilename)
    {
        GLuint _program = glCreateProgram();
        GLuint _shaders[2] = { create_shader(load_shader(_sVertexFilename), GL_VERTEX_SHADER), create_shader(load_shader(_sFragmentFilename), GL_FRAGMENT_SHADER) };

        for(uint32_t i = 0; i < 2; ++i)
        {
            glAttachShader(_program, _shaders[i]);
        }

        glBindAttribLocation(_program, 0, "position");
        glBindAttribLocation(_program, 1, "texCoord");

        glLinkProgram(_program);
        check_shader_error(_program, GL_LINK_STATUS, true, "Error: Program link failed!");

        glValidateProgram(_program);
        check_shader_error(_program, GL_VALIDATE_STATUS, true, "Error: Program validate failed!");

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_shader,
            _program
        );
    }

    uint32_t impl_renderer_create_frame_buffer_reader(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiTargetWidth, uint32_t _uiTargetHeight, frame_buffer_reader::Format _format, frame_buffer_reader::Type _type)
    {
        frame_buffer_reader _buffer(_pMemoryInterface, _pAllocator, _uiTargetWidth, _uiTargetHeight, _format, _type);

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_frameBufferReader,
            _buffer
        );
    }

    uint32_t impl_renderer_create_texture(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_descriptor& _descriptor)
    {
        GLuint _texture = -1;

        glGenTextures(1, &_texture);
        glBindTexture(_descriptor.m_target, _texture);

        glTexParameteri(_descriptor.m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(_descriptor.m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameterf(_descriptor.m_target, GL_TEXTURE_MIN_FILTER, _descriptor.m_filter);
        glTexParameterf(_descriptor.m_target, GL_TEXTURE_MAG_FILTER, _descriptor.m_filter);

        glTexImage2D(_descriptor.m_target, 0, GL_RGBA, _descriptor.m_width, _descriptor.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _descriptor.m_pData);

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_texture,
            _texture
        );
    }

    uint32_t impl_renderer_create_texture_object(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const texture_object_descriptor& _descriptor)
    {
        GLuint* _texture = static_cast<GLuint*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(GLuint) * _descriptor.m_uiCount, 8));

        glCreateTextures(_descriptor.m_target, _descriptor.m_uiCount, _texture);

        for(uint32_t i = 0; i < _descriptor.m_uiCount; ++i)
        {
            glBindTexture(_descriptor.m_target, _texture[i]);

            glTexParameterf(_descriptor.m_target, GL_TEXTURE_MIN_FILTER, _descriptor.m_filter[i]);
            glTexParameterf(_descriptor.m_target, GL_TEXTURE_MAG_FILTER, _descriptor.m_filter[i]);

            glTexImage2D(_descriptor.m_target, 0, _descriptor.m_internalFormats[i], _descriptor.m_width, _descriptor.m_height, 0, _descriptor.m_formats[i], _descriptor.m_types[i], _descriptor.m_pData[i]);
        }

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_textureObject,
            texture_object{ _texture, _descriptor.m_uiCount }
        );
    }

    uint32_t impl_renderer_create_framebuffer(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const framebuffer_descriptor& _descriptor)
    {
        GLuint _framebuffer = -1;
        GLuint _renderbuffer = -1;

        glCreateFramebuffers(1, &_framebuffer);
        glBindFramebuffer(_descriptor.m_target, _framebuffer);

        GLenum* _drawBuffers = static_cast<GLenum*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(GLenum) * _descriptor.m_uiCount, 8));

        bool _hasDepth = false;

        for(uint32_t i = 0; i < _descriptor.m_uiCount; ++i)
        {
            if(_descriptor.m_aAttachments[i] == GL_DEPTH_ATTACHMENT)
            {
                _drawBuffers[i] = GL_NONE;
                _hasDepth = true;
            }
            else
            {
                _drawBuffers[i] = _descriptor.m_aAttachments[i];
            }

            if(_descriptor.m_aAttachments[i] == GL_NONE)
            {
                continue;
            }

            glFramebufferTexture2D(_descriptor.m_target, _descriptor.m_aAttachments[i], _descriptor.m_textureTarget, _descriptor.m_aTextures[i], 0);
        }

        if(!_hasDepth)
        {
            glGenRenderbuffers(1, &_renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _descriptor.m_width, _descriptor.m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer);
        }

        glDrawBuffers(_descriptor.m_uiCount, _drawBuffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("ERROR!\n");

            return -1;
        }

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_fbo,
            framebuffer{ _framebuffer }
        );
    }

    uint32_t impl_renderer_create_vao(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, const vertex_array_object_descriptor& _descriptor)
    {
        GLuint _vao = -1;
        uint32_t _drawCount = 0;
        GLuint* _vbo = static_cast<GLuint*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(GLuint) * _descriptor.m_uiVBO_Count, 8));

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(_descriptor.m_uiVBO_Count, _vbo);

        for(uint32_t i = 0; i < _descriptor.m_uiVBO_Count; ++i)
        {
            const vertex_buffer_object_descriptor& _vboDesc = _descriptor.m_aVBO[i];

            if(_vboDesc.m_target == GL_ARRAY_BUFFER && _vboDesc.m_type == GL_UNSIGNED_INT)
            {
                glBindBuffer(_vboDesc.m_target, _vbo[i]);
                glBufferData(_vboDesc.m_target, _vboDesc.m_uiTypeSize * _vboDesc.m_usNumber * _vboDesc.m_uiCount, _vboDesc.m_pData, GL_STATIC_DRAW);
                glEnableVertexAttribArray(i);
                glVertexAttribIPointer(i, _vboDesc.m_usNumber, _vboDesc.m_type, 0, 0);
            }
            else if(_vboDesc.m_target == GL_ARRAY_BUFFER)
            {
                glBindBuffer(_vboDesc.m_target, _vbo[i]);
                glBufferData(_vboDesc.m_target, _vboDesc.m_uiTypeSize * _vboDesc.m_usNumber * _vboDesc.m_uiCount, _vboDesc.m_pData, GL_STATIC_DRAW);
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, _vboDesc.m_usNumber, _vboDesc.m_type, GL_FALSE, 0, 0);
            }
            else if(_vboDesc.m_target == GL_ELEMENT_ARRAY_BUFFER)
            {
                glBindBuffer(_vboDesc.m_target, _vbo[i]);
                glBufferData(_vboDesc.m_target, _vboDesc.m_uiTypeSize * _vboDesc.m_uiCount, _vboDesc.m_pData, GL_STATIC_DRAW);

                _drawCount = _vboDesc.m_uiCount;
            }
        }

        glBindVertexArray(0);

        return duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_vao,
            vertex_array_object{ _vao, _drawCount }
        );
    }

    void impl_renderer_bind_as_render_target()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    uint32_t impl_renderer_get_uniform_location(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID, const char* _sName)
    {
        const auto& _shader = DUCKVIL_SLOT_ARRAY_GET(_pData->m_shader, _uiID);
        const auto& _uniformLocation = glGetUniformLocation(_shader, _sName);

        return duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _pData->m_uniform, _uniformLocation);
    }

    void* impl_renderer_get_texture(renderer_data* _pData, uint32_t _uiID)
    {
        return DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_texture, _uiID);
    }

    GLuint* impl_renderer_get_textures(renderer_data* _pData, uint32_t _uiID)
    {
        const auto& _textureObject = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_textureObject, _uiID);

        return _textureObject->m_pTexture;
    }

    void impl_renderer_destroy_texture(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID)
    {
        glDeleteTextures(1, &_uiID);
    }

    void impl_renderer_destroy_framebuffer(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, renderer_data* _pData, uint32_t _uiID)
    {
        glDeleteFramebuffers(1, &_uiID);
    }



    bool impl_renderer_init(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, renderer_data* _pData)
    {
        _pData->m_shader = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, shader);
        _pData->m_texture = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, texture);
        _pData->m_textureObject = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, texture_object);
        _pData->m_fbo = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, framebuffer);
        _pData->m_vao = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, vertex_array_object);
        _pData->m_uniform = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, uniform);
        _pData->m_frameBufferReader = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, frame_buffer_reader);

        _pData->m_pCommandBuffer = command_buffer_new(_pMemoryInterface, _pAllocator);
        _pData->m_pAllocator = _pAllocator;

        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(_pWindow->GetWindow()), _pWindow->GetContext());

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK)
        {
            throw std::runtime_error((const char*)glewGetErrorString(err));
            // printf("%s\n", glewGetErrorString(err));
        }

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        return true;
    }

    void impl_renderer_submit_command_buffer(const Memory::ftable* _pMemoryInterface, renderer_data* _pData)
    {
        Memory::byte_buffer_seek_to_begin(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands);

        auto& _count = _pData->m_pCommandBuffer.m_uiCommandCount;
        
        while(_count > 0)
        {
            _count--;

            renderer_op_code _command = renderer_op_code_none;

            Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_command);

            switch(_command)
            {
            case renderer_op_code_bind_shader:
            {
                uint32_t _shaderID = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_shaderID);

                uint32_t _shader = DUCKVIL_SLOT_ARRAY_GET(_pData->m_shader, _shaderID);

                glUseProgram(_shader);
            }
                break;
            case renderer_op_code_bind_texture:
            {
                uint32_t _textureID = -1;
                uint32_t _unit = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_textureID);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_unit);

                uint32_t _texture = DUCKVIL_SLOT_ARRAY_GET(_pData->m_texture, _textureID);

                glActiveTexture(GL_TEXTURE0 + _unit);
                glBindTexture(GL_TEXTURE_2D, _texture);
            }
                break;
            case renderer_op_code_bind_texture_object:
            {
                uint32_t _textureObjectID = -1;
                uint32_t _textureID = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_textureObjectID);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_textureID);

                texture_object _texture = DUCKVIL_SLOT_ARRAY_GET(_pData->m_textureObject, _textureObjectID);

                glBindTexture(GL_TEXTURE_2D, _texture.m_pTexture[_textureID]);
            }
                break;
            case renderer_op_code_bind_framebuffer:
            {
                uint32_t _framebufferID = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_framebufferID);

                framebuffer _fbo = DUCKVIL_SLOT_ARRAY_GET(_pData->m_fbo, _framebufferID);

                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo.m_FBO);
            }
                break;
            case renderer_op_code_read_pixel:
            {
                uint32_t _attachmentIndex;
                uint32_t _x;
                uint32_t _y;
                void (*_fnCallback)(uint32_t);
                uint32_t _value = 0;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_attachmentIndex);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_x);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_y);
                
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_fnCallback);

                glReadBuffer(GL_COLOR_ATTACHMENT0 + _attachmentIndex);
                glReadPixels(_x, _y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &_value);

                _fnCallback(_value);
            }
                break;
            case renderer_op_code_fbo_read_pixel:
            {
                uint32_t _attachmentIndex;
                uint32_t _x;
                uint32_t _y;
                void (*_fnCallback)(uint32_t);
                uint32_t _value = 0;
                uint32_t _fboHandle = 0;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_attachmentIndex);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_x);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_y);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_fboHandle);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_fnCallback);

                framebuffer _fbo = DUCKVIL_SLOT_ARRAY_GET(_pData->m_fbo, _fboHandle);

                glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo.m_FBO);
                glReadBuffer(GL_COLOR_ATTACHMENT0 + _attachmentIndex);
                glReadPixels(_x, _y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &_value);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

                _fnCallback(_value);
            }
				break;
            case renderer_op_code_frame_buffer_reader_read:
            {
                uint32_t _handle;
                uint32_t _fboHandle;
                uint32_t _sourceWidth;
                uint32_t _sourceHeight;
                uint8_t _attachment;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_handle);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_fboHandle);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_sourceWidth);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_sourceHeight);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_attachment);

                frame_buffer_reader& _reader = DUCKVIL_SLOT_ARRAY_GET(_pData->m_frameBufferReader, _handle);
                framebuffer _fbo = DUCKVIL_SLOT_ARRAY_GET(_pData->m_fbo, _fboHandle);

                _reader.Read(_fbo.m_FBO, _sourceWidth, _sourceHeight, _attachment);
            }
				break;
            case renderer_op_code_frame_buffer_reader_update:
            {
                uint32_t _handle;
                void (*_fnCallback)(void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset);

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_handle);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_fnCallback);

                frame_buffer_reader& _reader = DUCKVIL_SLOT_ARRAY_GET(_pData->m_frameBufferReader, _handle);

                _reader.Update(_fnCallback);
            }
				break;
            case renderer_op_code_draw:
            {
                uint32_t _vaoID = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_vaoID);

                vertex_array_object _vao = DUCKVIL_SLOT_ARRAY_GET(_pData->m_vao, _vaoID);

                glBindVertexArray(_vao.m_vao);
                // glDrawArrays(GL_TRIANGLES, 0, _vao.m_uiDrawCount);
                glDrawElements(GL_TRIANGLES, _vao.m_uiDrawCount, GL_UNSIGNED_INT, 0);
            }
                break;
            case renderer_op_code_clear_color:
            {
                glm::vec4 _color;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_color);

                glClearColor(_color.x, _color.y, _color.z, _color.w);
            }
                break;
            case renderer_op_code_clear:
            {
                uint32_t _mask = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_mask);

                glClear(_mask);
            }
                break;
            case renderer_op_code_clear_attachment:
            {
                uint32_t _textureID = -1;
                GLenum _format;
                GLenum _type;
                void* _value;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_textureID);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_format);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_type);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                glClearTexImage(_textureID, 0, _format, _type, _value);
            }
                break;
            case renderer_op_code_viewport:
            {
                uint32_t _width = -1;
                uint32_t _height = -1;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_width);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_height);

                glViewport(0, 0, _width, _height);
            }
                break;
            case renderer_op_code_set_uniform:
            {
                uint32_t _id = -1;
                uniform_type _type;

                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_id);
                Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_type);

                GLuint _location = DUCKVIL_SLOT_ARRAY_GET(_pData->m_uniform, _id);

                switch(_type)
                {
                case uniform_type_int:
                    {
                        int _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniform1i(_location, _value);
                    }
                    break;
                case uniform_type_float:
                    {
                        float _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniform1f(_location, _value);
                    }
                    break;
                case uniform_type_vec2:
                    {
                        glm::vec2 _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniform2f(_location, _value.x, _value.y);
                    }
                    break;
                case uniform_type_vec3:
                    {
                        glm::vec3 _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniform3f(_location, _value.x, _value.y, _value.z);
                    }
                    break;
                case uniform_type_vec4:
                    {
                        glm::vec4 _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniform4f(_location, _value.x, _value.y, _value.z, _value.w);
                    }
                    break;
                case uniform_type_mat4:
                    {
                        glm::mat4 _value;

                        Memory::byte_buffer_read(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands, &_value);

                        glUniformMatrix4fv(_location, 1, GL_FALSE, &_value[0][0]);
                    }
                    break;
                };
            }
                break;
            default:
                break;
            }
        }

        //Memory::byte_buffer_seek_to_begin(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands);
        //_pData->m_pCommandBuffer.m_uiCommandCount = 0;
    }

    void impl_renderer_update(const Memory::ftable* _pMemoryInterface, renderer_data* _pData)
    {
        {
            ZoneScopedN("Submit renderer command buffer");

            impl_renderer_submit_command_buffer(_pMemoryInterface, _pData);
        }

        {
            ZoneScopedN("Renderer command buffer clear");

            command_buffer_clear(_pMemoryInterface, &_pData->m_pCommandBuffer);
        }
    }

}}}

const Duckvil::Graphics::Renderer::renderer_ftable* duckvil_graphics_renderer_init()
{
    static Duckvil::Graphics::Renderer::renderer_ftable _result = { 0 };

    _result.m_fnInit = &Duckvil::Graphics::Renderer::impl_renderer_init;
    _result.m_fnUpdate = &Duckvil::Graphics::Renderer::impl_renderer_update;
    _result.m_fnSubmitCommandBuffer = &Duckvil::Graphics::Renderer::impl_renderer_submit_command_buffer;

    _result.m_fnBindAsRenderTarget = &Duckvil::Graphics::Renderer::impl_renderer_bind_as_render_target;

    _result.m_fnCreateShader = &Duckvil::Graphics::Renderer::impl_renderer_create_shader;
    _result.m_fnCreateFrameBufferReader = &Duckvil::Graphics::Renderer::impl_renderer_create_frame_buffer_reader;
    _result.m_fnCreateTexture = &Duckvil::Graphics::Renderer::impl_renderer_create_texture;
    _result.m_fnCreateTextureObject = &Duckvil::Graphics::Renderer::impl_renderer_create_texture_object;
    _result.m_fnCreateFramebuffer = &Duckvil::Graphics::Renderer::impl_renderer_create_framebuffer;
    _result.m_fnCreateVAO = &Duckvil::Graphics::Renderer::impl_renderer_create_vao;

    _result.m_fnDestroyTexture = &Duckvil::Graphics::Renderer::impl_renderer_destroy_texture;
    _result.m_fnDestroyFramebuffer = &Duckvil::Graphics::Renderer::impl_renderer_destroy_framebuffer;

    _result.m_fnGetUniformLocation = &Duckvil::Graphics::Renderer::impl_renderer_get_uniform_location;

    _result.m_fnGetTexture = &Duckvil::Graphics::Renderer::impl_renderer_get_texture;
    _result.m_fnGetTextures = &Duckvil::Graphics::Renderer::impl_renderer_get_textures;

    return &_result;
}