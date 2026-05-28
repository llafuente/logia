#include "logia/ast/block.h"

#include "logia/log.h"

#include "logia/ast/llvm.h"
#include "logia/ast/type.h"
#include "logia/ast/stmt.h"
#include "logia/ast/identifier.h"
#include "logia/ast/semantic_error.h"
#include "logia/ast/functionblock.h"

namespace logia::AST
{
    //
    // Block
    //

    Block::Block(antlr4::ParserRuleContext *rule, Identifier *name) : Scope(rule), name(name)
    {
        name->skip_codegen = true; // even if it's not reachable we should be careful
    }

    Identifier *Block::get_identifier()
    {
        return this->name;
    }

    const char *Block::get_name()
    {
        return this->get_identifier()->identifier;
    }

    Type *Block::get_type()
    {
        return nullptr;
    }

    std::string Block::to_string()
    {
        std::string list;
        for (const auto &pair : this->scope)
        {
            list += list.empty() ? "" : ", ";
            list += pair.first;
        }

        return std::format("Block[{} {} statements] scope[{}]{}", this->name->identifier, this->children.size(), list, Node::to_string());
    }

    void Block::post_attach()
    {
        Scope::post_attach();

        // TODO we should throw if the block is moved outside current function!!
        if (!this->is_attached)
        {
            this->is_attached = true;

            try
            {
                auto fblock = this->first_parent<FunctionBlock>();
                fblock->scope_set(this->get_name(), this);
            }
            catch (std::exception e)
            {
                throw_semantic_error(this, "Expected block to be attached to a function");
            }
        }
    }

    void Block::pre_codegen(logia::Backend *backend)
    {
        if (this->llvm_basicblock != nullptr)
        {
            return;
        }

        this->llvm_basicblock = llvm::BasicBlock::Create(backend->context, this->get_name(), nullptr);

        LOG(DBG, "{} name = {} llvm_basicblock = {}", this->to_string(), this->get_name(), (void *)this->llvm_basicblock);
    }

    llvm::Value *Block::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value)
        {
            LOG(WRN, "Why??");
            return this->cg_value;
        }

        LOG(DBG, "{}", this->to_string());

        // unhandled block inside a function block
        auto previous_block = backend->builder->GetInsertBlock();
        LOG(DBG, "previous_block = {}", (void *)previous_block);

        if (!ast_llvm_block_has_terminator(previous_block))
        {
            backend->builder->CreateBr(this->llvm_basicblock); // goto
        }
        // insert block into current function
        auto func = backend->builder->GetInsertBlock()->getParent();
        func->insert(func->end(), this->llvm_basicblock);
        backend->builder->SetInsertPoint(this->llvm_basicblock);

        this->codegen_children(backend);

        this->cg_value = this->llvm_basicblock;
        return Node::post_codegen(backend);
    }
    void Block::codegen_children(logia::Backend *backend)
    {
        if (backend->debug && backend->dscopes.size())
        {
            auto scope = backend->dbuilder->createLexicalBlock(
                backend->dscopes[backend->dscopes.size() - 1],
                backend->dfile,
                this->rule->start->getLine(),
                this->rule->start->getCharPositionInLine());

            backend->dscopes.push_back(scope);
        }

        auto max = this->children.size();
        auto last = max - 1;
        for (size_t i = 0; i < max; ++i)
        {
        process_child:
            Node *n = this->children[i];
            LOG(DBG, "codegen.statement[{}] = {}", i, n->to_string());

            auto inst = n->codegen(backend);

            // if the current block has a terminator and we continue we run into multiple problems like:
            // fix: All predecessors must be dead!
            // lli could execute the program by jit fail
            // we should skip until next block
            // This is an ill formated program and could lead to problems/cashes later
            // we need a way to detect this "dead code" and raise a semantic_errror -> the coder to remove it!
            if (this->llvm_basicblock && i != last && ast_llvm_block_has_terminator(backend->builder->GetInsertBlock()))
            {
                LOG(WRN, "skip until next block, current block has terminator");
                ++i;
                do
                {
                    n = this->children[i];
                    if (n->is<Block>())
                    {
                        goto process_child;
                    }

                    LOG(WRN, "skip [{}] = {}", i, n->to_string());
                    ++i;
                } while (i < max);
                --i;
            }
        }
        if (backend->debug && backend->dscopes.size())
        {
            backend->dscopes.pop_back();
        }
    }

    LOGIA_API Block *ast_create_block(Identifier *name)
    {
        return new Block(nullptr, name);
    }
}