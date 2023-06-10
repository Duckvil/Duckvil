#include "Utils/Utils.h"

#include <sstream>
#include <limits>

namespace Duckvil { namespace Utils {

    void allocate(string* _pString, const Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, std::size_t _ullLength)
    {
        if(_pMemory == nullptr || _pAllocator == nullptr)
        {
            allocate(_pString, _ullLength);

            return;
        }

        _pString->m_sText = (char*)_pMemory->m_fnFreeListAllocate_(_pAllocator, _ullLength, 8);
        _pString->m_ullLength = _ullLength;
        _pString->m_pAllocator = _pAllocator;
        _pString->m_pMemory = _pMemory;

        memset(_pString->m_sText, 0, _ullLength);
    }

    void allocate(string* _pString, std::size_t _ullLength)
    {
        _pString->m_sText = new char[_ullLength];
        _pString->m_ullLength = _ullLength;
        _pString->m_pAllocator = nullptr;
        _pString->m_pMemory = nullptr;

        memset(_pString->m_sText, 0, _ullLength);
    }

    void allocate(string* _pString, const string& _string)
    {
        allocate(_pString, _string.m_pMemory, _string.m_pAllocator, _string.m_ullLength);
    }

    std::string replace_all(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;

        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return str;
    }

    bool operator==(const string& _lhs, const string& _rhs)
    {
        if(_lhs.m_ullLength == -1 || _rhs.m_ullLength == -1)
        {
            return false;
        }

        return strcmp(_lhs.m_sText, _rhs.m_sText) == 0;
    }

    bool operator!=(const string& _lhs, const string& _rhs)
    {
        return !(_lhs == _rhs);
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

    std::size_t find(const char* _sText, char _cToken, std::size_t _ullLength)
    {
        std::size_t _res = std::numeric_limits<std::size_t>::max();

        for(std::size_t i = 0; i < _ullLength; ++i)
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
        std::size_t _len = _sText.m_ullLength - 1;
        memcpy(_sBuffer.m_sText + _ullLength, _sText.m_sText, _len);
        _ullLength += _len;
    }

    void calculate_string_length(const string& _sText, std::size_t& _ullLength)
    {
        _ullLength += _sText.m_ullLength - 1;
    }

}}