#pragma once

#include "logia/ast/stmt.h"

namespace logia::AST
{
    struct Expression;

    /// @brief Return statement
    struct ReturnStmt : Stmt
    {
        ReturnStmt(location loc, Expression *expr);

        std::string to_string() override;

        void post_codegen(logia::Backend *backend) override;
        /// @brief Retrieves return expression
        /// @return return expression
        Expression *get_expr();

        void on_after_attach() override;

        void validate() override;

    protected:
        /// @brief Forward type to return expression
        void _on_set_type(TypeDecl *ty) override;
    };
}