#pragma once

#include "Memory/Memory.h"
#include "Memory/SlotArray.h"
#include "Memory/FreeList.h"
#include "Memory/Vector.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"

#include "Logger/Logger.h"

#include "Engine/Time.h"
#include "Engine/ReflectionFlags.h"
#include "Engine/ISystem.h"
#include "ECS/Entity.h"

#include "HotReloader/RuntimeCompilerSystem.h"

#include "Event/ImmediatePool.h"
#include "Event/BufferedPool.h"

#include "Window/IWindow.h"

#include "Window/Events/CloseEvent.h"
#include "Window/Events/ResizeEvent.h"
#include "Window/Events/KeyDownEvent.h"
#include "Window/Events/KeyUpEvent.h"
#include "Window/Events/MouseMotionEvent.h"
#include "Window/Events/SetMousePositionEvent.h"

// #include "Renderer/Renderer.h"

// #include "Renderer/ImguiRenderer/Renderer.h"

#include "Editor/Editor.h"
#include "Editor/Widget.h"

#include "Thread/ThreadPool.h"

#include "Memory/SmartPointer/SharedPointer.h"

#include "Graphics/Renderer/Renderer.h"

#include "glm/gtx/quaternion.hpp"

#include "Editor/Viewport.h"

#include "flecs/flecs.h"

#include "ProjectManager/ProjectManager.h"

#include "Engine/Events/RequestSystemEvent.h"

#include "ProjectManager/Events/LoadProjectEvent.h"

#include "Network/IServer.h"
#include "Network/IClient.h"

#include "ECS/EntityFactory.h"

#include "Serializer/EntitySerializerSystem.h"

#include "Engine/ScriptComponent.h"

#include "ScriptEngine/Language.h"

#include "DependencyInjection/IDependencyResolver.h"

namespace Duckvil {

    struct system
    {
    // TODO: Currently it is workaround for member callbacks, need to be fixed(Reflection recorder)
        typedef void (ISystem::*UpdateCallback)(double _dDelta);
        typedef bool (ISystem::*InitCallback)();

        RuntimeReflection::__duckvil_resource_type_t m_type;
        //DUCKVIL_TRACK_KEEPER_OBJECT(ISystem) m_pTrackKeeper;
        void* m_pObject;
        // ISystem* m_pISystem;
        bool m_bIsHot;
        system::UpdateCallback m_fnUpdateCallback;
        system::InitCallback m_fnInitCallback;
    };

    struct __data
    {
        bool m_bRunning;

#ifdef DUCKVIL_MEMORY_DEBUGGER
        duckvil_memory_debug_info* m_pMemoryDebugger;
#endif

        const Memory::ftable* m_pMemory;
        Memory::free_list_allocator* m_pHeap;

        Memory::FreeList m_heap; // General use memory
        Memory::FreeList m_objectsHeap; // Memory for hot objects
        Memory::FreeList m_eventsHeap;

        size_t m_ullLastTimeUsed;

        logger_ftable _loggerFTable;
        logger_data _loggerData;

        const RuntimeReflection::__ftable* m_pRuntimeReflection;
        const RuntimeReflection::__recorder_ftable* m_pRuntimeReflectionRecorder;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        // duckvil_recorderd_types* m_aRecordedTypes;
        // size_t m_ullRecordedTypesCount;
        Memory::ThreadsafeVector<duckvil_recorderd_types> m_aRecordedTypes;

        // PlugNPlay::__module_information* m_aLoadedModules;
        // uint32_t m_uiLoadedModulesCount;
        Memory::Vector<PlugNPlay::__module_information> m_aLoadedModules;

        HotReloader::RuntimeCompilerSystem* m_pRuntimeCompiler;
        system::UpdateCallback m_fnRuntimeCompilerUpdate;
        bool (ISystem::*m_fnRuntimeCompilerInit)(bool);

        // __logger_ftable* m_pLogger;
        // __logger_data* m_pLoggerData;
        __logger_channel_data* m_pLoggerChannelData;
        __logger_channel_ftable* m_pLoggerChannel;

        Thread::pool_ftable* m_pThread;
        Thread::pool_data* m_pThreadData;

        __time_ftable m_time;
        __time_data m_timeData;
        double m_dOneSecond;

        Event::Pool<Event::mode::immediate> m_eventPool;
        Event::Pool<Event::mode::buffered> m_windowEventPool;
        Event::Pool<Event::mode::buffered> m_bufferedEventPool;

        Memory::Vector<Duckvil::system> m_aEngineSystems;

        Window::IWindow* m_pWindow;

        Editor::EditorFTable* m_pEditor;
        Editor::ImGuiEditorData* m_pEditorData;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data m_pRendererData;

        flecs::world m_ecs;
        ECS::EntityFactory m_entityFactory;

        flecs::query<Graphics::TransformComponent> m_rendererQuery;
        flecs::query<ScriptComponent> m_scriptsQuery;

        ProjectManager::ftable m_projectManager;
        ProjectManager::data m_projectManagerData;

        Network::IServer* m_pServer;
        bool m_bIsServer;

        Network::IClient* m_pClient;
        bool m_bIsClient;

        uint16_t m_usPort;
        const char* m_sIP;

        ScriptEngine::Language* m_pCSharpLanguage;

        DependencyInjection::IDependencyResolver* m_pDependencyInjector;

        Memory::FreeList m_globalHeap;

        __data() :
            m_ecs(flecs::world()),
            m_entityFactory(m_heap, &m_eventPool, m_ecs)
        {
            m_pClient = nullptr;
            m_pServer = nullptr;
            m_bIsServer = false;
            m_bIsClient = false;
        }
    };

    struct __ftable
    {
        bool (*init)(Duckvil::__data* _pData, const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator);
        bool (*start)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        bool (*stop)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        void (*update)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
    };

}

typedef const Duckvil::__ftable* (*duckvil_init_callback)();

DUCKVIL_EXPORT const Duckvil::__ftable* duckvil_init();