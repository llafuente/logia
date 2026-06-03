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

        /// @brief Determines if the expression is constant at compile time
        /// @return true if constant, false otherwise
        virtual maybe_semantic_error is_constant() = 0;

        /// @brief Executes the expression if it is constant at compile time
        /// @return Returns the result of the expression execution
        virtual ConstExpression execute() = 0;

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
}