#include "RuntimeCompiler/Platform/Linux/Compiler.h"

namespace Duckvil { namespace RuntimeCompiler {

    LinuxCompiler::LinuxCompiler(const Process::ftable& _processFtable, Process::data* _pProcessData) :
        m_processFTable(_processFtable),
        m_pProcessData(_pProcessData)
    {

    }

    LinuxCompiler::~LinuxCompiler()
    {

    }

    void LinuxCompiler::Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions)
    {
        std::string _command = "gcc";

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
            _command.append(" -D" + _define);
        }

        for(const auto& _define : _compilerOptions.m_aDefines)
        {
            _command.append(" -D" + _define);
        }

        for(const auto& _include : m_compilerOptions.m_aIncludes)
        {
            _command.append(" -I" + _include);
        }

        for(const auto& _include : _compilerOptions.m_aIncludes)
        {
            _command.append(" -I" + _include);
        }

        for(const std::string& _file : _aFiles)
        {
            _command.append(" " + _file);
        }

        for(const auto& _libPath : m_compilerOptions.m_aLibrariesPaths)
        {
            _command.append(" -L" + _libPath);
        }

        for(const auto& _libPath : _compilerOptions.m_aLibrariesPaths)
        {
            _command.append(" -L" + _libPath);
        }

        for(const auto& _lib : m_compilerOptions.m_aLibraries)
        {
            _command.append(" -l:" + _lib);
        }

        for(const auto& _lib : _compilerOptions.m_aLibraries)
        {
            _command.append(" -l:" + _lib);
        }

        m_processFTable.m_fnWrite(m_pProcessData, _command.c_str());
    }

    void LinuxCompiler::AddDefine(const std::string& _sDefine)
    {
        m_compilerOptions.m_aDefines.push_back(_sDefine);
    }

    void LinuxCompiler::AddFlag(const std::string& _sFlag)
    {
        m_compilerOptions.m_aFlags.push_back(_sFlag);
    }

    void LinuxCompiler::AddFlag(Flag _flags)
    {
        if(_flags & Flag::Flag_SharedLibrary)
        {
            m_compilerOptions.m_aFlags.push_back("-shared");
        }

        if(_flags & Flag::Flag_SharedLibrary)
        {
            m_compilerOptions.m_aFlags.push_back("-g");
        }
    }

    void LinuxCompiler::AddInclude(const std::string& _sInclude)
    {
        m_compilerOptions.m_aIncludes.push_back(_sInclude);
    }

    void LinuxCompiler::AddLibraryPath(const std::string& _sLibraryPath)
    {
        m_compilerOptions.m_aLibrariesPaths.push_back(_sLibraryPath);
    }

    void LinuxCompiler::AddLibrary(const std::string& _sLibrary)
    {
        m_compilerOptions.m_aLibraries.push_back(_sLibrary);
    }

    const std::vector<std::string>& LinuxCompiler::GetDefines()
    {
        return m_compilerOptions.m_aDefines;
    }

    const std::vector<std::string>& LinuxCompiler::GetFlags()
    {
        return m_compilerOptions.m_aDefines;
    }

    const std::vector<std::string>& LinuxCompiler::GetIncludes()
    {
        return m_compilerOptions.m_aIncludes;
    }

    const std::vector<std::string>& LinuxCompiler::GetLibrariesPaths()
    {
        return m_compilerOptions.m_aLibrariesPaths;
    }

    const std::vector<std::string>& LinuxCompiler::GetLibraries()
    {
        return m_compilerOptions.m_aLibraries;
    }

}}