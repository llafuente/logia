#include "ast/semantic_error.h"

namespace logia::AST
{

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