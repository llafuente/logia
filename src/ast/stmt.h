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

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

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

    /// @brief Variable declaration/initialization statement
    struct VarDeclStmt : Stmt
    {
        /// @brief AllocaInst for this variable, populated at pre_codegen and cached for later use
        llvm::AllocaInst *alloca_inst;
        /// @brief Cached type
        Type *type;

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

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;

        /// @brief Returns the type of the variable
        /// @remarks While not necessary (logia do not support chained vardecl) could be usefull when referencing the variable
        /// @return Type of the variable
        Type *get_type() override;

    protected:
        /// @brief Sets type based on declaration or expression type
        void _pre_type_inference() override;
        /// @brief Sets type
        void _set_type(Type *ty) override;
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

    /// @brief Creates a return statement
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret);

    /// @brief Creates a variable declaration
    /// Note, to create a constant Type should be readonly.
    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Identifier *id, Type *type, Expression *expr);

    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Identifier *id);

}
