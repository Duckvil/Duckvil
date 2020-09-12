#include "Utils/CommandArgumentsParser.h"

#include <cstring>

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
        m_spArgv += (m_spArgv > 0);

        for(uint32_t i = 0; i < m_iArgc; i++)
        {
            char* argv = m_spArgv[i];

            if(std::strlen(argv) > 0 && argv[0] == '-')
            {
                argv++;

                for(uint32_t j = 0; j < _ullDescriptorsCount; j++)
                {
                    Descriptor& desc = _pDescriptors[j];

                    if(std::strcmp(desc.m_sOption, argv) == 0)
                    {
                        desc.m_bIsSet = true;

                        if(i + 1 < m_iArgc)
                        {
                            char* next = m_spArgv[i + 1];

                            if(std::strlen(next) > 0 && next[0] != '-')
                            {
                                desc.m_sResult = next;

                                i++;
                            }
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