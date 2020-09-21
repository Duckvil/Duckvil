#pragma once

#include <filesystem>

#include "PlugNPlay/Module.h"

namespace Duckvil { namespace PlugNPlay {

    class AutoLoader
    {
    private:
        std::filesystem::path m_sLoadPath;
        module m_module;

    public:
        AutoLoader(const std::filesystem::path& _sLoadPath);
        ~AutoLoader();

        void LoadAll();
    };

}}