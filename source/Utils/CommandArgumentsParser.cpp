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

    CommandArgumentsParser::CommandArgumentsParser(int _iArgc, const char* _spArgv[]) :
        m_iArgc(_iArgc)
    {
        m_spArgv = new char*[_iArgc];

        for(uint32_t i = 0; i < _iArgc; ++i)
        {
            const char* _current = _spArgv[i];

            m_spArgv[i] = new char[strlen(_current)];

            strcpy(m_spArgv[i], _current);
        }
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

        for(uint32_t i = 0; i < m_iArgc; ++i)
        {
            const char* argv = m_spArgv[i];
            std::size_t _len = std::strlen(argv);

            if(_len > 0 && argv[0] == '-')
            {
                argv++;

                for(uint32_t j = 0; j < _ullDescriptorsCount; ++j)
                {
                    Descriptor& desc = _pDescriptors[j];

                    if(std::strcmp(desc.m_sOption, argv) == 0)
                    {
                        desc.m_bIsSet = true;

                        if(i < m_iArgc - 1)
                        {
                            argv = m_spArgv[i + 1];

                            if(argv[0] != '-')
                            {
                                i++;
                                _len = std::strlen(argv);
                                char* _optionRes = new char[_len + 1];

                                memset(_optionRes, 0, _len + 1);
                                memcpy(_optionRes, argv, _len);

                                desc.m_sResult = _optionRes;

                                break;
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