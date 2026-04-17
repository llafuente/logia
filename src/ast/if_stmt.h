#pragma once

#include "ast/types.h"
#include "ast/expr.h"
#include "ast/stmt.h"
#include "utils.h"
#include "logia/backend.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

namespace logia::AST
{
    extern uint64_t if_stmt_count;

    struct IfStmt : Stmt
    {
        char *name;
        llvm::AllocaInst *ir;

        IfStmt(antlr4::ParserRuleContext *rule, Expression *condition);
        Expression *get_condition();
        Block *get_then();
        Block *get_else();
        Block *get_continue_block();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void post_attach() override;
    };

    /**
     * Creates an if statement
     */
    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition);
}