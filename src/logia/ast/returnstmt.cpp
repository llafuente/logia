#include "logia/ast/returnstmt.h"

#include "logia/log.h"
#include "logia/backend.h"
#include "logia/ast/llvm.h"
#include "logia/ast/expr.h"

namespace logia::AST
{
    ReturnStmt::ReturnStmt(location loc, Expression *expr) : Stmt(loc)
    {
        this->has_type = true;
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

    void ReturnStmt::on_after_attach() {}

    void ReturnStmt::validate() {}

    void ReturnStmt::_on_set_type(TypeDecl *ty)
    {
        this->get_expr()->set_type(ty);
    }

    void ReturnStmt::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        auto expr = this->get_expr();
        llvm::ReturnInst *ret;
        if (!expr)
        {
            ret = backend->builder->CreateRetVoid();
        }
        else
        {
            auto value = llvm_load_if_required(expr->get_codegen_value(backend), backend);
            ret = backend->builder->CreateRet(value);
        }
        backend->set_debug_loc(ret, this->loc);

        return Stmt::post_codegen(backend);
    }
}