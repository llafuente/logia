#include "logia/ast/semantic_error.h"

#include "logia/ast/node.h"
#include "logia/ast/program.h"
#include "logia/ast/import.h"

#include <format>
#include <stacktrace>

namespace logia::AST
{

    semantic_error::semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line) : std::runtime_error(format_message(node, message, trace, function, file, line)) {}
    // semantic_error::semantic_error(const char *message, const char *function, const char *file, int line): std::runtime_error(format_message(message, function, file, line)) {}

    std::string semantic_error::format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int file_line)
    {
        std::string code_location = node->loc.get_debug_location();

        return std::format("semantic error:\n    \033[31m{}\033[0m\nat: {}\nExpcetion thrown at {} {}:{}\n\nstacktrace:\n{}", message, code_location, function, file, file_line, trace);
    }

    void dump_program(Node *node)
    {
        auto program = node->first_parent<Program>();
        LOG_ERR("{}", program->intrinsics->to_string_tree());
        LOG_ERR("{}", program->to_string_tree());
    }
}
/*
    semantic_error::semantic_error(location loc, const std::string &message)
        : std::runtime_error(format_message(rule, message))
    {
    }

    std::string semantic_error::format_message(location loc, const std::string &message)
    {
        if (!rule)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        auto start = rule->getStart();
        if (!start)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        std::string source = "<unknown>";
        if (auto tokenSource = start->getTokenSource())
        {
            if (auto input = tokenSource->getInputStream())
            {
                source = input->getSourceName();
            }
        }

        return std::format("{} [at {}:{}:{}]", message, source, start->getLine(), start->getCharPositionInLine());
    }

    [[noreturn]] void throw_parser_context_exception(location loc, const std::string &message)
    {
        throw ParserRuleContextException(rule, message);
    }

    [[noreturn]] void throw_node_exception(const Node *node, const std::string &message)
    {
        throw_parser_context_exception(node ? node->loc : nullptr, message);
    }
}
*/