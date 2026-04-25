#include "logia/compiler_error.h"
#include <format>
#include <stacktrace>

namespace logia
{

    compiler_error::compiler_error(const std::string &message, const std::string &trace, const char *function, const char *file, int line) : std::runtime_error(format_message(message, trace, function, file, line)) {}
    // compiler_error::compiler_error(const char *message, const char *function, const char *file, int line): std::runtime_error(format_message(message, function, file, line)) {}

    std::string compiler_error::format_message(const std::string &message, const std::string &trace, const char *function, const char *file, int line)
    {
        return std::format("compiler error:\n    \033[31m{}\033[0m\nat {} {}:{}\n\nstacktrace:\n{}", message, function, file, line, trace);
    }
}
