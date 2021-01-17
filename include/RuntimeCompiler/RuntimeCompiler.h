#pragma once

#include "RuntimeCompiler/ICompiler.h"

#include "Memory/FreeList.h"

#include "Process/Process.h"

namespace Duckvil { namespace RuntimeCompiler {

    class Compiler
    {
    private:
        ICompiler* m_pCompiler;

        Process::ftable m_processFTable;
        Process::data m_processData;

    public:
        Compiler(const Memory::FreeList& _heap);
        ~Compiler();

        bool Setup();
        void Compile(const std::vector<std::string>& _aFiles, const CompilerOptions& _compilerOptions);

        void AddDefine(const std::string& _sDefine);
        void AddFlag(const std::string& _sFlag);
        void AddInclude(const std::string& _sInclude);
        void AddLibraryPath(const std::string& _sLibraryPath);
        void AddLibrary(const std::string& _sLibrary);
    };

}}