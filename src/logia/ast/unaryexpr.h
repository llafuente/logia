#pragma once

#include "logia/ast/expr.h"
#include "logia/ast/operators.h"

namespace logia::AST
{
    struct Type;
    struct CallExpression;

    /// @brief Prefix unary expression, used for prefix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    /// @remarks Reference operator can be an instrinic and it's implemented directly
    struct UnaryExpression : Expression
    {
        /// @brief call expression that replace this unary expression -> operator
        CallExpression *call_expr;

        /// @brief operator
        Operators op;

        UnaryExpression(location loc, Operators op, Expression *operand);

        Expression *get_operand();

        /// @brief A binary expression is constant if left and right are constants
        /// @return true if constant, false otherwise
        maybe_semantic_error can_execute() override;
        /// @brief Executes the binary expression if it is constant at compile time
        /// @return Returns the result of the expression execution
        ConstExpression *execute() override;

        std::string to_string() override;

        std::string to_code(size_t ident = 0) override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        bool type_inference(size_t pass_id) override;

    protected:
        void _on_set_type(TypeDecl *ty) override;
    };
}