#pragma once

#include "ast/node.h"
#include "ast/expr.h"
#include "ast/type.h"

namespace logia::AST
{
    // foward declaration
    struct Block;
    struct Type;
    struct Expression;
    struct Identifier;

    struct Stmt : Node
    {
        Stmt(antlr4::ParserRuleContext *rule, ast_types type);
        std::string to_string() override;
        Type *get_type() override;
    };

    struct ReturnStmt : Stmt
    {
        ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr);
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Expression *get_expr();
    };

    struct VarDeclStmt : Stmt
    {
        Type *type;
        llvm::AllocaInst *ir;

        VarDeclStmt(antlr4::ParserRuleContext *rule, Identifier *id, Type *type, Expression *expr);
        Expression *get_expr();
        const char *get_name();
        Identifier *get_identifier();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void on_after_attach() override;
        Type *get_type() override;
        bool pre_type_inference() override;
    };

    struct GotoStmt : Stmt
    {
        char *name;

        GotoStmt(antlr4::ParserRuleContext *rule, char *name);
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    /**
     * Creates a return statement
     */
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret);
    /**
     * Creates a variable declaration
     * Note, to create a constant Type should be readonly.
     */
    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Identifier *id, Type *type, Expression *expr);

}
