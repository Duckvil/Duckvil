#include "Graphics/Renderer/Renderer.h"

#include "glew/include/GL/glew.h"

#include <fstream>
#include <string>

#include "SDL2/include/SDL.h"

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

    uint32_t impl_renderer_create_shader(const char* _sVertexFilename, const char* _sFragmentFilename)
    {
        GLuint _program = glCreateProgram();
        GLuint _shaders[2] = { create_shader(load_shader(_sVertexFilename), GL_VERTEX_SHADER), create_shader(load_shader(_sFragmentFilename), GL_FRAGMENT_SHADER) };

        for(uint32_t i = 0; i < 2; ++i)
        {
            glAttachShader(_program, _shaders[i]);
        }

        glLinkProgram(_program);
        check_shader_error(_program, GL_LINK_STATUS, true, "Error: Program link failed!");

        glValidateProgram(_program);
        check_shader_error(_program, GL_VALIDATE_STATUS, true, "Error: Program validate failed!");

        return _program;
    }



    bool impl_renderer_init(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, Window::IWindow* _pWindow, renderer_data* _pData)
    {
        _pData->m_shader = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, shader);
        _pData->m_pCommandBuffer = command_buffer_new(_pMemoryInterface, _pAllocator);
        _pData->m_pAllocator = _pAllocator;

        SDL_GL_MakeCurrent((SDL_Window*)_pWindow->GetWindow(), _pWindow->GetContext());

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK)
        {
            throw std::exception("Failed to init GLEW!");
            // printf("%s\n", glewGetErrorString(err));
        }

        _pData->m_shaderID = duckvil_slot_array_insert(
            _pMemoryInterface,
            _pAllocator,
            _pData->m_shader,
            impl_renderer_create_shader(
                "F:/Projects/C++/Duckvil/resource/shader/test.vs",
                "F:/Projects/C++/Duckvil/resource/shader/test.fs"
            )
        );

        return true;
    }

    void impl_renderer_submit_command_buffer(Memory::ftable* _pMemoryInterface, renderer_data* _pData)
    {
        Memory::byte_buffer_seek_to_begin(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands);

        for(uint32_t i = 0; i < _pData->m_pCommandBuffer.m_uiCommandCount; ++i)
        {
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
            case renderer_op_code_clear_color:
            {
                glClearColor(0, 0, 0, 0);
            }
                break;
            default:
                break;
            }
        }

        Memory::byte_buffer_seek_to_begin(_pMemoryInterface, _pData->m_pCommandBuffer.m_pCommands);
        _pData->m_pCommandBuffer.m_uiCommandCount = 0;
    }

    void impl_renderer_update(Memory::ftable* _pMemoryInterface, renderer_data* _pData)
    {
        clear_color(_pMemoryInterface, _pData);
        bind_shader(_pMemoryInterface, _pData, _pData->m_shaderID);

        impl_renderer_submit_command_buffer(_pMemoryInterface, _pData);

        command_buffer_clear(_pMemoryInterface, &_pData->m_pCommandBuffer);
    }

}}}

Duckvil::Graphics::Renderer::renderer_ftable* duckvil_graphics_renderer_init()
{
    static Duckvil::Graphics::Renderer::renderer_ftable _result = { 0 };

    _result.m_fnInit = &Duckvil::Graphics::Renderer::impl_renderer_init;
    _result.m_fnUpdate = &Duckvil::Graphics::Renderer::impl_renderer_update;
    _result.m_fnSubmitCommandBuffer = &Duckvil::Graphics::Renderer::impl_renderer_submit_command_buffer;

    _result.m_fnCreateShader = &Duckvil::Graphics::Renderer::impl_renderer_create_shader;

    return &_result;
}