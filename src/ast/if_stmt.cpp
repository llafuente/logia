#include "ast/if_stmt.h"

namespace logia::AST
{

    uint64_t if_stmt_count = 0;

    IfStmt::IfStmt(antlr4::ParserRuleContext *rule, Expression *condition) : Stmt(rule, ast_types::IF_STMT), ir(nullptr)
    {
        this->push_child(condition);
        auto then_name = std::format("{}_{}", "then_block", if_stmt_count);
        this->push_child(new Block(nullptr, ast_create_identifier(then_name.c_str())));

        auto else_name = std::format("{}_{}", "else_block", if_stmt_count);
        this->push_child(new Block(nullptr, ast_create_identifier(else_name.c_str())));

        auto continue_name = std::format("{}_{}", "continue_block", if_stmt_count);
        this->push_child(new Block(nullptr, ast_create_identifier(continue_name.c_str())));

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
        return std::string("IfStmt: ");
    }

    llvm::Value *IfStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

        auto condition = this->get_condition()->codegen(codegen, builder);
        auto then_body = this->get_then();
        auto else_body = this->get_else();
        auto continue_body = this->get_continue_block();
        then_body->pre_codegen(codegen);
        auto then_block = then_body->llvm_basicblock;
        else_body->pre_codegen(codegen);
        auto else_block = else_body->llvm_basicblock;
        continue_body->pre_codegen(codegen);
        auto continue_block = continue_body->llvm_basicblock;

        // NOTE create before codegen each block so the blocks are attached to function before codegen
        auto v = builder->CreateCondBr(condition, then_block, else_block);
        auto func = builder->GetInsertBlock()->getParent();
        func->insert(func->end(), then_block);
        func->insert(func->end(), else_block);

        bool continue_block_inserted = false;
        auto _require_continue_block = [func, continue_block, &continue_block_inserted]() -> void
        {
            if (!continue_block_inserted)
            {
                continue_block_inserted = true;
                func->insert(func->end(), continue_block);
            }
        };

        then_body->codegen(codegen, builder);

        auto terminator = builder->GetInsertBlock()->getTerminator();
        if (terminator && llvm::isa<llvm::ReturnInst>(terminator))
        {
            DEBUG() << "then_block has terminator return" << std::endl;
        }
        else if (terminator && llvm::isa<llvm::UnreachableInst>(terminator))
        {
            DEBUG() << "then_block has terminator unreachable" << std::endl;
        }
        else
        {
            DEBUG() << "then_block has no terminator -> br";
            _require_continue_block();
            builder->CreateBr(continue_block);
        }

        else_body->codegen(codegen, builder);

        terminator = builder->GetInsertBlock()->getTerminator();
        if (terminator && llvm::isa<llvm::ReturnInst>(terminator))
        {
            DEBUG() << "else_block has terminator return";
        }
        else if (terminator && llvm::isa<llvm::UnreachableInst>(terminator))
        {
            DEBUG() << "else_block has terminator unreachable";
        }
        else
        {
            DEBUG() << "else_block has no terminator -> br";
            _require_continue_block();
            builder->CreateBr(continue_block);
        }

        if (continue_block_inserted)
        {
            builder->SetInsertPoint(continue_block);
            continue_body->codegen(codegen, builder);
            return continue_block;
        }

        return else_block;
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

    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition)
    {
        LOGIA_ASSERT(condition);

        return new IfStmt(nullptr, condition);
    }
}