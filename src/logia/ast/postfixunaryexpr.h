#pragma once

#include "logia/ast/callexpr.h"

namespace logia::AST
{
    /// @brief Postfix unary expression, used for postfix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct PostfixUnaryExpression : CallExpression
    {
        Operators op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;

    protected:
        virtual void _post_type_inference() override;
    };
}