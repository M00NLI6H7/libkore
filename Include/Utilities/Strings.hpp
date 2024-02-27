#pragma once

#include <string>
#include <vector>

namespace LibKore::Utilities
{
    class Strings
    {
      public:
        static void trimLeft(std::string& string);
        static void trimRight(std::string& string);
        static void trim(std::string& string);

        static std::vector<std::string> split(const std::string& string, char delimiter);
    };
} // namespace LibKore::Utilities
