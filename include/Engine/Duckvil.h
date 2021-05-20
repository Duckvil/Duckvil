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

#include "HotReloader/RuntimeCompilerSystem.h"

#include "Event/ImmediatePool.h"
#include "Event/BufferedPool.h"

#include "Window/IWindow.h"

#include "Window/Events/CloseEvent.h"
#include "Window/Events/ResizeEvent.h"

// #include "Renderer/Renderer.h"

// #include "Renderer/ImguiRenderer/Renderer.h"

#include "Editor/Editor.h"
#include "Editor/Widget.h"

#include "Thread/ThreadPool.h"

#include "Memory/SmartPointer/SharedPointer.h"

#include "Graphics/Renderer/Renderer.h"

namespace Duckvil {

    struct system
    {
    // TODO: Currently it is workaround for member callbacks, need to be fixed(Reflection recorder)
        typedef void (ISystem::*UpdateCallback)();
        typedef bool (ISystem::*InitCallback)();

        RuntimeReflection::__duckvil_resource_type_t m_type;
        DUCKVIL_TRACK_KEEPER_OBJECT(ISystem) m_pTrackKeeper;
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

        Memory::ftable* m_pMemory;
        Memory::free_list_allocator* m_pHeap;

        Memory::FreeList m_heap; // General use memory
        Memory::FreeList m_objectsHeap; // Memory for hot objects
        Memory::FreeList m_eventsHeap;

        size_t m_ullLastTimeUsed;

        logger_ftable _loggerFTable;
        logger_data _loggerData;

        RuntimeReflection::__ftable* m_pRuntimeReflection;
        RuntimeReflection::__recorder_ftable* m_pRuntimeReflectionRecorder;
        RuntimeReflection::__data* m_pRuntimeReflectionData;

        // duckvil_recorderd_types* m_aRecordedTypes;
        // size_t m_ullRecordedTypesCount;
        Memory::Vector<duckvil_recorderd_types> m_aRecordedTypes;

        PlugNPlay::__module_information* m_aLoadedModules;
        uint32_t m_uiLoadedModulesCount;

        HotReloader::RuntimeCompilerSystem* m_pRuntimeCompiler;
        system::UpdateCallback m_fnRuntimeCompilerUpdate;
        system::InitCallback m_fnRuntimeCompilerInit;

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

        Memory::Vector<Duckvil::system> m_aEngineSystems;

        Window::IWindow* m_pWindow;

        // Renderer::RendererFTable* m_pRenderer;
        // Renderer::Imgui::render_data m_renderData;

        Editor::EditorFTable* m_pEditor;
        Editor::ImGuiEditorData* m_pEditorData;

        Graphics::Renderer::renderer_ftable* m_pRenderer;
        Graphics::Renderer::renderer_data m_pRendererData;
    };

    struct __ftable
    {
        bool (*init)(Duckvil::__data* _pData, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator);
        bool (*start)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        bool (*stop)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
        void (*update)(Duckvil::__data* _pData, Duckvil::__ftable* _pFTable);
    };

}

typedef Duckvil::__ftable* (*duckvil_init_callback)();

DUCKVIL_EXPORT Duckvil::__ftable* duckvil_init();