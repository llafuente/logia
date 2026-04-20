#include "ast/if_stmt.h"
#include "ast/llvm.h"

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
        return std::string(std::format("IfStmt {} ", static_cast<void *>(this)));
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

        bool continue_block_required = false;

        then_body->codegen(codegen, builder);

        if (!ast_llvm_block_has_terminator(then_body->llvm_basicblock))
        {
            DEBUG() << "then_block has no terminator -> br";
            continue_block_required = true;
            builder->CreateBr(continue_block);
        }

        else_body->codegen(codegen, builder);
        if (!ast_llvm_block_has_terminator(then_body->llvm_basicblock))
        {
            DEBUG() << "else_block has no terminator -> br";
            continue_block_required = true;
            builder->CreateBr(continue_block);
        }

        if (continue_block_required)
        {
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