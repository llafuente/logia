#pragma once

#include "logia/ast/node.h"
#include "logia/ast/semantic_error.h"

namespace logia
{
    struct Backend;
}

namespace logia::AST
{
    struct ConstExpression;
    /// @brief Base expression
    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(location loc);
        Expression(const Expression &other);

        std::string to_string() override;

        /// @brief Determines if the expression is constant at compile time
        /// @return true if constant, false otherwise
        virtual maybe_semantic_error can_execute();

        /// @brief Executes the expression if it is constant at compile time
        /// @return Returns the result of the expression execution
        virtual ConstExpression *execute();

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
}