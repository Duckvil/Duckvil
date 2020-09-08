#include "Utils/CommandArgumentsParser.h"

#include <string>

namespace Duckvil { namespace Utils {

    CommandArgumentsParser::CommandArgumentsParser(int _iArgc, char* _sarrArgv[], CommandArgumentsParser::Descriptor* _pDescriptors, size_t _ullDescriptorsCount) :
        m_pDescriptors(_pDescriptors),
        m_ullDescriptorsCount(_ullDescriptorsCount)
    {
        _iArgc -= (_iArgc > 0);
        _sarrArgv += (_sarrArgv > 0);

        for(uint32_t i = 0; i < _iArgc; i++)
        {
            char* argv = _sarrArgv[i];

            if(strlen(argv) > 0 && argv[0] == '-')
            {
                argv++;

                for(uint32_t j = 0; j < m_ullDescriptorsCount; j++)
                {
                    Descriptor& desc = m_pDescriptors[j];

                    if(strcmp(desc.m_sOption, argv) == 0)
                    {
                        desc.m_bIsSet = true;

                        if(i + 1 < _iArgc)
                        {
                            char* next = _sarrArgv[i + 1];

                            if(strlen(next) > 0 && next[0] != '-')
                            {
                                desc.m_sResult = next;

                                i++;
                            }
                        }
                    }
                }
            }
        }
    }

    CommandArgumentsParser::~CommandArgumentsParser()
    {

    }

}}