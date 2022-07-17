#include "Utils/TemplateEngine.h"

#include <fstream>

namespace Duckvil { namespace TemplateEngine {

    void generate(const Utils::string& _sTemplatePath, const Utils::string& _sOutputPath, const std::map<Utils::string, Utils::string>& _aParameters)
    {
        std::ifstream _tplFile(_sTemplatePath);
        std::ofstream _oFile(_sOutputPath);
        std::string _line;
        bool _skip = false;

        while(getline(_tplFile, _line))
        {
            if(_skip)
            {
                if(_line == "{{endif}}")
                {
                    _skip = false;
                }

                continue;
            }

            size_t _pos = _line.find("{{");
            bool _isStatement = false;

            if(_pos != std::string::npos)
            {
                std::string _expression(_line.begin() + _pos + 2, _line.end());
                size_t _spacePos = _expression.find(" ");

                _isStatement = true;

                if(_spacePos != std::string::npos)
                {
                    std::string _statement(_expression.begin(), _expression.begin() + _spacePos);

                    if(_statement == "if")
                    {
                        size_t _endPos = _expression.find("}}");
                        bool _found = false;

                        _statement = std::string(_expression.begin() + _spacePos + 1, _expression.begin() + _endPos);

                        for(const auto& _parameter : _aParameters)
                        {
                            if(_parameter.first == _statement)
                            {
                                _found = true;

                                break;
                            }
                        }

                        if(!_found)
                        {
                            _skip = true;

                            continue;
                        }
                    }
                }
            }

            if(!_isStatement)
            {
                for(const auto& _parameter : _aParameters)
                {
                    _line = Utils::replace_all(_line, ("{$" + _parameter.first + "}").m_sText, _parameter.second.m_sText);
                }

                _oFile << _line << "\n";
            }
        }

        _oFile.close();
        _tplFile.close();
    }

    void generate(const std::map<Utils::string, Utils::string>& _aPaths, const std::map<Utils::string, Utils::string>& _aParameters)
    {
        for(const auto& _path : _aPaths)
        {
            generate(_path.first, _path.second, _aParameters);
        }
    }

}}