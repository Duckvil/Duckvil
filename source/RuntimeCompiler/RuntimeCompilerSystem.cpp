#include "RuntimeCompiler/RuntimeCompilerSystem.h"

#include <Windows.h>

#include "Serializer/Runtime/Serializer.h"

#undef max

namespace Duckvil { namespace RuntimeCompiler {

    RuntimeCompilerSystem::RuntimeCompilerSystem(
        int a,
        const Memory::FreeList& _heap,
        RuntimeReflection::__data* _pReflectionData,
        RuntimeReflection::__recorder_ftable* _pReflectionRecorderFTable,
        RuntimeReflection::__ftable* _pReflectionFTable
    ) :
        m_heap(_heap),
        m_pReflectionData(_pReflectionData),
        m_pReflectionFTable(_pReflectionFTable),
        m_pReflectionRecorderFTable(_pReflectionRecorderFTable)
    {
        _heap.Allocate(m_aHotObjects, 1);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _processModuleInfo("Process");

        _module.load(&_processModuleInfo);

        void (*_duckvilProcessInit)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::Process::ftable* _pFTable);

        _module.get(_processModuleInfo, "duckvil_process_init", (void**)&_duckvilProcessInit);

        _duckvilProcessInit(_heap.GetMemoryInterface(), _heap.GetAllocator(), &m_processFTable);

        m_processFTable.m_fnInit(_heap.GetMemoryInterface(), _heap.GetAllocator(), &m_processData);
        m_processFTable.m_fnSetup(&m_processData);

        m_userData.m_pRuntimeCompiler = this;

        m_pFileWatcher = _heap.Allocate<FileWatcher, FileWatcher::ActionCallback, void*>(&Action, (void*)&m_userData);
    }

    RuntimeCompilerSystem::~RuntimeCompilerSystem()
    {

    }

