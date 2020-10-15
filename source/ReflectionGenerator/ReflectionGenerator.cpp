#include "Parser/AST.h"

#include "PlugNPlay/Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Generator.h"

int main(int argc, char* argv[])
{
    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _reflectionModule("RuntimeReflection");
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _parser("Parser");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_reflectionModule);
    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback _fnMemoryInit;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&_fnMemoryInit);

    Duckvil::Memory::IMemory* _memoryInterface = _fnMemoryInit();
    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    _module.load(&_parser);

    Duckvil::Parser::__lexer_ftable* (*_lexer_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);
    Duckvil::Parser::__ast_ftable* (*_ast_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);

    _module.get(_parser, "duckvil_lexer_init", (void**)&_lexer_init);
    _module.get(_parser, "duckvil_ast_init", (void**)&_ast_init);

    Duckvil::Parser::__ast_ftable* _ast = _ast_init(_memoryInterface, _free_list);
    Duckvil::Parser::__lexer_ftable* _lexerFtable = _lexer_init(_memoryInterface, _free_list);

    Duckvil::RuntimeReflection::__generator_ftable* (*_runtime_reflection_generator)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);

    _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", (void**)&_runtime_reflection_generator);

    Duckvil::RuntimeReflection::__generator_ftable* _generatorFtable = _runtime_reflection_generator(_memoryInterface, _free_list);

    for(auto& _path : std::filesystem::recursive_directory_iterator(std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include"))
    {
        if(_path.path().extension() != ".h")
        {
            continue;
        }

        Duckvil::Parser::__lexer_data _data;

        _lexerFtable->load_file(&_data, _path.path().string().c_str());

        Duckvil::Parser::__ast _astData;

        _astData.m_aUserDefines.push_back("DUCKVIL_EXPORT");

        _ast->ast_generate(&_astData, _lexerFtable, _data);
        _ast->ast_print(_astData);
    }

// Load each file in specified folder, generate AST and generate reflection based on AST
    {
        Duckvil::Parser::__lexer_data _data;

        _lexerFtable->load_file(&_data, (std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include/Parser/AST.h").string().c_str());

        Duckvil::Parser::__ast _astData;

        _astData.m_aUserDefines.push_back("DUCKVIL_EXPORT");

        _ast->ast_generate(&_astData, _lexerFtable, _data);

        Duckvil::RuntimeReflection::__generator_data _generatorData;// = _generatorFtable->init(_memoryInterface, _free_list);

        strcpy(_generatorData.m_sInclude, "Parser/AST.h");

        _generatorFtable->generate(&_generatorData, (std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__/Parser/AST.generated.cpp").string().c_str(), _astData);
    }

    return 0;
}