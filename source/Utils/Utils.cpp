#include "Utils/Utils.h"

#include <sstream>

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

    void join_string(string& _sBuffer, const string& _sText, std::size_t& _ullLength)
    {
        std::size_t _len = _sText.m_ullLength;
        strcpy_s(_sBuffer.m_sText + _ullLength, _len, _sText.m_sText);
        _ullLength += _len - 1;
    }

    void calculate_string_length(const string& _sText, std::size_t& _ullLength)
    {
        _ullLength += _sText.m_ullLength - 1;
    }

}}