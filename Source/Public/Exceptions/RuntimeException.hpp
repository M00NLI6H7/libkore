#pragma once

#include "backward.hpp"

#include <iostream>
#include <stdexcept>

namespace LibKore::Exceptions
{
    class RuntimeException : public std::runtime_error
    {
      public:
        explicit RuntimeException(const char* message);
        explicit RuntimeException(const std::string& message);

        void printStackTrace(std::ostream& stream = std::cout);
    };
} // namespace LibKore::Exceptions
