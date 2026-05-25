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

    //
    // utils
    //

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_postfix_unary_operator_to_string(Operators op, Type *operand);

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_prefix_unary_operator_to_string(Operators op, Type *operand);

    /// @brief Converts a binary operator to its string representation
    /// @param op The binary operator
    /// @param left The left operand type
    /// @param right The right operand type
    /// @return The string representation of the binary operator
    LOGIA_API const char *ast_binary_operator_to_string(Operators op, Type *left, Type *right);

}