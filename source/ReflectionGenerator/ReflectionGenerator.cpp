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

nlohmann::json process_file(Duckvil::Parser::__ast_ftable* _pAST_FTable, Duckvil::Parser::__lexer_ftable* _pLexerFTable, Duckvil::RuntimeReflection::__generator_ftable* _pGeneratorFTable, const std::filesystem::path& _cwd, const std::filesystem::path& _path, std::filesystem::path& _lastPath, uint32_t& _index, const std::function<void(nlohmann::json&)> _fnProcessJson, bool _bGenerate = false, bool _bOneFile = false)
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

    Duckvil::RuntimeReflection::__generator_data _generatorData;

    std::filesystem::path _pluginDirectory = _relativePath;

// At least it is working...
    while(_pluginDirectory.has_parent_path())
    {
        _pluginDirectory = _pluginDirectory.parent_path();
    }

    if(!std::filesystem::is_directory(_cwd / "include" / _pluginDirectory))
    {
        _pluginDirectory = "";
    }

    if(_lastPath == "")
    {
        _lastPath = _pluginDirectory;
    }

    if(!_lastPath.has_extension())
    {
        _lastPath = _lastPath;
    }
    else
    {
        _lastPath = ".";
    }

    _jFile["name"] = _pluginDirectory;

    std::filesystem::path _jSource = _relativePath;
    std::filesystem::path _jHeader = _relativePath;

    _jSource.replace_extension(".generated.cpp");
    _jHeader.replace_extension(".generated.h");

    _jFile["source"] = _relativePath;
    _jFile["generated_h"] = _jHeader;
    _jFile["generated_cpp"] = _jSource;

    if(_fnProcessJson)
    {
        _fnProcessJson(_jFile);

        _index = _jFile["index"].get<uint32_t>();
    }

    if(_bOneFile || _lastPath != _pluginDirectory)
    {
        if(_bGenerate)
        {
            std::ofstream _file(_cwd / "__generated_reflection__" / _lastPath / "plugin_info.cpp");

            generate_plugin_info(_file, _index, _lastPath);

            _file.close();
        }

        if(!_bOneFile)
        {
            _index = 0;
        }
        _lastPath = _pluginDirectory;
    }

    _jFile["index"] = _index;

    if(_relativePath.string().size() < DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX)
    {
        strcpy(_generatorData.m_sInclude, _relativePath.string().c_str());
    }
    else
    {
        assert(false && "Path is too long!");
    }

    _generatorData.m_uiRecorderIndex = _index++;

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

    // _j["files"].push_back(_jFile);

    return _jFile;
}

enum class Options
{
    CWD,
    FILE,
    IS_RELATIVE,
    FORCE
};

