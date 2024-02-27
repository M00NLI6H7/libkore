#include "Exceptions/RuntimeException.hpp"

#include "fmt/format.h"

namespace LibKore::Exceptions
{
    RuntimeException::RuntimeException(const char* message)
        : runtime_error(message)
    {
    }

    RuntimeException::RuntimeException(const std::string& message)
        : runtime_error(message)
    {
    }

    void RuntimeException::printStackTrace(std::ostream& stream)
    {
        backward::StackTrace stackTrace{};
        stackTrace.load_here();

        backward::Printer stackTracePrinter{};
        stackTracePrinter.print(stackTrace, stream);
    }
} // namespace LibKore::Exceptions