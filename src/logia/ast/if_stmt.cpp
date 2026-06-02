#include "logia/ast/if_stmt.h"

#include "logia/log.h"
#include "logia/ast/llvm.h"
#include "logia/ast/identifier.h"

namespace logia::AST
{

    uint64_t if_stmt_count = 0;

    IfStmt::IfStmt(antlr4::ParserRuleContext *rule, Expression *condition) : Stmt(rule)
    {
        this->push_child(condition);
        auto then_name = std::format("{}_{}", "then_block", if_stmt_count);
        this->push_child(new Block(nullptr, new Identifier(rule, then_name.c_str())));

        auto else_name = std::format("{}_{}", "else_block", if_stmt_count);
        this->push_child(new Block(nullptr, new Identifier(rule, else_name.c_str())));

        auto continue_name = std::format("{}_{}", "continue_block", if_stmt_count);
        this->push_child(new Block(nullptr, new Identifier(rule, continue_name.c_str())));

        ++if_stmt_count;

        this->freezed = true;
    }
    Expression *IfStmt::get_condition()
    {
        return this->get_child<Expression>(0);
    }
    Block *IfStmt::get_then()
    {
        return this->get_child<Block>(1);
    }
    Block *IfStmt::get_else()
    {
        return this->get_child<Block>(2);
    }
    Block *IfStmt::get_continue_block()
    {
        return this->get_child<Block>(3);
    }

    std::string IfStmt::to_string()
    {
        return std::string(std::format("IfStmt{} ", Node::to_string()));
    }

    llvm::Value *IfStmt::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        auto condition = this->get_condition()->codegen(backend);
        auto then_body = this->get_then();
        auto else_body = this->get_else();
        auto continue_body = this->get_continue_block();
        then_body->pre_codegen(backend);
        auto then_block = then_body->ir_basicblock;
        else_body->pre_codegen(backend);
        auto else_block = else_body->ir_basicblock;
        continue_body->pre_codegen(backend);
        auto continue_block = continue_body->ir_basicblock;

        // NOTE create before codegen each block so the blocks are attached to function before codegen
        auto v = backend->builder->CreateCondBr(condition, then_block, else_block);
        auto func = backend->builder->GetInsertBlock()->getParent();

        bool continue_block_required = false;

        then_body->codegen(backend);

        if (!ast_llvm_block_has_terminator(then_body->ir_basicblock))
        {
            LOG(DBG, "then_block has no terminator -> br");
            continue_block_required = true;
            backend->builder->CreateBr(continue_block);
        }

        else_body->codegen(backend);
        if (!ast_llvm_block_has_terminator(then_body->ir_basicblock))
        {
            LOG(DBG, "else_block has no terminator -> br");
            continue_block_required = true;
            backend->builder->CreateBr(continue_block);
        }

        if (continue_block_required)
        {
            continue_body->codegen(backend);
            this->cg_value = continue_block;
            return Node::post_codegen(backend);
        }

        this->cg_value = else_block;
        return Node::post_codegen(backend);
    }

    void IfStmt::post_attach()
    {
        if (!this->is_attached)
        {
            this->is_attached = true;
            this->get_then()->post_attach();
            this->get_else()->post_attach();
        }
    }

    void IfStmt::_post_type_inference()
    {
        // condition should have bool type!
        auto condition = this->get_condition();
        auto ty = condition->get_final_type();
        Integer *int_ty;
        if (ty->try_cast<Integer>(&int_ty))
        {
            if (int_ty->bits == 1)
            {
                // ok !
                return Stmt::_post_type_inference();
            }
        }

        throw_semantic_error(condition, std::format("LGERR_IF001 Expected contition type to be 'bool' but found: '{}'", ty->get_repr()));
    }

    void IfStmt::_set_type(Type *t) {}

    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition)
    {
        LOGIA_ASSERT(condition == nullptr);

        return new IfStmt(nullptr, condition);
    }
}