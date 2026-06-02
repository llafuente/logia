#include "logia/ast/expr.h"

namespace logia::AST
{
    /// @brief Binary expression, used for binary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct BinaryExpression : Expression
    {
        /// @brief The operator of the binary expression
        Operators op = Operators::DEFAULT;
        /// @brief Replacement call expression for this binary expression, optional.
        CallExpression *call_expr = nullptr;
        /// @brief Cached type of the binary expression (call expression return type)
        Type *type = nullptr;

        BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, Operators op, Expression *right);

        /// @brief Checks if the operator is an assignment operator (=, +=, -=, etc.)
        bool is_assignament();
        /// @brief Gets the left-hand side expression of the binary expression
        /// @return The left-hand side expression
        Expression *get_left();
        /// @brief Gets the right-hand side expression of the binary expression
        /// @return The right-hand side expression
        Expression *get_right();

        std::string to_string() override;

        Type *get_type() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _set_type(Type *type) override;
        void _pre_type_inference() override;
        void _post_type_inference() override;
    };

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, Operators op, Expression *right);
}