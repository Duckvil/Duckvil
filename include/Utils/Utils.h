#pragma once

#include <string>
#include <vector>
#include <iterator>

namespace Duckvil { namespace Utils {

    void split(const char* s, char delim, std::back_insert_iterator<std::vector<const char*>> result);
    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);
    std::vector<const char*> split(const char* s, char delim);

}}