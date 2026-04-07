#pragma once

#include "ast/types.h"
#include "ast/expr.h"
#include "ast/stmt.h"
#include "utils.h"
#include "backend.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

namespace logia::AST
{
    struct IfStmt : Stmt
    {
        char *name;
        llvm::AllocaInst *ir;

        IfStmt(antlr4::ParserRuleContext *rule, Expression *condition) : Stmt(rule, ast_types::IF_STMT), ir(nullptr)
        {
            this->push_child(condition);
            this->push_child(new Block(nullptr)); // then_block
            this->push_child(new Block(nullptr)); // else_block
            this->push_child(new Block(nullptr)); // continue_block
            this->freezed = true;
        }
        Expression *get_condition()
        {
            return (Expression *)this->children[0];
        }
        Block *get_then()
        {
            return (Block *)this->children[1];
        }
        Block *get_else()
        {
            return (Block *)this->children[2];
        }
        Block *get_continue_block()
        {
            return (Block *)this->children[3];
        }
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void on_after_attach() override
        {
            if (!this->is_attached)
            {
                this->is_attached = true;
                this->get_then()->on_after_attach();
                this->get_else()->on_after_attach();
            }
        }
    };

    /**
     * Creates an if statement
     */
    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition);
}