Duckvil::Utils::CommandArgumentsParser::Descriptor g_pDescriptors[] =
{
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::CWD, "CWD"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::FILE, "file"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IS_RELATIVE, "is_relative"),
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
    std::filesystem::path _lastPath;
    uint32_t _index = 0;
    Duckvil::PlugNPlay::__module_information* _loadedModules;
    uint32_t _loadedModulesCount;

    _runtimeReflectionData->m_pEvents = static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_heap.Allocate<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>>(_heap));

    Duckvil::PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

    _autoLoader.LoadAll(_memoryInterface, _free_list, &_loadedModules, &_loadedModulesCount);

    for(uint32_t i = 0; i < _loadedModulesCount; ++i)
    {
        const Duckvil::PlugNPlay::__module_information& _loadedModule = _loadedModules[i];
        uint32_t (*get_recorder_count)();

        _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorder_count", reinterpret_cast<void**>(&get_recorder_count));

        if(get_recorder_count == nullptr)
        {
            printf("No recorder for %s\n", _loadedModule.m_sName.m_sText);

            continue;
        }

        printf("Module %s is present\n", _loadedModule.m_sName.m_sText);

        uint32_t _recordersCount = get_recorder_count();

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            duckvil_recorderd_types (*record)(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData);

            _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            record(_memoryInterface, _free_list, _reflectionRecorderFTable, _reflectionFTable, _runtimeReflectionData);
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

    if(_argumentsParser[Options::FILE].m_bIsSet)
    {
        std::filesystem::path _file = _argumentsParser[Options::FILE].m_sResult;

        if(_argumentsParser[Options::IS_RELATIVE].m_bIsSet)
        {
            _file = std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / _file;
        }

        nlohmann::json _j;
        std::ifstream _iJson(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / "reflection_db.json");

        _iJson >> _j;

        _iJson.close();

        nlohmann::json _last;
        nlohmann::json _exists;
        nlohmann::json::iterator _lastIt = _j["files"].end();

        try
        {
            const nlohmann::json& _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _file, _lastPath, _index, [&](nlohmann::json& _json)
            {
                for(nlohmann::json::iterator _it = _j["files"].begin(); _it != _j["files"].end(); ++_it)
                {
                    if((*_it)["name"].get<std::string>() == _json["name"].get<std::string>())
                    {
                        if(_last.empty() || _last["index"].get<uint32_t>() < (*_it)["index"])
                        {
                            _last = *_it;
                            _lastIt = _it;

                            if(_last["source"].get<std::string>() == _relativePath.string())
                            {
                                _json["index"] = _last["index"].get<uint32_t>();

                                _exists = *_it;
                            }
                            else
                            {
                                _json["index"] = _last["index"].get<uint32_t>() + 1;
                            }
                        }
                    }
                }

                if(_last.empty())
                {
                    _json["index"] = (*(_lastIt - 1))["index"].get<uint32_t>() + 1;
                }
            }, true, true);

            if(_exists.empty())
            {
                if(_lastIt != _j["files"].end())
                {
                    _j["files"].insert(_lastIt + 1, _jFile);
                }
                else
                {
                    _j["files"].insert(_lastIt, _jFile);
                }
            }
        }
        catch(const Duckvil::Parser::blank_file& _e)
        {
            return 0;
        }

        std::ofstream _oJson(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / "reflection_db.json");

        _oJson << std::setw(4) << _j << std::endl;

        _oJson.close();
    }
    else
    {
        nlohmann::json _j;
        const auto& _p = std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / "reflection_db.json";

        if(std::filesystem::exists(_p))
        {
            std::ifstream _iJson(_p);

            _iJson >> _j;

            _iJson.close();
        }
        else
        {
            std::ofstream _oJson(_p);

            _oJson << _j;

            _oJson.close();
        }

        for(auto& _path : std::filesystem::recursive_directory_iterator(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "include"))
        {
            if(!_path.path().has_extension())
            {
                continue;
            }

            bool _equal = false;
            uint32_t _foundIndex = -1;
            uint32_t _i = 0;

            for(const auto& _e : _j["files"])
            {
                if(!_e.empty())
                {
                    const auto& _a = std::filesystem::relative(_path.path(), std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "include");

                    if(_a == _e["source"].get<std::string>())
                    {
                        if(_e.contains("hash") && _e["hash"] == md5(load_file_as_string(_path.path().string())))
                        {
                            _equal = true;
                        }

                        _foundIndex = _i;

                        break;
                    }
                }

                ++_i;
            }

            try
            {
                auto _jFile = process_file(_ast, _lexerFtable, _generatorFtable, _argumentsParser[Options::CWD].m_sResult, _path, _lastPath, _index, nullptr, _argumentsParser[Options::FORCE].m_bIsSet ? true : !_equal);

                _jFile["hash"] = md5(load_file_as_string(_path.path().string()));

                if(_foundIndex == -1)
                {
                    _j["files"].push_back(_jFile);
                }
                else
                {
                    _j["files"][_i] = _jFile;
                }
            }
            catch(const Duckvil::Parser::blank_file& _e)
            {

            }
        }

        if(!_lastPath.has_extension())
        {
            _lastPath = _lastPath;
        }
        else
        {
            _lastPath = ".";
        }

        std::ofstream _file(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / _lastPath / "plugin_info.cpp");

        generate_plugin_info(_file, _index, _lastPath);

        _file.close();

        std::ofstream _oJson(std::filesystem::path(_argumentsParser[Options::CWD].m_sResult) / "__generated_reflection__" / "reflection_db.json");

        _oJson << std::setw(4) << _j << std::endl;

        _oJson.close();
    }

    return 0;
}