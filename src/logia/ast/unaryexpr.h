#pragma once

#include "logia/ast/callexpr.h"

namespace logia::AST
{
    /// @brief Prefix unary expression, used for prefix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    /// @remarks Reference operator can be an instrinic and it's implemented directly
    struct UnaryExpression : Expression
    {
        /// @brief call expression that replace this unary expression -> operator
        CallExpression *call_expr;

        /// @brief Unary expression type
        Type *type = nullptr;

        /// @brief operator
        Operators op;

        UnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand);

        Expression *get_operand();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;

    protected:
        void _set_type(Type *ty) override;
        void _pre_type_inference() override;
    };

    /// @brief Creates a reference to an expression
    /// @param operand
    /// @return
    LOGIA_API LOGIA_LEND UnaryExpression *ast_create_ref(Expression *operand);

    /// @brief Creates a prefix unary expression
    /// @param op The prefix unary operator
    /// @param operand The operand of the prefix unary expression
    /// @return The created prefix unary expression
    LOGIA_API LOGIA_LEND UnaryExpression *ast_create_preunary_expr(Operators op, Expression *operand);
}