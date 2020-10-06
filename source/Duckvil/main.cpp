#include <cstdio>
#include <filesystem>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "Parser/AST.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::AutoLoader _auto(DUCKVIL_OUTPUT);

    _auto.LoadAll();

    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _dbModule("RuntimeDatabase");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);
    _module.load(&_dbModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    {
        Duckvil::PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");
        duckvil_runtime_reflection_init_callback _rr_init;

        _module.load(&_runtimeReflectionModule);
        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_rr_init);

        Duckvil::RuntimeReflection::__functions* _rr_data = _rr_init(_memoryInterface, _free_list);
        _rr_data->m_fnInit(_memoryInterface, _free_list, _rr_data);

        struct test
        {
            test(int a) :
                a(a)
            {

            }

            int a;
            int b;
        };

        struct test2
        {

        };

        Duckvil::RuntimeReflection::__duckvil_resource_type_t a = Duckvil::RuntimeReflection::record_type<test>(_memoryInterface, _free_list, _rr_data, "test");
        Duckvil::RuntimeReflection::__duckvil_resource_type_t b = Duckvil::RuntimeReflection::record_type<test2>(_memoryInterface, _free_list, _rr_data, "test2");

        Duckvil::RuntimeReflection::__duckvil_resource_constructor_t a_cons = Duckvil::RuntimeReflection::record_constructor<test, int>(_memoryInterface, _free_list, _rr_data);

        //test* aaaaa = (test*)Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data->m_pData, a_cons, 10);
        //test* bbbbb = (test*)Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data->m_pData, a_cons, 20);

        test* bbbbb = (test*)Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data->m_pData, "test", 20);

        Duckvil::RuntimeReflection::__duckvil_resource_property_t prop = Duckvil::RuntimeReflection::record_property<test, int>(_memoryInterface, _free_list, _rr_data, offsetof(test, a), "a");
        
        int* aa = (int*)Duckvil::RuntimeReflection::get_property(_rr_data->m_pData, "a", bbbbb);

        aa = (int*)Duckvil::RuntimeReflection::get_property(_rr_data->m_pData, "a", bbbbb);

        _rr_data->m_fnRecordProperty(_memoryInterface, _free_list, _rr_data->m_pData, a, typeid(int).hash_code(), "b", offsetof(test, b));
    }

    {
        Duckvil::PlugNPlay::__module_information _parser("Parser");
        Duckvil::Parser::__lexer_ftable* (*duckvil_lexer_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);
        Duckvil::Parser::__ast_ftable* (*duckvil_ast_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);

        _module.load(&_parser);
        _module.get(_parser, "duckvil_lexer_init", (void**)&duckvil_lexer_init);
        _module.get(_parser, "duckvil_ast_init", (void**)&duckvil_ast_init);

        Duckvil::Parser::__lexer_ftable* _funcs = duckvil_lexer_init(_memoryInterface, _free_list);
        Duckvil::Parser::__lexer_data _data;

        _funcs->load_file(&_data, (std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include/Parser/Lexer.h").string().c_str());

        Duckvil::Parser::__ast_ftable* _ast = duckvil_ast_init(_memoryInterface, _free_list);

        Duckvil::Parser::__ast _ast_data;

        _ast->generate_ast(&_ast_data, _funcs, _data);

        // std::string _token;

        // while(_funcs->next_token(&_data, &_token))
        // {
        //     if(!_funcs->space(_data))
        //         printf("%s", _token.c_str());
        //     else
        //         printf(" ");

        //     if(_data.m_cPendingSymbol != 0)
        //         printf("%c", _data.m_cPendingSymbol);
        // }
    }

    // duckvil_init(_memoryInterface, _free_list);

    // Duckvil::RuntimeDatabase::__data* _db = (Duckvil::RuntimeDatabase::__data*)Duckvil::PlugNPlay::instantiate_plugin(_module, _dbModule);

    // _db->m_pAllocator = (Duckvil::Memory::__free_list_allocator*)1;
    // Duckvil::RuntimeDatabase::__data* _db2 = (Duckvil::RuntimeDatabase::__data*)Duckvil::PlugNPlay::instantiate_plugin(_module, _dbModule);

    return 0;
}