#pragma once

#include "RuntimeCompiler/ICompiler.h"

#include "Process/Process.h"

namespace Duckvil { namespace RuntimeCompiler {

    class LinuxCompiler : public ICompiler
    {
    private:
        Options m_compilerOptions;

        Process::ftable m_processFTable;
        Process::data* m_pProcessData;

    public:
        LinuxCompiler(const Process::ftable& _processFtable, Process::data* _pProcessData);
        ~LinuxCompiler();

        void Compile(const std::vector<std::string>& _aFiles, const Options& _compilerOptions) override;

        void AddDefine(const std::string& _sDefine) override;
        void AddFlag(const std::string& _sFlag) override;
        void AddFlag(Flag _flags) override;
        void AddInclude(const std::string& _sInclude) override;
        void AddLibraryPath(const std::string& _sLibraryPath) override;
        void AddLibrary(const std::string& _sLibrary) override;
    };

}}