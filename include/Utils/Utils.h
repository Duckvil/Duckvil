#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <type_traits>
#include <cstring>

namespace Duckvil { namespace Utils {

    template<typename T1, typename T2, typename... Error>
    struct is_string : std::false_type {};

    template<typename T, typename... Checking>
    struct is_string<T, T, Checking...> : is_string<T, Checking...> {};

    template<typename T>
    struct is_string<const char*, T> : std::true_type {};

    struct string
    {
        string(std::size_t _ullSize)
        {
            m_sText = new char[_ullSize];

            memset(m_sText, 0, _ullSize);
        }

        ~string()
        {
            delete[] m_sText;
        }

        char* m_sText;
    };

    void split(const char* s, char delim, std::back_insert_iterator<std::vector<const char*>> result);
    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);
    std::vector<const char*> split(const char* s, char delim);

    void join_string(string& _sBuffer, const char* _sText, std::size_t& _ullIndex);

    template <typename... A>
    void join(string& _Buffer, A&& ... _sText)
    {
        static_assert(is_string<typename std::decay<A>::type...>::value,
            "All types are not const char*");

        std::size_t _index = 0;

        int _[] = { 0, (join_string(_Buffer, _sText, _index), 0)... };
    }

}}