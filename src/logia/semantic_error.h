#pragma once

#include <stdexcept>
#include <string>
#include <stacktrace>

namespace logia::AST
{
    class compiler_error : public std::runtime_error
    {
    public:
        compiler_error(const std::string &message, const std::string &trace, const char *function, const char *file, int line);
        // compiler_error(const char *message, const char *function, const char *file, int line);

        std::string format_message(const std::string &message, const std::string &trace, const char *function, const char *file, int line);
    };

#define throw_compiler_error(message)                                                                                \
    do                                                                                                               \
    {                                                                                                                \
        throw compiler_error(message, std::to_string(std::stacktrace::current()), __FUNCTION__, __FILE__, __LINE__); \
    } while (false)
}