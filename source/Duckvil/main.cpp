#include <cstdio>
#include <filesystem>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"

#include "Utils/Benchmark.h"

#include "Engine/Duckvil.h"

#include "Event/ImmediateChannel.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

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
    Duckvil::__ftable* (*duckvil_init)(Duckvil::Memory::IMemory*, Duckvil::Memory::__free_list_allocator*);

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);
    _module.get(_engineModule, "duckvil_init", (void**)&duckvil_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    Duckvil::Memory::debug_info _memoryDebug = {};
    std::stack<Duckvil::Memory::debug_info> _stack;

    _memoryDebug.m_pAllocator = &_mainMemoryAllocator;
    _memoryDebug.m_pParent = nullptr;

    _mainMemoryAllocator.m_pParentAllocator = nullptr;
    _mainMemoryAllocator.m_pDebugData = &_memoryDebug;
    _mainMemoryAllocator.m_fnOnAllocate = Duckvil::Event::cify([&](Duckvil::Memory::__allocator* _pAllocator, Duckvil::Memory::allocator_type _type)
    {
        Duckvil::Memory::__allocator* _pParentAllocator = _pAllocator->m_pParentAllocator;

        if(_pParentAllocator != nullptr)
        {
            Duckvil::Memory::debug_info* _parentDebugInfo = (Duckvil::Memory::debug_info*)_pParentAllocator->m_pDebugData;

            _stack.push(Duckvil::Memory::debug_info{ _pAllocator, _parentDebugInfo, _type });

            Duckvil::Memory::debug_info* _currentInfo = &_stack.top();

            _parentDebugInfo->m_aOther.push_back(_currentInfo);

            _pAllocator->m_pDebugData = _currentInfo;
        }
    });

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);

    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);
    Duckvil::Memory::__fixed_vector_allocator* _vec = _memoryInterface->m_fnAllocateFixedVectorAllocator(&_mainMemoryAllocator, 1 * 1024, 16);

    Duckvil::__ftable* _engine = duckvil_init(_memoryInterface, _free_list);
    Duckvil::__data _engineData = {};

    _engine->init(&_engineData, _memoryInterface, _free_list);
    _engine->start(&_engineData, _engine);

    return 0;
}