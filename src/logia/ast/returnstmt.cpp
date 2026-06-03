#pragma once

#include "logia/log.h"
#include "logia/backend.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/llvm.h"
#include "logia/ast/expr.h"

namespace logia::AST
{
    ReturnStmt::ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr) : Stmt(rule)
    {
        this->push_child(expr);
    }

    std::string ReturnStmt::to_string()
    {
        return std::format("ReturnStmt{}", Node::to_string());
    }

    Expression *ReturnStmt::get_expr()
    {
        return this->get_child<Expression>(0);
    }

    void ReturnStmt::_set_type(Type *ty)
    {
        this->get_expr()->set_type(ty);
    }

    llvm::Value *ReturnStmt::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        auto expr = this->get_expr();
        if (!expr)
        {
            this->cg_value = backend->builder->CreateRetVoid();
        }
        else
        {
            auto value = llvm_load_if_required(expr->codegen(backend), backend);
            this->cg_value = backend->builder->CreateRet(value);
        }

        return Stmt::post_codegen(backend);
    }

    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret)
    {
        auto stmt = new ReturnStmt(nullptr, ret);
        return stmt;
    }
}