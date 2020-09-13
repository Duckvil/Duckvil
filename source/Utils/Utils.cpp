#include "Utils/Utils.h"

#include <sstream>

namespace Duckvil { namespace Utils {

    void split(const char* s, char delim, std::back_insert_iterator<std::vector<const char*>> result)
    {
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            *(result++) = item.c_str();
        }
    }

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

    std::vector<const char*> split(const char* s, char delim)
    {
        std::vector<const char*> elems;
        split(s, delim, std::back_inserter(elems));

        return elems;
    }

    void join_string(string& _sBuffer, const char* _sText, std::size_t& _ullIndex)
    {
        strcpy(_sBuffer.m_sText + _ullIndex, _sText);
        _ullIndex += strlen(_sText);
    }

}}