#include "logia/ast/expr.h"

namespace logia::AST
{
    /// @brief Binary expression, used for binary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct BinaryExpression : Expression
    {
        Operators op = Operators::DEFAULT;
        CallExpression *call_expr = nullptr;
        Type *type = nullptr;

        BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, Operators op, Expression *right);

        bool is_assignament();
        Expression *get_left();
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