    bool RuntimeCompilerSystem::Init()
    {
        m_aFlags.push_back("/Zi");
        m_aFlags.push_back("/MDd");
        m_aFlags.push_back("/LD");
        m_aFlags.push_back("/FC");
        m_aFlags.push_back("/Fe" + std::string(DUCKVIL_OUTPUT) + "/Swap/test.dll");
        m_aFlags.push_back("/Fd" + std::string(DUCKVIL_OUTPUT) + "/Swap/test.pdb");

        m_aIncludes.push_back((std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include").string());
        m_aIncludes.push_back((std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__").string());

        m_aDefines.push_back("DUCKVIL_PLATFORM_WINDOWS");
        m_aDefines.push_back("DUCKVIL_RUNTIME_COMPILE");
        m_aDefines.push_back(std::string("DUCKVIL_OUTPUT=\"") + DUCKVIL_OUTPUT + "\"");

        m_aLibrariesPaths.push_back(DUCKVIL_OUTPUT);

        m_aLibraries.push_back("Utils.lib");
        m_aLibraries.push_back("UniTestFramework.lib");
        m_aLibraries.push_back("PlugNPlay.lib");

        // m_aFlags.push_back("/c");
        // m_aFlags.push_back("/std:c++latest");
        // m_aFlags.push_back("/Fo" + std::string(DUCKVIL_OUTPUT) + "/Swap/test.obj");

        // m_aIncludes.push_back((std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include").string());

        // m_aDefines.push_back("DUCKVIL_PLATFORM_WINDOWS");

        // m_aLibrariesPaths.push_back(DUCKVIL_OUTPUT);

        // m_aLibraries.push_back("Utils.lib");
        // m_aLibraries.push_back("UniTestFramework.lib");
        // m_aLibraries.push_back("PlugNPlay.lib");

        m_pFileWatcher->Watch("F:/Projects/C++/Duckvil/source");

        return true;
    }

    void RuntimeCompilerSystem::Update()
    {
        m_pFileWatcher->Update();
    }

    void RuntimeCompilerSystem::Compile(const std::string& _sFile)
    {
        std::string _command = "cl";

        for(const auto& _flag : m_aFlags)
        {
            _command.append(" " + _flag);
        }

        for(const auto& _define : m_aDefines)
        {
            _command.append(" /D" + _define);
        }

        for(const auto& _include : m_aIncludes)
        {
            _command.append(" /I\"" + _include + "\"");
        }

        // add generated reflection file source and other compiled .objs

        _command.append(" " + _sFile);

        {
            std::filesystem::path _file = _sFile;
            std::string _filename = _file.filename().string();
            std::string _moduleName = _file.parent_path().stem().string();
            std::size_t _dotPosition = _filename.find_first_of('.');

            if(_dotPosition != std::string::npos)
            {
                _filename.erase(_filename.begin() + _dotPosition, _filename.end());
            }

            std::string _generatedFilename = _filename + ".Generated.cpp";
            std::filesystem::path _generatedFile = std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__" / _moduleName / _generatedFilename;

            _command.append(" " + _generatedFile.string());
        }

        _command.append(" /link");

        for(const auto& _libPath : m_aLibrariesPaths)
        {
            _command.append(" /LIBPATH:\"" + _libPath + "\"");
        }

        for(const auto& _lib : m_aLibraries)
        {
            _command.append(" " + _lib);
        }

        _command.append("\n_COMPLETION_TOKEN_\n");

        m_processFTable.m_fnWrite(&m_processData, _command.c_str());

        while(!m_processData.m_bComplete)
        {

        }

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _testModule("test", DUCKVIL_SWAP_OUTPUT);
        uint32_t (*get_recorder_index)();
        Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> (*record)(Memory::IMemory* _pMemoryInterface, Memory::__free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

        _module.load(&_testModule);
        _module.get(_testModule, "duckvil_get_recorder_index", (void**)&get_recorder_index);
        _module.get(_testModule, (std::string("duckvil_runtime_reflection_record_") + std::to_string(get_recorder_index())).c_str(), (void**)&record);

        Memory::Vector<RuntimeReflection::__duckvil_resource_type_t> _types = record(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pReflectionRecorderFTable, m_pReflectionFTable, m_pReflectionData);

        for(uint32_t i = 0; i < m_aHotObjects.Size(); i++)
        {
            for(const auto& _type : _types)
            {
                hot_object aa = m_aHotObjects[i];

                if(aa.m_typeHandle.m_ID == _type.m_ID)
                {
                    RuntimeSerializer::Serializer _serializer;

                    _serializer.SetLoading(false);

                    RuntimeReflection::__duckvil_resource_function_t _serializeFunctionHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(m_pReflectionData, _type, "Serialize");
                    RuntimeReflection::__function<void(HotObject::*)(RuntimeSerializer::ISerializer*)>* _func =
                        RuntimeReflection::get_function_callback<HotObject, RuntimeSerializer::ISerializer*>(m_pReflectionData, _type, _serializeFunctionHandle);

                    _serializer.Serialize(*aa.m_pObject, _func);

                    _serializer.SetLoading(true);

                    void* _oldObject = *aa.m_pObject;
                    void* _newObject = RuntimeReflection::create(m_heap.GetMemoryInterface(), m_heap.GetAllocator(), m_pReflectionData, _type);

                    _serializer.Serialize(_newObject, _func);

                    *aa.m_pObject = _newObject;

                    m_heap.GetMemoryInterface()->m_fnFreeListFree_(m_heap.GetAllocator(), _oldObject);
                }
            }
        }
    }

    void RuntimeCompilerSystem::AddHotObject(void** _pHotObject, RuntimeReflection::__duckvil_resource_type_t _typeHandle)
    {
        RuntimeReflection::__duckvil_resource_function_t _castHotObjectFunctionHandle = RuntimeReflection::get_function_handle<void*>(m_pReflectionData, _typeHandle, "Cast");
        RuntimeCompiler::HotObject* _systemInheritance2 = (RuntimeCompiler::HotObject*)RuntimeReflection::invoke_static_result<void*, void*>(m_pReflectionData, _typeHandle, _castHotObjectFunctionHandle, *_pHotObject);

        _systemInheritance2->m_ullHotObjectID++;

        hot_object _hot = {};

        _hot.m_pObject = _pHotObject;
        _hot.m_typeHandle = _typeHandle;

        m_aHotObjects.Allocate(_hot);
    }

}}