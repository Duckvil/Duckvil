#include "Window/SDL_Window.h"

#include "SDL2/include/SDL.h"

namespace Duckvil { namespace Window {

    WindowSDL::WindowSDL(Event::Pool<Event::mode::buffered>* _pEventsPool) :
        m_pEvents(_pEventsPool)
    {

    }

    WindowSDL::~WindowSDL()
    {

    }

    bool WindowSDL::Create(const char* _sTitle, uint32_t _uiWidth, uint32_t _uiHeight)
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_pWindow = SDL_CreateWindow(_sTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _uiWidth, _uiHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        if(m_pWindow == nullptr)
        {
            return false;
        }

        m_pContext = SDL_GL_CreateContext((SDL_Window*)m_pWindow);

        SDL_GL_MakeCurrent((SDL_Window*)m_pWindow, m_pContext);
        SDL_GL_SetSwapInterval(-1);

        m_uiWidth = _uiWidth;
        m_uiHeight = _uiHeight;

        return true;
    }

    void WindowSDL::Refresh() const
    {
        SDL_GL_SwapWindow((SDL_Window*)m_pWindow);

        FrameMarkStart("Sleep");
        SDL_Delay(static_cast<int>((1.f / 60.f) * 1000));
        FrameMarkEnd("Sleep");
    }

    void WindowSDL::PopulateEvents()
    {
        SDL_Event _event;

        while(SDL_PollEvent(&_event))
        {
            m_fnProcessEvents(&_event);

            switch(_event.type)
            {
            case SDL_QUIT:
                m_pEvents->Broadcast(CloseEvent{ });
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                m_pEvents->Broadcast(ResizeEvent{ _event.window.data1, _event.window.data2 });
                break;
            case SDL_KEYDOWN:
                switch(_event.key.keysym.sym)
                {
                case SDLK_w:
                    m_pEvents->Broadcast(KeyDownEvent{ key_w });
                    break;
                case SDLK_s:
                    m_pEvents->Broadcast(KeyDownEvent{ key_s });
                    break;
                case SDLK_a:
                    m_pEvents->Broadcast(KeyDownEvent{ key_a });
                    break;
                case SDLK_d:
                    m_pEvents->Broadcast(KeyDownEvent{ key_d });
                    break;
                case SDLK_k:
                    m_pEvents->Broadcast(KeyDownEvent{ key_k });
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch(_event.key.keysym.sym)
                {
                case SDLK_w:
                    m_pEvents->Broadcast(KeyUpEvent{ key_w });
                    break;
                case SDLK_s:
                    m_pEvents->Broadcast(KeyUpEvent{ key_s });
                    break;
                case SDLK_a:
                    m_pEvents->Broadcast(KeyUpEvent{ key_a });
                    break;
                case SDLK_d:
                    m_pEvents->Broadcast(KeyUpEvent{ key_d });
                    break;
                case SDLK_k:
                    m_pEvents->Broadcast(KeyUpEvent{ key_k });
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                m_pEvents->Broadcast(MouseMotionEvent{ _event.motion.x, _event.motion.y });
                break;
            default:
                break;
            }
        }
    }

    void* WindowSDL::GetWindow() const
    {
        return m_pWindow;
    }

    void* WindowSDL::GetContext() const
    {
        return m_pContext;
    }

    void WindowSDL::SetMousePosition(int _iX, int _iY, bool _bGlobal)
    {
        if(_bGlobal)
        {
            SDL_WarpMouseGlobal(_iX, _iY);
        }
        else
        {
            SDL_WarpMouseInWindow(static_cast<SDL_Window*>(m_pWindow), _iX, _iY);
        }
    }

    void WindowSDL::SetProcessEventsCallback(void (*_fnProcessEvents)(void* _pData))
    {
        m_fnProcessEvents = _fnProcessEvents;
    }

}}