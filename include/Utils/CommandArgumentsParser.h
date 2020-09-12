#pragma once

#include <cstddef>
#include <cstdint>

namespace Duckvil { namespace Utils {

    class CommandArgumentsParser
    {
    public:
        struct Descriptor
        {
            Descriptor()
            {

            }

            template <typename A>
            Descriptor(A _uiIndex, const char* _sOption) :
                m_uiIndex((uint32_t)_uiIndex)
            {
                m_bIsSet =  false;
                m_sResult = 0;
                m_sOption = _sOption;
            }

            uint32_t    m_uiIndex;
            const char* m_sOption;
            const char* m_sResult;
            bool        m_bIsSet;
        };

    private:
        int         m_iArgc;
        char**      m_spArgv;

    public:
        CommandArgumentsParser(int _iArgc, char* _spArgv[]);
        ~CommandArgumentsParser();

        bool Parse(CommandArgumentsParser::Descriptor* _pDescriptors, std::size_t _ullDescriptorsCount);

        template <typename A>
        inline const Descriptor& operator[](A _index) { return m_pDescriptors[(uint32_t)_index]; }

        template <typename A>
        inline const Descriptor& operator[](A _index) const { return m_pDescriptors[(uint32_t)_index]; }
    };

}}