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

struct user_data
{
    std::filesystem::path m_path;
};

void generate(std::ofstream& _hFile, std::ofstream& _sFile, void* _pUserData, const Duckvil::RuntimeReflection::__ftable* _ftableReflection, Duckvil::RuntimeReflection::__data* _dataReflection)
{
    user_data* _userData = static_cast<user_data*>(_pUserData);

    std::string _fileID = _userData->m_path.string();

    _fileID = Duckvil::Utils::replace_all(_fileID, "\\", "_");
    _fileID = Duckvil::Utils::replace_all(_fileID, ".", "_");

    _hFile << "#include \"Serializer/Runtime/ISerializer.h\"\n\n";
    _hFile << "#include \"RuntimeReflection/Markers.h\"\n\n";

    std::vector<std::pair<uint32_t, std::vector<std::string>>> _generated;

    for(auto& _module : _aModules)
    {
        if(_module.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member<std::ofstream&, std::ofstream&, Duckvil::RuntimeReflection::GeneratedVector&>(_ftableReflection, _dataReflection, _module.m_typeHandle, _module.m_generateCustomFunctionHandle, _module.m_pObject, _hFile, _sFile, _generated);
    }

    for(const auto& _generated2 : _generated)
    {
        _hFile << "#define " << _fileID << "_" << _generated2.first << "_GENERATED_BODY";

        if(_generated2.second.size())
        {
            _hFile << " \\\n";
        }

        for(uint32_t i = 0; i < _generated2.second.size(); ++i)
        {
            _hFile << _fileID << "_" << _generated2.first << "_REFLECTION_MODULE_" << _generated2.second[i];

            if(i < _generated2.second.size() - 1)
            {
                _hFile << " \\\n";
            }
            else
            {
                _hFile << "\n\n";
            }
        }
    }

    _hFile << "#undef DUCKVIL_CURRENT_FILE_ID\n";
    _hFile << "#define DUCKVIL_CURRENT_FILE_ID " << _fileID << "\n";
}

void generate_plugin_info(std::ofstream& _file, const uint32_t& _uiIndex, const std::filesystem::path& _path)
{
    _file << "#include \"RuntimeReflection/Recorder.h\"\n";
    _file << "#include \"Logger/Logger.h\"\n";
    _file << "DUCKVIL_RUNTIME_REFLECTION_RECORDERS_COUNT(" << _uiIndex << ")\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)\n";
    _file << "{\n";
    _file << "Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);\n";
    _file << "}\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)\n";
    _file << "{\n";
    _file << "Duckvil::logger_make_current(_loggerContext);\n";
    _file << "}\n\n";
    _file << "DUCKVIL_EXPORT void duckvil_plugin_make_current_heap_context(const Duckvil::Memory::free_list_context& _heapContext)\n";
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

nlohmann::json process_file(const Duckvil::Parser::__ast_ftable* _pAST_FTable, const Duckvil::Parser::__lexer_ftable* _pLexerFTable, Duckvil::Parser::__lexer_data* _pLexerData, const Duckvil::RuntimeReflection::__generator_ftable* _pGeneratorFTable, const Duckvil::RuntimeReflection::__ftable* _ftableReflection, Duckvil::RuntimeReflection::__data* _dataReflection, const std::filesystem::path& _cwd, const std::filesystem::path& _path, const std::filesystem::path& _currentModule, uint32_t _index, bool _bGenerate = false)
{
    if(_path.extension() != ".h")
    {
        return nlohmann::json();
    }

    nlohmann::json _jFile;
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
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_CS_FUNCTION", &Duckvil::Utils::user_define_behavior });

#ifdef DUCKVIL_HOT_RELOADING
    _astData.m_aUserDefines.push_back(Duckvil::Parser::user_define{ "DUCKVIL_HOT_RELOADING", &Duckvil::Utils::user_define_behavior });
#endif

    _relativePath = std::filesystem::relative(_path, _cwd / "include");

    _astData.m_sFile = _relativePath;
    _astData.m_sPath = _cwd / "include";

    _pAST_FTable->ast_generate(&_astData, _pLexerFTable, *_pLexerData);
    // _ast->ast_print(_astData);

    for(auto& _reflectionModule : _aModules)
    {
        if(_reflectionModule.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member<Duckvil::Parser::__ast*>(_ftableReflection, _dataReflection, _reflectionModule.m_typeHandle, _reflectionModule.m_processAST_FunctionHandle, _reflectionModule.m_pObject, &_astData);
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
        snprintf(_generatorData.m_sInclude, DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX, "%s", _relativePath.string().c_str());
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
        _pGeneratorFTable->generate(&_generatorData, _source.string().c_str(), _header.string().c_str(), _astData, &generate, &_userData, _ftableReflection, _dataReflection);
    }

    for(auto& _module : _aModules)
    {
        if(_module.m_pObject == nullptr)
        {
            continue;
        }

        Duckvil::RuntimeReflection::invoke_member(_ftableReflection, _dataReflection, _module.m_typeHandle, _module.m_clearFunctionHandle, _module.m_pObject);
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
    FORCE,          // Force regenerate all files in CWD

                    // Maybe some option for specific module? eg. Duckvil/Editor
                    // Then only files in this folder will be processed

                    // Some option to specify purging/cleanup?

                    // TODO: Fix one file processing
    SINGLE_MODULE
};

Duckvil::Utils::CommandArgumentsParser::Descriptor g_pDescriptors[] =
{
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::CWD, "CWD"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::FILE, "file"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IS_RELATIVE, "is_relative"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IS_ABSOLUTE, "is_absolute"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::FORCE, "force"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::SINGLE_MODULE, "single")
};

bool process_single_file(const Duckvil::Utils::CommandArgumentsParser& _cmdParser, const std::filesystem::path& _CWD, const Duckvil::Parser::__ast_ftable& _ftableAST, const Duckvil::Parser::__lexer_ftable& _ftableLexer, Duckvil::Parser::__lexer_data& _dataLexer, const Duckvil::RuntimeReflection::__generator_ftable& _ftableGenerator, const Duckvil::RuntimeReflection::__ftable& _ftableRuntimeReflection, Duckvil::RuntimeReflection::__data* _dataReflection)
{
    std::filesystem::path _file; // _argumentsParser[Options::FILE].m_sResult;
    std::filesystem::path _cwd = _CWD;
    std::filesystem::path _currentModule;

    if (_cmdParser[Options::IS_RELATIVE].m_bIsSet)
    {
        _file = _cmdParser[Options::FILE].m_sResult;
    }
    else if (_cmdParser[Options::IS_ABSOLUTE].m_bIsSet)
    {
        auto _x = _cmdParser[Options::FILE].m_sResult;
        _file = std::filesystem::relative(_x, _cwd / "include");
    }
    else
    {
        // Print message if file is relative or absolute

        return 0;
    }

    nlohmann::json _dbJ;
    const std::filesystem::path& _dbPath = _CWD / "__generated_reflection__" / "reflection_db.json";
    std::ifstream _iJson(_dbPath);

    _iJson >> _dbJ;

    _iJson.close();

    auto _f = _dbJ["files2"].find(_file.string());
    const auto& _fileMD5 = md5(load_file_as_string((_cwd / "include" / _file).string()));

    if (_f == _dbJ["files2"].end())
    {
        auto _lastIndex = _dbJ["files2"].back()["index"].get<uint32_t>();
        auto _jFile = process_file(&_ftableAST, &_ftableLexer, &_dataLexer, &_ftableGenerator, &_ftableRuntimeReflection, _dataReflection, _CWD, _cwd / "include" / _file, _currentModule, _lastIndex + 1, true);

        _jFile["hash"] = _fileMD5;

        _dbJ["files2"].push_back(nlohmann::json::object_t::value_type(_file.string(), _jFile));
    }
    else
    {
        auto _lastIndex = _f->at("index").get<uint32_t>();
        auto _jFile = process_file(&_ftableAST, &_ftableLexer, &_dataLexer, &_ftableGenerator, &_ftableRuntimeReflection, _dataReflection, _CWD, _cwd / "include" / _file, _f->at("name").get<std::string>(), _lastIndex, _f->at("hash").get<std::string>() != _fileMD5 || _cmdParser[Options::FORCE].m_bIsSet);

        _jFile["hash"] = _fileMD5;

        _f->update(_jFile);
    }

    std::ofstream _oJson(_dbPath);

    _oJson << std::setw(4) << _dbJ << std::endl;

    _oJson.close();

    return true;
}

bool process_multiple_files(const Duckvil::Utils::CommandArgumentsParser& _cmdParser, const std::filesystem::path& _CWD, const Duckvil::Parser::__ast_ftable& _ftableAST, const Duckvil::Parser::__lexer_ftable& _ftableLexer, Duckvil::Parser::__lexer_data& _dataLexer, const Duckvil::RuntimeReflection::__generator_ftable& _ftableGenerator, const Duckvil::RuntimeReflection::__ftable& _ftableRuntimeReflection, Duckvil::RuntimeReflection::__data* _dataReflection)
{
    uint32_t _index = 0;

    nlohmann::json _dbJ;

    const std::filesystem::path& _dbPath = _CWD / "__generated_reflection__" / "reflection_db.json";

    if (std::filesystem::exists(_dbPath))
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

    const auto& _isSingleModuleFound = _dbJ.find("isSingleModule");
    bool _isSingleModule = _cmdParser[Options::SINGLE_MODULE].m_bIsSet;

    if (_isSingleModuleFound != _dbJ.end())
    {
        _isSingleModule = _isSingleModuleFound.value().get<bool>();
    }
    else
    {
        _dbJ["isSingleModule"] = _isSingleModule;
    }

    for (auto _it = _dbJ["files2"].begin(); _it != _dbJ["files2"].end(); ++_it)
    {
        auto _p = std::filesystem::path(_it.key());

        while (_p.has_parent_path())
        {
            _p = _p.parent_path();
        }

        // Remove old files if exists
        if (!std::filesystem::exists(std::filesystem::path(_CWD) / "include" / _it.key()))
        {
            _modulesChanged.push_back(_p.string());

            const auto& _gfcpp = std::filesystem::path(_CWD) / "__generated_reflection__" / _it->at("generated_cpp").get<std::string>();

            if (std::filesystem::exists(_gfcpp))
            {
                std::filesystem::remove(_gfcpp);
            }

            const auto& _gfh = std::filesystem::path(_CWD) / "__generated_reflection__" / _it->at("generated_h").get<std::string>();

            if (std::filesystem::exists(_gfh))
            {
                std::filesystem::remove(_gfh);
            }
        }
    }

    std::filesystem::path _currentModule;
    std::filesystem::path _lastModule;

    for (auto& _path : std::filesystem::recursive_directory_iterator(std::filesystem::path(_CWD) / "include"))
    {
        if (!_path.path().has_extension() || _path.path().extension() != ".h")
        {
            continue;
        }

        const auto& _a = std::filesystem::relative(_path.path(), std::filesystem::path(_CWD) / "include");
        const auto& _fileMD5 = md5(load_file_as_string(_path.path().string()));
        auto _f = _dbJ["files2"].find(_a.string());

        if (_a.has_parent_path())
        {
            _currentModule = _a;

            while (_currentModule.has_parent_path())
            {
                _currentModule = _currentModule.parent_path();
            }
        }

        if (_lastModule == "")
        {
            _lastModule = _currentModule;
        }

        if (_lastModule != _currentModule && !_isSingleModule)
        {
            if (_moduleChanged)
            {
                std::ofstream _file(std::filesystem::path(_CWD) / "__generated_reflection__" / _lastModule / "plugin_info.cpp");

                generate_plugin_info(_file, _index, _lastModule);

                _file.close();
            }

            _moduleChanged = false;

            _lastModule = _currentModule;
            _index = 0;
        }

        for (auto _it = _modulesChanged.begin(); _it != _modulesChanged.end(); ++_it)
        {
            if (_currentModule.string() == *_it)
            {
                _moduleChanged = true;

                _modulesChanged.erase(_it);

                break;
            }
        }

        if (_f == _dbJ["files2"].end())
        {
            // Not found
            // Need to be generated and added

            auto _jFile = process_file(&_ftableAST, &_ftableLexer, &_dataLexer, &_ftableGenerator, &_ftableRuntimeReflection, _dataReflection, _CWD, _path, _currentModule, _index, true);

            _jFile["hash"] = _fileMD5;

            _dbNewJ.push_back(nlohmann::json::object_t::value_type(_a.string(), _jFile));

            _moduleChanged = true;
        }
        else if (_cmdParser[Options::FORCE].m_bIsSet || _f->at("hash").get<std::string>() != _fileMD5 || _moduleChanged)
        {
            // Found but need to be generated and updated or forced

            auto _jFile = process_file(&_ftableAST, &_ftableLexer, &_dataLexer, &_ftableGenerator, &_ftableRuntimeReflection, _dataReflection, _CWD, _path, _currentModule, _index, true);

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

    if (_lastModule.has_extension() || _isSingleModule)
    {
        _lastModule = ".";
    }

    std::ofstream _file(std::filesystem::path(_CWD) / "__generated_reflection__" / _lastModule / "plugin_info.cpp");

    generate_plugin_info(_file, _index, _lastModule);

    _file.close();

    std::ofstream _oJson(_dbPath);

    _oJson << std::setw(4) << _dbJ << std::endl;

    _oJson.close();

    return true;
}

bool initialize_modules(const Duckvil::Memory::FreeList& _heap, const Duckvil::Memory::ftable& _ftableMemory, Duckvil::Memory::free_list_allocator* _pAllocator, const Duckvil::RuntimeReflection::__recorder_ftable& _ftableRecorder, const Duckvil::RuntimeReflection::__ftable& _ftableRuntimeReflection, Duckvil::RuntimeReflection::__data* _dataReflection, const Duckvil::PlugNPlay::__module& _module)
{
    Duckvil::PlugNPlay::__module_information* _loadedModules;
    uint32_t _loadedModulesCount;

    _dataReflection->m_pEvents = static_cast<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>*>(_heap.Allocate<Duckvil::Event::Pool<Duckvil::Event::mode::immediate>>(_heap));

    Duckvil::PlugNPlay::AutoLoader _autoLoader(DUCKVIL_OUTPUT);

    _autoLoader.LoadAll(&_ftableMemory, _pAllocator, &_loadedModules, &_loadedModulesCount);

    for (uint32_t i = 0; i < _loadedModulesCount; ++i)
    {
        const Duckvil::PlugNPlay::__module_information& _loadedModule = _loadedModules[i];
        Duckvil::RuntimeReflection::GetRecordersCountFunction get_recorder_count = nullptr;

        _module.get(_loadedModule, "duckvil_get_runtime_reflection_recorders_count", reinterpret_cast<void**>(&get_recorder_count));

        if (get_recorder_count == nullptr)
        {
            printf("No recorder for %s\n", _loadedModule.m_sName.m_sText);

            continue;
        }

        printf("Module %s is present\n", _loadedModule.m_sName.m_sText);

        uint32_t _recordersCount = get_recorder_count();

        duckvil_runtime_reflection_recorder_stuff _stuff =
        {
            ._pMemoryInterface = &_ftableMemory,
            ._pAllocator = _pAllocator,
            ._pFunctions = &_ftableRecorder,
            ._pData = _dataReflection
        };

        for (uint32_t j = 0; j < _recordersCount; ++j)
        {
            Duckvil::RuntimeReflection::RecordFunction record = nullptr;

            _module.get(_loadedModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if (record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            record(_stuff);
        }
    }

    {
        auto _types = _ftableRuntimeReflection.m_fnGetTypes(_dataReflection, &_ftableMemory, _pAllocator); // Duckvil::RuntimeReflection::get_types(_runtimeReflectionData, _memoryInterface, _free_list);

        for (auto& _typeHandle : _types)
        {
            const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(&_ftableRuntimeReflection, _dataReflection, _typeHandle, Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule);

            if (_variant.m_ullTypeID != std::numeric_limits<std::size_t>::max() && (uint8_t)_variant.m_traits & (uint8_t)Duckvil::RuntimeReflection::property_traits::is_bool)
            {
                reflection_module _module = {};

                _module.m_pObject = Duckvil::RuntimeReflection::create<const Duckvil::Memory::FreeList&, const Duckvil::RuntimeReflection::__ftable*, Duckvil::RuntimeReflection::__data*>(&_ftableMemory, _pAllocator, &_ftableRuntimeReflection, _dataReflection, _typeHandle, false, _heap, &_ftableRuntimeReflection, _dataReflection);
                _module.m_typeHandle = _typeHandle;
                _module.m_generateCustomFunctionHandle = Duckvil::RuntimeReflection::get_function_handle<std::ofstream&, std::ofstream&, Duckvil::RuntimeReflection::GeneratedVector&>(&_ftableRuntimeReflection, _dataReflection, _typeHandle, "GenerateCustom");
                _module.m_clearFunctionHandle = Duckvil::RuntimeReflection::get_function_handle(&_ftableRuntimeReflection, _dataReflection, _typeHandle, "Clear");
                _module.m_processAST_FunctionHandle = Duckvil::RuntimeReflection::get_function_handle<Duckvil::Parser::__ast*>(&_ftableRuntimeReflection, _dataReflection, _typeHandle, "ProcessAST");

                if (_aModules.Full())
                {
                    _aModules.Resize(_aModules.Size() * 2);
                }

                _aModules.Allocate(_module);

                // Duckvil::RuntimeReflection::invoke_member<std::ofstream&>(_runtimeReflectionData, _typeHandle, _function, _testSystem);
            }
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _cmdParser(argc, argv);

    if(!_cmdParser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    bool _isSingleModule = _cmdParser[Options::SINGLE_MODULE].m_bIsSet;
    auto _CWD = _cmdParser[Options::CWD].m_sResult;

    printf("CWD: %s\n", _CWD);

    if(_cmdParser[Options::IS_ABSOLUTE].m_bIsSet)
    {
        printf("Is absolute\n");
    }
    else if(_cmdParser[Options::IS_RELATIVE].m_bIsSet)
    {
        printf("Is relative\n");
    }

    if(_cmdParser[Options::FILE].m_bIsSet)
    {
        printf("File: %s\n", _cmdParser[Options::FILE].m_sResult);
    }

    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _reflectionModule("RuntimeReflection");
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _parser("Parser");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_reflectionModule);
    _module.load(&_memoryModule);
    _module.load(&_parser);

    duckvil_memory_init_callback _fnMemoryInit;
    duckvil_lexer_init_callback _lexer_init;
    duckvil_ast_init_callback _ast_init;
    duckvil_runtime_reflection_generator_init_callback _runtime_reflection_generator_init;
    duckvil_runtime_reflection_init_callback _runtimeReflectionInit;
    duckvil_runtime_reflection_recorder_init_callback _runtimeReflectionRecorderInit;

    _module.get(_memoryModule, "duckvil_memory_init", reinterpret_cast<void**>(&_fnMemoryInit));
    _module.get(_parser, "duckvil_lexer_init", reinterpret_cast<void**>(&_lexer_init));
    _module.get(_parser, "duckvil_ast_init", reinterpret_cast<void**>(&_ast_init));
    _module.get(_reflectionModule, "duckvil_runtime_reflection_generator_init", reinterpret_cast<void**>(&_runtime_reflection_generator_init));
    _module.get(_reflectionModule, "duckvil_runtime_reflection_init", reinterpret_cast<void**>(&_runtimeReflectionInit));
    _module.get(_reflectionModule, "duckvil_runtime_reflection_recorder_init", reinterpret_cast<void**>(&_runtimeReflectionRecorderInit));

    const Duckvil::Memory::ftable* _memoryInterface = _fnMemoryInit();
    Duckvil::Memory::linear_allocator* _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);

    Duckvil::Memory::free_list_allocator* _free_list = _memoryInterface->m_fnLinearAllocateFreeListAllocator(_mainMemoryAllocator, 1000 * 1024);
    Duckvil::Memory::FreeList _heap(_memoryInterface, _free_list);

    _heap.Allocate(_aModules, 1);

    const Duckvil::Parser::__ast_ftable* _ast = _ast_init();
    const Duckvil::Parser::__lexer_ftable* _lexerFtable = _lexer_init();
    const Duckvil::RuntimeReflection::__ftable* _reflectionFTable = _runtimeReflectionInit();
    const Duckvil::RuntimeReflection::__recorder_ftable* _reflectionRecorderFTable = _runtimeReflectionRecorderInit();
    const Duckvil::RuntimeReflection::__generator_ftable* _generatorFtable = _runtime_reflection_generator_init();

    Duckvil::RuntimeReflection::__data* _runtimeReflectionData = _reflectionFTable->m_fnInit(_memoryInterface, _free_list, _reflectionFTable);

    initialize_modules(_heap, *_memoryInterface, _free_list, *_reflectionRecorderFTable, *_reflectionFTable, _runtimeReflectionData, _module);

    Duckvil::Parser::__lexer_data _lexerData;

    _lexerFtable->init(&_lexerData);

    _lexerData.m_pConfig->AddInclude((std::filesystem::path(_CWD) / "__generated_reflection__").string());

    if(_cmdParser[Options::FILE].m_bIsSet)
    {
        process_single_file(_cmdParser, _CWD, *_ast, *_lexerFtable, _lexerData, *_generatorFtable, *_reflectionFTable, _runtimeReflectionData);
    }
    else
    {
        process_multiple_files(_cmdParser, _CWD, *_ast, *_lexerFtable, _lexerData, *_generatorFtable, *_reflectionFTable, _runtimeReflectionData);
    }

    return 0;
}