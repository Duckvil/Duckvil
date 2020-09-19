#include "Utils/CommandArgumentsParser.h"

#include <cstring>
#include <limits>

#include "Utils/Utils.h"

namespace Duckvil { namespace Utils {

    CommandArgumentsParser::CommandArgumentsParser(int _iArgc, char* _spArgv[]) :
        m_iArgc(_iArgc),
        m_spArgv(_spArgv)
    {

    }

    CommandArgumentsParser::~CommandArgumentsParser()
    {

    }

    bool CommandArgumentsParser::Parse(CommandArgumentsParser::Descriptor* _pDescriptors, std::size_t _ullDescriptorsCount)
    {
        if(_pDescriptors == 0 || _ullDescriptorsCount == 0)
        {
            return true;
        }

        m_pDescriptors = _pDescriptors;
        m_iArgc -= (m_iArgc > 0);
        m_spArgv += (m_spArgv != nullptr);

        for(uint32_t i = 0; i < m_iArgc; i++)
        {
            char* argv = m_spArgv[i];
            std::size_t _len = std::strlen(argv);

            if(_len > 0 && argv[0] == '-')
            {
                argv++;

                std::size_t _index = Utils::find(argv, ' ', _len - 1);
                char* _optionRes = 0;

                if(_index != std::numeric_limits<std::size_t>::max())
                {
                    _optionRes = new char[_len - _index];
                    char* _argv = new char[_index + 1];

                    _argv[_index] = 0;

                    _optionRes = argv + _index + 1;
                    memcpy(_argv, argv, _index);
                    argv = _argv;
                }

                for(uint32_t j = 0; j < _ullDescriptorsCount; j++)
                {
                    Descriptor& desc = _pDescriptors[j];

                    if(std::strcmp(desc.m_sOption, argv) == 0)
                    {
                        desc.m_bIsSet = true;

                        if(_index != std::numeric_limits<std::size_t>::max())
                        {
                            desc.m_sResult = _optionRes;

                            delete[] argv;
                        }
                    }
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

}}