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

        string(const string& _string)
        {
            Allocate(_string.m_ullLength);
            memcpy(m_sText, _string.m_sText, _string.m_ullLength);
        }

        string(const char* _sText, std::size_t _ullLength)
        {
            Allocate(_ullLength);
            memcpy(m_sText, _sText, _ullLength);
        }

        template <std::size_t Length>
        string(const char (&_sText)[Length])
        {
            Allocate(Length);
            memcpy(m_sText, _sText, Length);
        }

        ~string()
        {
            delete[] m_sText;
        }

        template <std::size_t Length>
        string& operator=(const char (&_sText)[Length])
        {
            Allocate(Length);
            memcpy(m_sText, _sText, Length);

            return *this;
        }

        string& operator=(const string& _string)
        {
            m_ullLength = _string.m_ullLength;

            Allocate(m_ullLength);
            memcpy(m_sText, _string.m_sText, m_ullLength);

            return *this;
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

    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);

    std::size_t find(const char* _sText, char _cToken, std::size_t _ullLength);

    template <std::size_t Length>
    void join_string(string& _sBuffer, const char (&_sText)[Length], std::size_t& _ullLength)
    {
        memcpy(_sBuffer.m_sText + _ullLength, _sText, Length);
        _ullLength += Length - 1;
    }

    template <std::size_t Length>
    void calculate_string_length(const char (&_sText)[Length], std::size_t& _ullLength)
    {
        _ullLength += Length - 1;
    }

    void join_string(string& _sBuffer, const string& _sText, std::size_t& _ullLength);
    void calculate_string_length(const string& _sText, std::size_t& _ullLength);

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