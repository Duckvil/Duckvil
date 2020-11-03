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

#include "Duckvil/Duckvil.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

// template <typename Type, typename Allocator>
// struct BaseTest
// {

// };

// template <typename Type, typename Allocator>
// struct Test : public BaseTest<Type, Allocator> { };

// struct alloc1
// {

// };

// struct alloc2
// {

// };

// template <typename Type>
// struct Test<Type, alloc1>
// {
//     Test(alloc1 aa, Type a)
//     {
//         printf("int\n");
//     }
// };

// template <typename Type>
// Test(alloc1, Type) -> Test<Type, alloc1>;

// template <typename Type>
// struct Test<Type, alloc2>
// {
//     Test(alloc2 aa, Type a)
//     {
//         printf("float\n");
//     }
// };

// template <typename Type>
// Test(alloc2, Type) -> Test<Type, alloc2>;

#include "Memory/FreeList.h"

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);

    Duckvil::Memory::FreeList _free(_memoryInterface, &_mainMemoryAllocator, 1024);

    Duckvil::Memory::Vector<float> _vec;

    _free.Allocate(_vec, 2);

    float* _a = (float*)_vec.Allocate(10.f);
    float* _b = (float*)_vec.Allocate(20.f);
    float* _c = (float*)_vec.Allocate(40.f);

    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    Duckvil::__ftable* _engine = duckvil_init(_memoryInterface, _free_list);
    Duckvil::__data _engineData = {};

    _engine->init(&_engineData, _memoryInterface, _free_list);
    _engine->start(&_engineData, _engine);

    return 0;
}