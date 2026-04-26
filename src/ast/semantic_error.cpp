#include "ast/semantic_error.h"

#include "ast/node.h"
#include "ast/program.h"
#include "ast/import.h"

#include <format>
#include <stacktrace>

namespace logia::AST
{

    semantic_error::semantic_error(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line) : std::runtime_error(format_message(node, message, trace, function, file, line)) {}
    // semantic_error::semantic_error(const char *message, const char *function, const char *file, int line): std::runtime_error(format_message(message, function, file, line)) {}

    std::string semantic_error::format_message(Node *node, const std::string &message, const std::string &trace, const char *function, const char *file, int line)
    {
        std::string code_location;
        if (node->rule == nullptr)
        {
            code_location = std::format("at <unknown>:0:0", message);
        }
        else
        {
            // TODO handle error at import
            // TODO extract code snippet!!
            auto program = node->first_parent<Program>();

            code_location = std::format("at {}:{}:{}", program->entry_point_file, node->rule->start->getLine(), node->rule->start->getCharPositionInLine());
        }

        return std::format("semantic error:\n    \033[31m{}\033[0m\nat: {}\nExpcetion thrown at {} {}:{}\n\nstacktrace:\n{}", message, code_location, function, file, line, trace);
    }
}
/*
    semantic_error::semantic_error(antlr4::ParserRuleContext *rule, const std::string &message)
        : std::runtime_error(format_message(rule, message))
    {
    }

    std::string semantic_error::format_message(antlr4::ParserRuleContext *rule, const std::string &message)
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

    [[noreturn]] void throw_parser_context_exception(antlr4::ParserRuleContext *rule, const std::string &message)
    {
        throw ParserRuleContextException(rule, message);
    }

    [[noreturn]] void throw_node_exception(const Node *node, const std::string &message)
    {
        throw_parser_context_exception(node ? node->rule : nullptr, message);
    }
}
*/