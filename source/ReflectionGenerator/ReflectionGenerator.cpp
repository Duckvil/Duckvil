#include "Parser/AST.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/Meta.h"

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

#include "Engine/ReflectionFlags.h"

#include "Utils/AST.h"
#include "Utils/Utils.h"
#include "Utils/CommandArgumentsParser.h"
#include "Utils/md5.h"

#include <fstream>
#include <cassert>

#include "nlohmann/json.hpp"

std::string load_file_as_string(const std::string& _sFile)
{
    std::ifstream _f(_sFile);

    std::string _str = std::string((std::istreambuf_iterator<char>(_f)), std::istreambuf_iterator<char>());

    _f.close();

    return _str;
}

struct reflection_module
{
    void* m_pObject;
    Duckvil::RuntimeReflection::__duckvil_resource_type_t m_typeHandle;
    Duckvil::RuntimeReflection::__duckvil_resource_function_t m_generateCustomFunctionHandle;
    Duckvil::RuntimeReflection::__duckvil_resource_function_t m_processAST_FunctionHandle;
    Duckvil::RuntimeReflection::__duckvil_resource_function_t m_clearFunctionHandle;
};

std::filesystem::path _relativePath;
Duckvil::Memory::Vector<reflection_module> _aModules;
Duckvil::RuntimeReflection::__data* _runtimeReflectionData;
Duckvil::RuntimeReflection::__ftable* _reflectionFTable;

struct user_data
{
    std::filesystem::path m_path;
};

void generate(std::ofstream& _file, void* _pUserData)
{
    user_data* _userData = static_cast<user_data*>(_pUserData);

    std::string _fileID = _userData->m_path.string();

    _fileID = Duckvil::Utils::replace_all(_fileID, "\\", "_");
    _fileID = Duckvil::Utils::replace_all(_fileID, ".", "_");

    _file << "#include \"Serializer/Runtime/ISerializer.h\"\n\n";
    _file << "#include \"RuntimeReflection/Markers.h\"\n\n";

    std::vector<std::pair<uint32_t, std::vector<std::string>>> _generated;

    for(auto& _module : _aModules)
    {
        if(_module.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member<std::ofstream&, std::vector<std::pair<uint32_t, std::vector<std::string>>>&>(_reflectionFTable, _runtimeReflectionData, _module.m_typeHandle, _module.m_generateCustomFunctionHandle, _module.m_pObject, _file, _generated);
    }

    for(const auto& _generated2 : _generated)
    {
        _file << "#define " << _fileID << "_" << _generated2.first << "_GENERATED_BODY";

        if(_generated2.second.size())
        {
            _file << " \\\n";
        }

        for(uint32_t i = 0; i < _generated2.second.size(); ++i)
        {
            _file << _fileID << "_" << _generated2.first << "_REFLECTION_MODULE_" << _generated2.second[i];

            if(i < _generated2.second.size() - 1)
            {
                _file << " \\\n";
            }
            else
            {
                _file << "\n\n";
            }
        }
    }

    _file << "#undef DUCKVIL_CURRENT_FILE_ID\n";
    _file << "#define DUCKVIL_CURRENT_FILE_ID " << _fileID << "\n";
}

void generate_plugin_info(std::ofstream& _file, const uint32_t& _uiIndex, const std::filesystem::path& _path)
{
    _file << "#include \"RuntimeReflection/Recorder.h\"\n";
    _file << "#include \"Logger/Logger.h\"\n";
    _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(" << _uiIndex << ")\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)\n";
    _file << "{\n";
    _file << "Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);\n";
    _file << "}\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)\n";
    _file << "{\n";
    _file << "Duckvil::logger_make_current(_loggerContext);\n";
    _file << "}\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_heap_context(const Duckvil::Memory::free_list_context& _heapContext)";
    _file << "{\n";
    _file << "Duckvil::Memory::heap_make_current(_heapContext);\n";
    _file << "}\n\n";

    // std::string _moduleName = _path.string();

    // std::transform(_moduleName.begin(), _moduleName.end(), _moduleName.begin(), [](uint8_t _c)
    // {
    //     return std::tolower(_c);
    // });

    // _file << "DUCKVIL_EXPORT const char* DUCKVIL_MODULE_NAME = \"duckvil_" << _moduleName << "_module\";\n";
}

