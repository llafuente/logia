#pragma once

#include "ast/stmt.h"
#include "utils.h"
#include "logia/backend.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

namespace logia::AST
{
    struct Expression;
    extern uint64_t if_stmt_count;

    // references: https://clang.llvm.org/doxygen/classclang_1_1IfStmt.html

    struct IfStmt : Stmt
    {
        IfStmt(antlr4::ParserRuleContext *rule, Expression *condition);
        Expression *get_condition();
        Block *get_then();
        Block *get_else();
        Block *get_continue_block();
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        void post_attach() override;
    };

    /**
     * Creates an if statement
     */
    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition);
}