#include "Parser/AST.h"

#include "PlugNPlay/Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Generator.h"

#include <fstream>

int main(int argc, char* argv[])
{
    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _reflectionModule("RuntimeReflection.dll");
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory.dll");
    Duckvil::PlugNPlay::__module_information _parser("Parser.dll");

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
    std::filesystem::path _lastPath;
    uint32_t _index = 0;

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
        _astData.m_aUserDefines.push_back("slot");
        _astData.m_aUserDefines.push_back("DUCKVIL_RESOURCE_DECLARE");

        _ast->ast_generate(&_astData, _lexerFtable, _data);
        _ast->ast_print(_astData);

        Duckvil::RuntimeReflection::__generator_data _generatorData;
        std::filesystem::path _relativePath = std::filesystem::relative(_path.path(), std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include");
        std::filesystem::path _pluginDirectory = _relativePath;

    // At least it is working...
        while(_pluginDirectory.has_parent_path())
        {
            _pluginDirectory = _pluginDirectory.parent_path();
        }

        if(_lastPath == "")
        {
            _lastPath = _pluginDirectory;
        }

        if(_lastPath != _pluginDirectory)
        {
            std::ofstream _file(std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__" / _lastPath / "plugin_info.cpp");

            _file << "#include \"RuntimeReflection/Recorder.h\"\n";
            _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(" << _index << ")";

            _file.close();

            _index = 0;
            _lastPath = _pluginDirectory;
        }

        strcpy(_generatorData.m_sInclude, _relativePath.string().c_str());
        _generatorData.m_uiRecorderIndex = _index++;

        std::filesystem::path _generatePath = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__" / _relativePath;

        if(!std::filesystem::exists(_generatePath.parent_path()))
        {
            std::filesystem::create_directories(_generatePath.parent_path());
        }

        _generatePath.replace_extension(".generated.cpp");

        _generatorFtable->generate(&_generatorData, _generatePath.string().c_str(), _astData);
    }

    std::ofstream _file(std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__" / _lastPath / "plugin_info.cpp");

    _file << "#include \"RuntimeReflection/Recorder.h\"\n";
    _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(" << _index << ")";

    _file.close();

    _index = 1;

    return 0;
}