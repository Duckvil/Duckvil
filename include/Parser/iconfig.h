#pragma once

#include <string>

namespace Duckvil { namespace Parser {

    struct IConfig
    {
        virtual void AddInclude(const std::string& _sInclude) = 0;
        virtual void AddMacroDefinition(const std::string& _sName, const std::string& _sDefinition) = 0;
    };

}}