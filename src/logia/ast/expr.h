#pragma once

#include "logia/ast/node.h"
#include "logia/ast/type.h"
#include "logia/ast/operators.h"

namespace logia::AST
{
    /// @brief Base expression
    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
}