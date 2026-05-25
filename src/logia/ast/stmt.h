#pragma once

#include "logia/ast/node.h"
#include "logia/ast/expr.h"
#include "logia/ast/type.h"

namespace logia::AST
{
    // forward declaration
    struct Block;
    struct Type;
    struct Expression;
    struct Identifier;

    /// @brief Base class for all statements
    struct Stmt : Node
    {
        Stmt(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        Type *get_type() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

    /// @brief Goto statement
    struct GotoStmt : Stmt
    {
        GotoStmt(antlr4::ParserRuleContext *rule, Identifier *id);

        /// @brief Returns the identifier of the target label
        /// @return Identifier of the target label
        Identifier *get_identifier();

        /// @brief Returns the name of the target label
        /// @return Name of the target label
        const char *get_name();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        /// @brief Do nothing
        void _set_type(Type *ty) override;
    };

    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Identifier *id);

}
