#pragma once

#include <stdexcept>
#include <string>
#include <stacktrace>

#include "antlr4-runtime.h"
#include "logia/log.h"
#include "logia/ast/node.h"

namespace logia::AST
{

#define LGERR_CONSTEX001a "LGERR_CONSTEX001a Left operand is not constant"
#define LGERR_CONSTEX001b "LGERR_CONSTEX001b Right operand is not constant"
#define LGERR_CONSTEX002 "LGERR_CONSTEX002 Right operand should be an Integer"
#define LGERR_CONSTEX005 "LGERR_CONSTEX005 Unsupported operator in constant expression"

    class semantic_error : public std::runtime_error
    {
    public:
        semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
        // semantic_error(const char *message, const char *function, const char *file, int line);

        std::string format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
    };

    struct maybe_semantic_error
    {
        Node *aggressor;
        std::string &message;
    };

    typedef utils::maybe_error<bool, maybe_semantic_error> maybe_error_semantic;
    constexpr auto make_semantic_error = logia::utils::make_error<bool, maybe_semantic_error>;
    constexpr auto make_semantic_success = logia::utils::make_success<bool, maybe_semantic_error>;
    constexpr auto make_semantic_chained_error = logia::utils::make_chained_error<bool, maybe_semantic_error>;

#define throw_semantic_error(node, message)                                                                                                    \
    do                                                                                                                                         \
    {                                                                                                                                          \
        auto ___e = ::logia::AST::semantic_error(node, message, std::to_string(std::stacktrace::current()), __FUNCTION__, __FILE__, __LINE__); \
        LOG(ERR, "{}", ___e.what());                                                                                                           \
        throw ___e;                                                                                                                            \
    } while (false)
}