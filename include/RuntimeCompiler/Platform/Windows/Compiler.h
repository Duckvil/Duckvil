#pragma once

#include "RuntimeCompiler/ICompiler.h"

#include "Process/Process.h"

#include <vector>

namespace Duckvil { namespace RuntimeCompiler {

    class WindowsCompiler : public ICompiler
    {
    private:
        Options m_compilerOptions;

        Process::ftable m_processFTable;
        Process::data* m_pProcessData;

    public:
        WindowsCompiler(const Process::ftable& _processFtable, Process::data* _pProcessData);
        ~WindowsCompiler();

        void Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions) override;

        void AddDefine(const std::string& _sDefine) override;
        void AddFlag(const std::string& _sFlag) override;
        void AddFlag(Flag _flags) override;
        void AddInclude(const std::string& _sInclude) override;
        void AddLibraryPath(const std::string& _sLibraryPath) override;
        void AddLibrary(const std::string& _sLibrary) override;

        const std::vector<std::string>& GetDefines() override;
        const std::vector<std::string>& GetFlags() override;
        const std::vector<std::string>& GetIncludes() override;
        const std::vector<std::string>& GetLibrariesPaths() override;
        const std::vector<std::string>& GetLibraries() override;
    };

}}