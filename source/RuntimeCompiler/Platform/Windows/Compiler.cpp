#include "RuntimeCompiler/Platform/Windows/Compiler.h"

namespace Duckvil { namespace RuntimeCompiler {

    WindowsCompiler::WindowsCompiler(const Process::ftable& _processFtable, Process::data* _pProcessData) :
        m_processFTable(_processFtable),
        m_pProcessData(_pProcessData)
    {

    }

    WindowsCompiler::~WindowsCompiler()
    {

    }

    void WindowsCompiler::Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions)
    {
        std::string _command = "cl";

        for(const auto& _flag : m_compilerOptions.m_aFlags)
        {
            _command.append(" " + _flag);
        }

        for(const auto& _flag : _compilerOptions.m_aFlags)
        {
            _command.append(" " + _flag);
        }

        for(const auto& _define : m_compilerOptions.m_aDefines)
        {
            _command.append(" /D" + _define);
        }

        for(const auto& _define : _compilerOptions.m_aDefines)
        {
            _command.append(" /D" + _define);
        }

        for(const auto& _include : m_compilerOptions.m_aIncludes)
        {
            _command.append(" /I\"" + _include + "\"");
        }

        for(const auto& _include : _compilerOptions.m_aIncludes)
        {
            _command.append(" /I\"" + _include + "\"");
        }

        for(const std::string& _file : _aFiles)
        {
            _command.append(" " + _file);
        }

        _command.append(" /link");

        for(const auto& _libPath : m_compilerOptions.m_aLibrariesPaths)
        {
            _command.append(" /LIBPATH:\"" + _libPath + "\"");
        }

        for(const auto& _libPath : _compilerOptions.m_aLibrariesPaths)
        {
            _command.append(" /LIBPATH:\"" + _libPath + "\"");
        }

        for(const auto& _lib : m_compilerOptions.m_aLibraries)
        {
            _command.append(" " + _lib);
        }

        for(const auto& _lib : _compilerOptions.m_aLibraries)
        {
            _command.append(" " + _lib);
        }

        _command.append("\n_COMPLETION_TOKEN_\n");

        m_processFTable.m_fnWrite(m_pProcessData, _command.c_str());
        m_processFTable.m_fnWait(m_pProcessData);
    }

    void WindowsCompiler::AddDefine(const std::string& _sDefine)
    {
        m_compilerOptions.m_aDefines.push_back(_sDefine);
    }

    void WindowsCompiler::AddFlag(const std::string& _sFlag)
    {
        m_compilerOptions.m_aFlags.push_back(_sFlag);
    }

    void WindowsCompiler::AddFlag(Flag _flags)
    {
        if(_flags & Flag::Flag_SharedLibrary)
        {
            m_compilerOptions.m_aFlags.push_back("/LD");
        }

        if(_flags & Flag::Flag_DebugInfo)
        {
            m_compilerOptions.m_aFlags.push_back("/Zi");
        }
    }

    void WindowsCompiler::AddInclude(const std::string& _sInclude)
    {
        m_compilerOptions.m_aIncludes.push_back(_sInclude);
    }

    void WindowsCompiler::AddLibraryPath(const std::string& _sLibraryPath)
    {
        m_compilerOptions.m_aLibrariesPaths.push_back(_sLibraryPath);
    }

    void WindowsCompiler::AddLibrary(const std::string& _sLibrary)
    {
        m_compilerOptions.m_aLibraries.push_back(_sLibrary);
    }

    const std::vector<std::string>& WindowsCompiler::GetDefines()
    {
        return m_compilerOptions.m_aDefines;
    }

    const std::vector<std::string>& WindowsCompiler::GetFlags()
    {
        return m_compilerOptions.m_aDefines;
    }

    const std::vector<std::string>& WindowsCompiler::GetIncludes()
    {
        return m_compilerOptions.m_aIncludes;
    }

    const std::vector<std::string>& WindowsCompiler::GetLibrariesPaths()
    {
        return m_compilerOptions.m_aLibrariesPaths;
    }

    const std::vector<std::string>& WindowsCompiler::GetLibraries()
    {
        return m_compilerOptions.m_aLibraries;
    }

}}