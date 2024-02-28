#include "Utilities/Strings.hpp"

#include <cctype>

namespace LibKore::Utilities
{
    // clang-format off
    void Strings::trimLeft(std::string& string)
    {
        string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](unsigned char ch)
        { 
            return !std::isspace(ch); 
        }));
    }

    void Strings::trimRight(std::string& string)
    {
        string.erase(std::find_if(string.rbegin(), string.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), string.end());
    }
    // clang-format on

    void Strings::trim(std::string& string)
    {
        trimLeft(string);
        trimRight(string);
    }

    std::vector<std::string> Strings::split(const std::string& string, char delimiter)
    {
        std::vector<std::string> split;

        size_t start;
        size_t end = 0;
        while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
        {
            end = string.find(delimiter, start);
            split.push_back(string.substr(start, end - start));
        }

        return split;
    }
} // namespace LibKore::Utilities
