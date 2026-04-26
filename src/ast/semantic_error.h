#pragma once

#include <stdexcept>
#include <string>
#include <stacktrace>

#include "antlr4-runtime.h"
#include "ast/node.h"

namespace logia::AST
{
    class semantic_error : public std::runtime_error
    {
    public:
        semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
        // semantic_error(const char *message, const char *function, const char *file, int line);

        std::string format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
    };

#define throw_semantic_error(node, message)                                                                                \
    do                                                                                                                     \
    {                                                                                                                      \
        throw semantic_error(node, message, std::to_string(std::stacktrace::current()), __FUNCTION__, __FILE__, __LINE__); \
    } while (false)
}