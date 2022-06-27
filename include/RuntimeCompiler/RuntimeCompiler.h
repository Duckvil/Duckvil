#pragma once

#include "RuntimeCompiler/ICompiler.h"

#include "Memory/FreeList.h"

#include "Process/Process.h"

namespace Duckvil { namespace RuntimeCompiler {

    class Compiler : public ICompiler
    {
    private:
        ICompiler* m_pCompiler;

        Process::ftable m_processFTable;
        Process::data m_processData;

    public:
        Compiler(const Memory::FreeList& _heap);
        ~Compiler();

        bool Setup();
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