#define STB_IMAGE_IMPLEMENTATION
#include <cstdio>
#include <filesystem>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"
#include "Memory/MemoryDebugger.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"

#include "Utils/Benchmark.h"

#include "Engine/Duckvil.h"

#include "Event/ImmediateChannel.h"

enum class Options
{
    PROJECT,
    SERVER,
    CLIENT
};

Duckvil::Utils::CommandArgumentsParser::Descriptor g_pDescriptors[] =
{
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::PROJECT, "project"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::SERVER, "server"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::CLIENT, "client")
};

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _engineModule("Engine");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);
    _module.load(&_engineModule);

    Duckvil::Memory::init_callback duckvil_memory_init;
    Duckvil::__ftable* (*duckvil_init)();

    _module.get(_memoryModule, "duckvil_memory_init", reinterpret_cast<void**>(&duckvil_memory_init));
    _module.get(_engineModule, "duckvil_init", reinterpret_cast<void**>(&duckvil_init));

    Duckvil::Memory::ftable* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::linear_allocator* _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024 * 2);

#ifdef DUCKVIL_MEMORY_DEBUGGER
    duckvil_memory_debug_info _memoryDebug = {};

    _memoryDebug.m_pAllocator = _mainMemoryAllocator;
    _memoryDebug.m_pParent = nullptr;
    _memoryDebug.m_allocatorType = duckvil_memory_allocator_type_linear;

    _mainMemoryAllocator->m_pDebugData = &_memoryDebug;
    _mainMemoryAllocator->m_fnOnAllocate = Duckvil::Utils::lambda([&](Duckvil::Memory::allocator* _pParentAllocator, Duckvil::Memory::allocator* _pAllocator, duckvil_memory_allocator_type _type)
    {
        duckvil_memory_debug_info* _parentDebugInfo = (duckvil_memory_debug_info*)_pParentAllocator->m_pDebugData;
        duckvil_memory_debug_info* _currentInfo = new duckvil_memory_debug_info{ _pAllocator, _parentDebugInfo, _type };

        _parentDebugInfo->m_aOther.push_back(_currentInfo);

        _pAllocator->m_pDebugData = _currentInfo;
    });
    _mainMemoryAllocator->m_fnOnDeallocate = Duckvil::Utils::lambda([&](Duckvil::Memory::allocator* _pParentAllocator, Duckvil::Memory::allocator* _pAllocator)
    {
        duckvil_memory_debug_info* _parentDebugInfo = (duckvil_memory_debug_info*)_pParentAllocator->m_pDebugData;
        duckvil_memory_debug_info* _currentInfo = _pAllocator->m_pDebugData;

        for(auto it = _parentDebugInfo->m_aOther.begin(); it != _parentDebugInfo->m_aOther.end(); ++it)
        {
            if(*it == _currentInfo)
            {
                _parentDebugInfo->m_aOther.erase(it);

                break;
            }
        }

        delete _currentInfo;
    });
#endif

    Duckvil::Memory::free_list_allocator* _free_list = _memoryInterface->m_fnLinearAllocateFreeListAllocator(_mainMemoryAllocator, 1000 * 1024 * 2);

    Duckvil::__ftable* _engine = duckvil_init();
    Duckvil::__data _engineData = { .m_ecs = flecs::world() };

    if(_parser[Options::SERVER].m_bIsSet)
    {
        _engineData.m_bIsServer = true;
    }
    else if(_parser[Options::CLIENT].m_bIsSet)
    {
        _engineData.m_bIsClient = true;
    }

    _engine->init(&_engineData, _memoryInterface, _free_list);

#ifdef DUCKVIL_MEMORY_DEBUGGER
    _engineData.m_pMemoryDebugger = &_memoryDebug;
#endif

    // load project if specified

    if(_parser[Options::PROJECT].m_bIsSet)
    {
        _engineData.m_bufferedEventPool.Broadcast(Duckvil::ProjectManager::LoadProjectEvent{ _parser[Options::PROJECT].m_sResult });
    }

    _engine->start(&_engineData, _engine);

    _memoryInterface->m_fnFreeListClear_(_free_list);
    _memoryInterface->m_fnLinearClear(_mainMemoryAllocator);

    return 0;
}