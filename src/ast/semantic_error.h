#pragma once

#include "antlr4-runtime.h"
#include "ast/node.h"

namespace logia::AST
{
    class semantic_error : std::runtime_error
    {
    public:
        semantic_error(antlr4::ParserRuleContext *rule, const std::string &message);
    };
    [[noreturn]] void throw_parser_context_exception(antlr4::ParserRuleContext *rule, const std::string &message);
    [[noreturn]] void throw_node_exception(const Node *node, const std::string &message);
}