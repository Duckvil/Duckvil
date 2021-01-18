#pragma once

#include <string>
#include <vector>

namespace Duckvil { namespace RuntimeCompiler {

    struct Options
    {
        std::vector<std::string> m_aFlags;
        std::vector<std::string> m_aDefines;
        std::vector<std::string> m_aIncludes;
        std::vector<std::string> m_aLibrariesPaths;
        std::vector<std::string> m_aLibraries;
    };

    enum Flag
    {
        Flag_None = 0,
        Flag_SharedLibrary = 1 << 0,
        Flag_StaticLibrary = 1 << 1, // in feature
        Flag_DebugInfo = 1 << 2
    };

    struct ICompiler
    {
        virtual void Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions) = 0;

        virtual void AddDefine(const std::string& _sDefine) = 0;
        virtual void AddFlag(const std::string& _sFlag) = 0;
        virtual void AddFlag(Flag _flags) = 0;
        virtual void AddInclude(const std::string& _sInclude) = 0;
        virtual void AddLibraryPath(const std::string& _sLibraryPath) = 0;
        virtual void AddLibrary(const std::string& _sLibrary) = 0;
    };

}}