#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{
    struct Identifier;

    /// @brief Member access expression, used for struct field access and method calls
    struct MemberAccessExpression : Expression
    {
        Type *type = nullptr;

        MemberAccessExpression(location loc, Expression *left, Expression *right);
        /// @brief Retrieves the left expression of the member access
        /// @return The left expression
        Expression *get_left();
        /// @brief Retrieves the right expression of the member access
        /// @return The right expression
        Expression *get_right();

        std::string to_string() override;

        void post_codegen(logia::Backend *backend) override;

        Type *get_type() override;

        void on_after_attach() override;

        void validate() override;

        bool type_inference(size_t pass_id) override;

    protected:
        void _on_set_type(TypeDecl *type) override;
    };
    /*
        type x  = struct { float x; float y;}
        var x xi = {}
        x.x // <-- NamedMemberAccessExpression / Dot access
    */
    /// @brief Named member access expression, used for struct/type field access and method calls with identifier on the right side
    class NamedMemberAccessExpression : public MemberAccessExpression
    {
    public:
        NamedMemberAccessExpression(location loc, Expression *left, Identifier *right);
        Identifier *get_right_ident();
        void validate() override;
    };

    /*
        // constexpr values
        type x  = struct { float x; float y;}
        var x xi = {}
        x["x"] // <-- StringLiteral

        var z = int[10](0)
        z[10] // <-- IntLiteral

        // runtime values
        var z = int[10](0)
        var idx = 10
        z[idx] // <-- (value) Identifier
        z[xxx()] // <-- (value) CallExpr
    */
    class ComputedMemberAccessExpression : public MemberAccessExpression
    {
    public:
        ComputedMemberAccessExpression(location loc, Expression *left, Expression *right);
        bool is_constexpr();
        std::string get_constexpr_value();
        void validate() override;
    };

} // namespace logia
