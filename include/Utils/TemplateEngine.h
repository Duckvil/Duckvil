#pragma once

#include "Utils/Utils.h"

#include <map>

namespace Duckvil { namespace TemplateEngine {

    void generate(const Utils::string& _sTemplatePath, const Utils::string& _sOutputPath, const std::map<Utils::string, Utils::string>& _aParameters);
    void generate(const std::map<Utils::string, Utils::string>& _aPaths, const std::map<Utils::string, Utils::string>& _aParameters);

}}