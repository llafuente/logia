#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{
    struct CallExpression;

    /// @brief Defines a cast expression
    struct Cast : Expression
    {
        /// @brief Some casting operations are in fact a function
        CallExpression *callexpr = nullptr;
        /// @brief Constructs a cast expression
        /// @param rule The parser rule context
        /// @param expr The expression to cast
        /// @param to The target type
        Cast(
            antlr4::ParserRuleContext *rule,
            Expression *expr,
            Type *to);
        /// @brief Gets the source type of the cast
        /// @return The source type
        Type *get_source_type();
        /// @brief Gets the expression being cast
        /// @return The expression being cast
        Expression *get_expr();
        /// @brief Gets the target type of the cast
        /// @return The target type
        Type *get_target_type();

        std::string to_string() override;
        Type *get_type() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        /// @brief Overrides current target type
        /// @param type The new target type
        void _set_type(Type *type) override;
        void _post_type_inference() override;
    };
}