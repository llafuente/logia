#pragma once

#include "ast/node.h"
#include "ast/expr.h"
#include "ast/type.h"

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
    };

    /// @brief Return statement
    struct ReturnStmt : Stmt
    {
        ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr);
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Expression *get_expr();
    };

    /// @brief Variable declaration/initialization statement
    struct VarDeclStmt : Stmt
    {
        llvm::AllocaInst *alloca;

        VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr);

        /// @brief Returns initializer expression
        /// @return
        Expression *get_expr();

        /// @brief  Returns variable name
        /// @return
        const char *get_name();

        /// @brief Returns the identifier of the variable
        /// @return
        Identifier *get_identifier();

        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void post_attach() override;
        Type *get_type() override;
        bool pre_type_inference() override;
        void post_type_inference() override;
    };

    /// @brief Goto statement
    struct GotoStmt : Stmt
    {
        GotoStmt(antlr4::ParserRuleContext *rule, Identifier *id);
        Identifier *get_identifier();
        const char *get_name();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    /// @brief Creates a return statement
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret);

    /// @brief Creates a variable declaration
    /// Note, to create a constant Type should be readonly.
    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Identifier *id, Type *type, Expression *expr);

    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Identifier *id);

}
