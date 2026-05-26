#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{
    /// @brief Defines a cast expression
    struct Cast : Expression
    {
        /// @brief Some casting operations are in fact a function
        CallExpression *callexpr = nullptr;

        Cast(
            antlr4::ParserRuleContext *rule,
            Expression *expr,
            Type *to);
        Type *get_from_type();
        Expression *get_expr();
        Type *get_to_type();

        std::string to_string() override;
        Type *get_type() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _set_type(Type *type);
        void _post_type_inference() override;
    };
}