#include "RuntimeCompiler/RuntimeCompiler.h"

#include "RuntimeCompiler/Platform/Windows/Compiler.h"
#include "RuntimeCompiler/Platform/Linux/Compiler.h"

#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"

namespace Duckvil { namespace RuntimeCompiler {

    Compiler::Compiler(const Memory::FreeList& _heap)
    {
        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _processModuleInfo("Process");

        _module.load(&_processModuleInfo);

        const Process::ftable* (*_duckvilProcessInit)();

        _module.get(_processModuleInfo, "duckvil_process_init", (void**)&_duckvilProcessInit);

        m_processFTable = _duckvilProcessInit();

        m_processFTable->m_fnInit(_heap.GetMemoryInterface(), _heap.GetAllocator(), &m_processData);
        m_processFTable->m_fnSetup(&m_processData);
    }

    Compiler::~Compiler()
    {

    }

    bool Compiler::Setup()
    {
#ifdef DUCKVIL_PLATFORM_WINDOWS
        m_pCompiler = new WindowsCompiler(*m_processFTable, &m_processData);

        return true;
#else
#ifdef DUCKVIL_PLATFORM_LINUX
        m_pCompiler = new LinuxCompiler(m_processFTable, &m_processData);

        return true;
#else
        return false;
#endif
#endif
    }

    void Compiler::Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions)
    {
        m_pCompiler->Compile(_aFiles, _compilerOptions);
    }

    void Compiler::AddDefine(const std::string& _sDefine)
    {
        m_pCompiler->AddDefine(_sDefine);
    }

    void Compiler::AddFlag(const std::string& _sFlag)
    {
        m_pCompiler->AddFlag(_sFlag);
    }

    void Compiler::AddFlag(Flag _flags)
    {
        m_pCompiler->AddFlag(_flags);
    }

    void Compiler::AddInclude(const std::string& _sInclude)
    {
        m_pCompiler->AddInclude(_sInclude);
    }

    void Compiler::AddLibraryPath(const std::string& _sLibraryPath)
    {
        m_pCompiler->AddLibraryPath(_sLibraryPath);
    }

    void Compiler::AddLibrary(const std::string& _sLibrary)
    {
        m_pCompiler->AddLibrary(_sLibrary);
    }

    const std::vector<std::string>& Compiler::GetDefines()
    {
        return m_pCompiler->GetDefines();
    }

    const std::vector<std::string>& Compiler::GetFlags()
    {
        return m_pCompiler->GetFlags();
    }

    const std::vector<std::string>& Compiler::GetIncludes()
    {
        return m_pCompiler->GetIncludes();
    }

    const std::vector<std::string>& Compiler::GetLibrariesPaths()
    {
        return m_pCompiler->GetLibrariesPaths();
    }

    const std::vector<std::string>& Compiler::GetLibraries()
    {
        return m_pCompiler->GetLibraries();
    }

}}