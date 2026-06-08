#pragma once

#include <stdexcept>
#include <string>
#include <stacktrace>

#include "logia/log.h"

namespace logia::AST
{
    struct Node;
#define LGERR_ID001 "LGERR_ID001 Identifier '{}' not found in current scope"
#define LGERR_ID002 "LGERR_ID002 Found '{}' elements in current scope mattching identifier '{}' but expected one:\n{}"
#define LGERR_SCOPE003 "LGERR_SCOPE003 Found '{}' of type '{}' but expected type '{}'"

#define LGERR_GT001 "LGERR_GT001 use of undeclared or unreachable label '{}' "
#define LGERR_GT002 "LGERR_GT002 Identifier '{}' has multiple matches: {}"
#define LGERR_GT003 "LGERR_GT003 expected label '{}' to reference a block, but found:\n{}"

#define LGERR_BLK001 "LGERR_BLK001 Identifier '{}' in use:\n{}"

#define LGERR_VDECL001 "LGERR_VDECL001 Redeclaration/Identifier '{}' in use:\n{}"

#define LGERR_CONSTEX000 "LGERR_CONSTEX000 Expression is not constant"
#define LGERR_CONSTEX001a "LGERR_CONSTEX001a Left operand is not constant"
#define LGERR_CONSTEX001b "LGERR_CONSTEX001b Right operand is not constant"
#define LGERR_CONSTEX002 "LGERR_CONSTEX002 Right operand should be an Integer"
#define LGERR_CONSTEX005 "LGERR_CONSTEX005 Unsupported operator in constant expression"
#define LGERR_IMP004 "LGERR_IMP004 import list should contain only identifiers, invalid element at position {}"
#define LGERR_IMP003 "LGERR_IMP003 target of import should be a scope"
#define LGERR_IMP002 "LGERR_IMP002 cannot use import into scope and import list together"
#define LGERR_IMP001 "LGERR_IMP001 cannot use import all and import list together"
#define LGERR_IMP005 "LGERR_IMP005 package list should contain only identifiers, invalid element at position {}"

    class semantic_error : public std::runtime_error
    {
    public:
        semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
        // semantic_error(const char *message, const char *function, const char *file, int line);

        std::string format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line);
    };

#define throw_semantic_error(node, message)                                                                                                    \
    do                                                                                                                                         \
    {                                                                                                                                          \
        auto ___e = ::logia::AST::semantic_error(node, message, std::to_string(std::stacktrace::current()), __FUNCTION__, __FILE__, __LINE__); \
        LOG_ERR("{}", ___e.what());                                                                                                            \
        throw ___e;                                                                                                                            \
    } while (false)
}

// resolve cyclic dependency

#include "logia/maybe_error.h"

namespace logia::AST
{
    struct Node;

    using namespace logia::utils;

    typedef maybe_error<bool, Node *> maybe_semantic_error;

    constexpr auto make_semantic_error = make_error<bool, logia::AST::Node *>;
    constexpr auto make_semantic_success = make_success<bool, logia::AST::Node *>;
    constexpr auto make_semantic_chained_error = make_chained_error<bool, logia::AST::Node *>;
}