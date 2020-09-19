#include "Utils/Utils.h"

#include <sstream>
#include <limits>

namespace Duckvil { namespace Utils {

    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result)
    {
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));

        return elems;
    }

    std::size_t find(const char* _sText, char _cToken, std::size_t _ullLength)
    {
        std::size_t _res = std::numeric_limits<std::size_t>::max();

        for(std::size_t i = 0; i < _ullLength; i++)
        {
            if(_sText[i] == _cToken)
            {
                _res = i;

                break;
            }
        }

        return _res;
    }

    void join_string(string& _sBuffer, const string& _sText, std::size_t& _ullLength)
    {
        std::size_t _len = _sText.m_ullLength;
        memcpy(_sBuffer.m_sText + _ullLength, _sText.m_sText, _len);
        _ullLength += _len - 1;
    }

    void calculate_string_length(const string& _sText, std::size_t& _ullLength)
    {
        _ullLength += _sText.m_ullLength - 1;
    }

}}