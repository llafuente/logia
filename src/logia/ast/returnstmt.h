#pragma once

#include "logia/ast/stmt.h"

namespace logia::AST
{
    struct Expression;

    /// @brief Return statement
    struct ReturnStmt : Stmt
    {
        ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr);

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;
        /// @brief Retrieves return expression
        /// @return return expression
        Expression *get_expr();

    protected:
        /// @brief Forward type to return expression
        void _set_type(Type *ty) override;
    };

    /// @brief Creates a return statement
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret);

}