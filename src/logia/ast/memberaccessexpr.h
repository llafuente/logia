#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{
    /// @brief Member access expression, used for struct field access and method calls
    struct MemberAccessExpression : Expression
    {
        Type *type = nullptr;

        MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right);
        /// @brief Retrieves the left expression of the member access
        /// @return The left expression
        Expression *get_left();
        /// @brief Retrieves the right identifier of the member access
        /// @return The right identifier
        Identifier *get_right();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;

        Node *resolve() override;

    protected:
        void _set_type(Type *type) override;
        void _pre_type_inference() override;
    };

    /// @brief Creates a member access expression
    /// @param left The left expression of the member access
    /// @param right The right identifier of the member access
    /// @return The created member access expression
    LOGIA_API LOGIA_LEND MemberAccessExpression *ast_create_memberaccess_expr(Node *left, Node *right);

} // namespace logia
