#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <type_traits>
#include <cstring>

namespace Duckvil { namespace Utils {

    struct string
    {
        string()
        {
            m_sText = 0;
            m_ullLength = 0;
        }

        string(std::size_t _ullLength)
        {
            Allocate(_ullLength);
        }

        ~string()
        {
            delete[] m_sText;
        }

        void Allocate(std::size_t _ullLength)
        {
            m_sText = new char[_ullLength];
            m_ullLength = _ullLength;

            memset(m_sText, 0, _ullLength);
        }

        char* m_sText;
        std::size_t m_ullLength;
    };

    void split(const char* s, char delim, std::back_insert_iterator<std::vector<const char*>> result);
    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);
    std::vector<const char*> split(const char* s, char delim);

    void join_string(string& _sBuffer, const char* _sText, std::size_t& _ullLength);
    void calculate_string_length(const char* _sText, std::size_t& _ullLength);

    template <typename... A>
    void join(string& _buffer, A&& ... _sText)
    {
        if(_buffer.m_ullLength == 0)
        {
            std::size_t _ullLength = 0;
            int _[] = { 0, (calculate_string_length(_sText, _ullLength), 0)... };

            _buffer.Allocate(_ullLength + 1);
        }

        std::size_t _index = 0;
        int _[] = { 0, (join_string(_buffer, _sText, _index), 0)... };
    }

}}