nlohmann::json process_file(Duckvil::Parser::__ast_ftable* _pAST_FTable, Duckvil::Parser::__lexer_ftable* _pLexerFTable, Duckvil::RuntimeReflection::__generator_ftable* _pGeneratorFTable, const std::filesystem::path& _cwd, const std::filesystem::path& _path, const std::filesystem::path& _currentModule, uint32_t _index, bool _bGenerate = false)
{
    if(_path.extension() != ".h")
    {
        return nlohmann::json();
    }

    nlohmann::json _jFile;

    Duckvil::Parser::__lexer_data _data;

    _pLexerFTable->load_file(&_data, _path.string().c_str());

    Duckvil::Parser::__ast _astData;

    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_EXPORT", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "slot", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_RESOURCE_DECLARE", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_GENERATED_BODY", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_RESOURCE", &Duckvil::Utils::user_define_resource_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "TracyLockable", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_SLOT_ARRAY_DECLARE", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_META_CAT", &Duckvil::Utils::user_define_behavior });
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_META_UTIL", &Duckvil::Utils::user_define_behavior });

#ifdef DUCKVIL_HOT_RELOADING
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_HOT_RELOADING", &Duckvil::Utils::user_define_behavior });
#endif

    _relativePath = std::filesystem::relative(_path, _cwd / "include");

    _astData.m_sFile = _relativePath;

    _pAST_FTable->ast_generate(&_astData, _pLexerFTable, _data);
    // _ast->ast_print(_astData);

    for(auto& _reflectionModule : _aModules)
    {
        if(_reflectionModule.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member<Duckvil::Parser::__ast*>(_reflectionFTable, _runtimeReflectionData, _reflectionModule.m_typeHandle, _reflectionModule.m_processAST_FunctionHandle, _reflectionModule.m_pObject, &_astData);
    }

    _jFile["name"] = _currentModule;

    std::filesystem::path _jSource = _relativePath;
    std::filesystem::path _jHeader = _relativePath;

    _jSource.replace_extension(".generated.cpp");
    _jHeader.replace_extension(".generated.h");

    _jFile["generated_h"] = _jHeader;
    _jFile["generated_cpp"] = _jSource;

    // if(_fnProcessJson)
    // {
    //     _fnProcessJson(_jFile);

    //     _index = _jFile["index"].get<uint32_t>();
    // }

    _jFile["index"] = _index;

    Duckvil::RuntimeReflection::__generator_data _generatorData;

    _generatorData.m_uiRecorderIndex = _index;

    if(_relativePath.string().size() < DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX)
    {
        strcpy(_generatorData.m_sInclude, _relativePath.string().c_str());
    }
    else
    {
        assert(false && "Path is too long!");
    }

    printf("Processing: %s\n", _relativePath.string().c_str());

    std::filesystem::path _generatePath = _cwd / "__generated_reflection__" / _relativePath;

    if(!std::filesystem::exists(_generatePath.parent_path()))
    {
        std::filesystem::create_directories(_generatePath.parent_path());
    }

    std::filesystem::path _source = _generatePath;
    std::filesystem::path _header = _generatePath;

    _source.replace_extension(".generated.cpp");
    _header.replace_extension(".generated.h");

    // strcpy(_generatorData.m_sGeneratedHeader, std::filesystem::relative(_header, std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__").string().c_str());

    user_data _userData;

    _userData.m_path = _relativePath;

    if(_bGenerate)
    {
        _pGeneratorFTable->generate(&_generatorData, _source.string().c_str(), _header.string().c_str(), _astData, &generate, &_userData);
    }

    for(auto& _module : _aModules)
    {
        if(_module.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member(_reflectionFTable, _runtimeReflectionData, _module.m_typeHandle, _module.m_clearFunctionHandle, _module.m_pObject);
    }

    return _jFile;
}

enum class Options
{
    CWD,            // Whole project path, all files will be checked, possibly project regeneration will be needed if file was added or removed
                    // Needed for getting '__generated_reflection__' path
    FILE,           // Only if one file has changed, when eg. hot-reloading, but NOT adding or removing files
    IS_RELATIVE,    // If specified file above is relative
    IS_ABSOLUTE,    // If specified file above is relative
    FORCE           // Force regenerate all files in CWD

                    // Maybe some option for specific module? eg. Duckvil/Editor
                    // Then only files in this folder will be processed

                    // Some option to specify purging/cleanup?

                    // TODO: Fix one file processing
};

Duckvil::Utils::CommandArgumentsParser::Descriptor g_pDescriptors[] =
{
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::CWD, "CWD"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::FILE, "file"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IS_RELATIVE, "is_relative"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IS_ABSOLUTE, "is_absolute"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::FORCE, "force")
};

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _argumentsParser(argc, argv);

    if(!_argumentsParser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    printf("CWD: %s\n", _argumentsParser[Options::CWD].m_sResult);

    if(_argumentsParser[Options::IS_ABSOLUTE].m_bIsSet)
    {
        printf("Is absolute\n");
    }
    else if(_argumentsParser[Options::IS_RELATIVE].m_bIsSet)
    {
        printf("Is relative\n");
    }

    if(_argumentsParser[Options::FILE].m_bIsSet)
    {
        printf("File: %s\n", _argumentsParser[Options::FILE].m_sResult);
    }

    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _reflectionModule("RuntimeReflection");
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _parser("Parser");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_reflectionModule);
    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback _fnMemoryInit;

    _module.get(_memoryModule, "duckvil_memory_init", reinterpret_cast<void**>(&_fnMemoryInit));

    Duckvil::Memory::ftable* _memoryInterface = _fnMemoryInit();
    Duckvil::Memory::linear_allocator* _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::free_list_allocator* _free_list = _memoryInterface->m_fnLinearAllocateFreeListAllocator(_mainMemoryAllocator, 512 * 1024);
    Duckvil::Memory::FreeList _heap(_memoryInterface, _free_list);

    _heap.Allocate(_aModules, 1);

    _module.load(&_parser);

    Duckvil::Parser::__lexer_ftable* (*_lexer_init)();
    Duckvil::Parser::__ast_ftable* (*_ast_init)();

    _module.get(_parser, "duckvil_lexer_init", reinterpret_cast<void**>(&_lexer_init));
    _module.get(_parser, "duckvil_ast_init", reinterpret_cast<void**>(&_ast_init));

    Duckvil::Parser::__ast_ftable* _ast = _ast_init();
    Duckvil::Parser::__lexer_ftable* _lexerFtable = _lexer_init();

    Duckvil::RuntimeReflection::__generator_ftable* (*_runtime_reflection_generator)();

    _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", reinterpret_cast<void**>(&_runtime_reflection_generator));

    duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
    duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

    _module.get(_reflectionModule, "duckvil_runtime_reflection_init", reinterpret_cast<void**>(&_runtimeReflectionInit));
    _module.get(_reflectionModule, "duckvil_runtime_reflection_recorder_init", reinterpret_cast<void**>(&_runtimeReflectionRecorderInit));

    _reflectionFTable = _runtimeReflectionInit();
    Duckvil::RuntimeReflection::__recorder_ftable* _reflectionRecorderFTable = _runtimeReflectionRecorderInit();

    _runtimeReflectionData = _reflectionFTable->m_fnInit(_memoryInterface, _free_list, _reflectionFTable);

    Duckvil::RuntimeReflection::__generator_ftable* _generatorFtable = _runtime_reflection_generator();
    std::filesystem::path _lastModule;
    std::filesystem::path _currentModule;
    uint32_t _index = 0;
    Duckvil::PlugNPlay::__module_information* _loadedModules;
    uint32_t _loadedModulesCount;

    _runtimeReflectionData->m_pEvents = static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_heap.Allocate<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>>(_heap));

    Duckvil::PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

    _autoLoader.LoadAll(_memoryInterface, _free_list, &_loadedModules, &_loadedModulesCount);

    for(uint32_t i = 0; i < _loadedModulesCount; ++i)
    {
        const Duckvil::PlugNPlay::__module_information& _loadedModule = _loadedModules[i];
        Duckvil::RuntimeReflection::GetRecordersCountFunction get_recorder_count = nullptr;

        _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", reinterpret_cast<void**>(&get_recorder_count));

        if(get_recorder_count == nullptr)
        {
            printf("No recorder for %s\n", _loadedModule.m_sName.m_sText);

            continue;
        }

        printf("Module %s is present\n", _loadedModule.m_sName.m_sText);

        uint32_t _recordersCount = get_recorder_count();

        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = _memoryInterface,
            ._pAllocator = _free_list,
            ._pFunctions = _reflectionRecorderFTable,
            ._pData = _runtimeReflectionData
        };

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            Duckvil::RuntimeReflection::RecordFunction record = nullptr;

            _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            record(_stuff);
        }
    }

    {
        auto _types = _reflectionFTable->m_fnGetTypes(_runtimeReflectionData, _memoryInterface, _free_list); // Duckvil::RuntimeReflection::get_types(_runtimeReflectionData, _memoryInterface, _free_list);

        for(auto& _typeHandle : _types)
        {
            const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_reflectionFTable, _runtimeReflectionData, _typeHandle, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule);

            if(_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() && (uint8_t)_variant.m_traits & (uint8_t)Duckvil::RuntimeReflection::property_traits::is_bool)
            {
                reflection_module _module = {};

                _module.m_pObject = Duckvil::RuntimeReflection::create<const Duckvil::Memory::FreeList&, Duckvil::RuntimeReflection::__ftable*, Duckvil::RuntimeReflection::__data*>(_memoryInterface, _free_list, _reflectionFTable, _runtimeReflectionData, _typeHandle, false, _heap, _reflectionFTable, _runtimeReflectionData);
                _module.m_typeHandle = _typeHandle;
                _module.m_generateCustomFunctionHandle = Duckvil::RuntimeReflection::get_function_handle<std::ofstream&, std::vector<std::pair<uint32_t, std::vector<std::string>>>&>(_reflectionFTable, _runtimeReflectionData, _typeHandle, "GenerateCustom");
                _module.m_clearFunctionHandle = Duckvil::RuntimeReflection::get_function_handle(_reflectionFTable, _runtimeReflectionData, _typeHandle, "Clear");
                _module.m_processAST_FunctionHandle = Duckvil::RuntimeReflection::get_function_handle<Duckvil::Parser::__ast*>(_reflectionFTable, _runtimeReflectionData, _typeHandle, "ProcessAST");

                if(_aModules.Full())
                {
                    _aModules.Resize(_aModules.Size() * 2);
                }

                _aModules.Allocate(_module);

                // Duckvil::RuntimeReflection::invoke_member<std::ofstream&>(_runtimeReflectionData, _typeHandle, _function, _testSystem);
            }
        }
    }

    const auto& _dbPath = std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / "reflection_db.json";

    if(_argumentsParser[Options::FILE].m_bIsSet)
    {
        std::filesystem::path _file; // _argumentsParser[Options::FILE].m_sResult;
        std::filesystem::path _cwd = _argumentsParser[Options::CWD].m_sResult;

        if(_argumentsParser[Options::IS_RELATIVE].m_bIsSet)
        {
            _file = _argumentsParser[Options::FILE].m_sResult;
        }
        else if(_argumentsParser[Options::IS_ABSOLUTE].m_bIsSet)
        {
            auto _x = _argumentsParser[Options::FILE].m_sResult;
            _file = std::filesystem::relative(_x, _cwd / "include");
        }
        else
        {
            // Print message if file is relative or absolute

            return 0;
        }

        nlohmann::json _dbJ;
        std::ifstream _iJson(_dbPath);

        _iJson >> _dbJ;

        _iJson.close();

        // nlohmann::json _last;
        // nlohmann::json _exists;
        // nlohmann::json::iterator _lastIt = _j["files"].end();

        // try
        // {
        //     const nlohmann::json& _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _file, _lastModule, _currentModule, _index, [&](nlohmann::json& _json)
        //     {
        //         for(nlohmann::json::iterator _it = _j["files"].begin(); _it != _j["files"].end(); ++_it)
        //         {
        //             if((*_it)["name"].get<std::string>() == _json["name"].get<std::string>())
        //             {
        //                 if(_last.empty() || _last["index"].get<uint32_t>() < (*_it)["index"])
        //                 {
        //                     _last = *_it;
        //                     _lastIt = _it;

        //                     if(_last["source"].get<std::string>() == _relativePath.string())
        //                     {
        //                         _json["index"] = _last["index"].get<uint32_t>();

        //                         _exists = *_it;
        //                     }
        //                     else
        //                     {
        //                         _json["index"] = _last["index"].get<uint32_t>() + 1;
        //                     }
        //                 }
        //             }
        //         }

        //         if(_last.empty())
        //         {
        //             _json["index"] = (*(_lastIt - 1))["index"].get<uint32_t>() + 1;
        //         }
        //     }, true);

        //     if(_exists.empty())
        //     {
        //         if(_lastIt != _j["files"].end())
        //         {
        //             _j["files"].insert(_lastIt + 1, _jFile);
        //         }
        //         else
        //         {
        //             _j["files"].insert(_lastIt, _jFile);
        //         }
        //     }
        // }
        // catch(const Duckvil::Parser::blank_file& _e)
        // {
        //     return 0;
        // }

        auto _f = _dbJ["files2"].find(_file.string());

        if(_f == _dbJ["files2"].end())
        {
            // Print some message about file not exists

            return 0;
        }

        const auto& _fileMD5 = md5(load_file_as_string((_cwd / "include" / _file).string()));
        auto _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _cwd / "include" / _file, _f->at("name").get<std::string>(), _f->at("index").get<uint32_t>(), _f->at("hash").get<std::string>() != _fileMD5 || _argumentsParser[Options::FORCE].m_bIsSet);

        _jFile["hash"] = _fileMD5;

        _f->update(_jFile);

        std::ofstream _oJson(_dbPath);

        _oJson << std::setw(4) << _dbJ << std::endl;

        _oJson.close();
    }
    else
    {
        nlohmann::json _dbJ;

        if(std::filesystem::exists(_dbPath))
        {
            std::ifstream _iJson(_dbPath);

            _iJson >> _dbJ;

            _iJson.close();
        }
        else
        {
            std::ofstream _oJson(_dbPath);

            _oJson << _dbJ;

            _oJson.close();
        }

        nlohmann::json _dbNewJ;
        bool _moduleChanged = false;

        // Iterate over the json db to check if any file has been deleted

        std::vector<std::string> _modulesChanged;

        for(auto _it = _dbJ["files2"].begin(); _it != _dbJ["files2"].end(); ++_it)
        {
            auto _p = std::filesystem::path(_it.key());

            while(_p.has_parent_path())
            {
                _p = _p.parent_path();
            }

        // Remove old files if exists
            if(!std::filesystem::exists(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "include" / _it.key()))
            {
                _modulesChanged.push_back(_p.string());

                const auto& _gfcpp = std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / _it->at("generated_cpp").get<std::string>();

                if(std::filesystem::exists(_gfcpp))
                {
                    std::filesystem::remove(_gfcpp);
                }

                const auto& _gfh = std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / _it->at("generated_h").get<std::string>();

                if(std::filesystem::exists(_gfh))
                {
                    std::filesystem::remove(_gfh);
                }
            }
        }

        for(auto& _path : std::filesystem::recursive_directory_iterator(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "include"))
        {
            if(!_path.path().has_extension() || _path.path().extension() != ".h")
            {
                continue;
            }

            const auto& _a = std::filesystem::relative(_path.path(), std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "include");
            const auto& _fileMD5 = md5(load_file_as_string(_path.path().string()));
            auto _f = _dbJ["files2"].find(_a.string());

            if(_a.has_parent_path())
            {
                _currentModule = _a;

                while(_currentModule.has_parent_path())
                {
                    _currentModule = _currentModule.parent_path();
                }
            }

            if(_lastModule == "")
            {
                _lastModule = _currentModule;
            }

            if(_lastModule != _currentModule)
            {
                if(_moduleChanged)
                {
                    std::ofstream _file(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / _lastModule / "plugin_info.cpp");

                    generate_plugin_info(_file, _index, _lastModule);

                    _file.close();
                }

                _moduleChanged = false;

                _lastModule = _currentModule;
                _index = 0;
            }

            for(auto _it = _modulesChanged.begin(); _it != _modulesChanged.end(); ++_it)
            {
                if(_currentModule.string() == *_it)
                {
                    _moduleChanged = true;

                    _modulesChanged.erase(_it);

                    break;
                }
            }

            if(_f == _dbJ["files2"].end())
            {
                // Not found
                // Need to be generated and added

                auto _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _path, _currentModule, _index, true);

                _jFile["hash"] = _fileMD5;

                _dbNewJ.push_back(nlohmann::json::object_t::value_type(_a.string(), _jFile));

                _moduleChanged = true;
            }
            else if(_argumentsParser[Options::FORCE].m_bIsSet || _f->at("hash").get<std::string>() != _fileMD5 || _moduleChanged)
            {
                // Found but need to be generated and updated or forced

                auto _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _path, _currentModule, _index, true);

                _jFile["hash"] = _fileMD5;

                _f->update(_jFile);

                _dbNewJ.push_back(nlohmann::json::object_t::value_type(_f.key(), _f.value()));
            }
            else
            {
                _dbNewJ.push_back(nlohmann::json::object_t::value_type(_f.key(), _f.value()));
            }

            _index++;
        }

        _dbJ["files2"] = _dbNewJ;

        if(!_lastModule.has_extension())
        {
            _lastModule = _lastModule;
        }
        else
        {
            _lastModule = ".";
        }

        std::ofstream _file(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / _lastModule / "plugin_info.cpp");

        generate_plugin_info(_file, _index, _lastModule);

        _file.close();

        std::ofstream _oJson(_dbPath);

        _oJson << std::setw(4) << _dbJ << std::endl;

        _oJson.close();
    }

    return 0;
}