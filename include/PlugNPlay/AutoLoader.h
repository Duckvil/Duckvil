#pragma once

#include <filesystem>

#include "PlugNPlay/Module.h"

#include "Memory/Vector.h"

namespace Duckvil { namespace PlugNPlay {

    class AutoLoader
    {
    private:
        std::filesystem::path m_sLoadPath;
        __module m_module;

    public:
        AutoLoader(const std::filesystem::path& _sLoadPath);
        ~AutoLoader();

        void LoadAll(const Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, __module_information** _pModules, uint32_t* _pLoaded);
        void LoadAll(const Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, Memory::Vector<__module_information>* _aLoaded);
    };

}}