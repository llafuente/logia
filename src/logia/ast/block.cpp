#include "logia/ast/block.h"

#include "logia/log.h"
#include "utils.h"

#include "logia/backend.h"
#include "logia/ast/llvm.h"
#include "logia/ast/types/type.h"
#include "logia/ast/stmt.h"
#include "logia/ast/identifier.h"
#include "logia/ast/semantic_error.h"
#include "logia/ast/functionblock.h"

#include "llvm/IR/BasicBlock.h"

namespace logia::AST
{
    //
    // Block
    //

    Block::Block(location loc, Identifier *name) : Scope(loc), name(name)
    {
        LOGIA_VERIFY(name != nullptr);
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
        return std::format("Block[{} {} statements] {}", this->name->identifier, this->children.size(), Scope::to_string());
    }

    std::string Block::to_code(size_t ident)
    {
        // repeat LOGIA_IDENT_STRING ident times
        std::string ident_str;
        for (size_t i = 0; i < ident; ++i)
        {
            ident_str += "    ";
        }

        // accumulate code from children separated by newlines
        return std::format("{{\n{}\n{}}}", std::accumulate(this->children.begin(), this->children.end(), std::string(), [ident_str](const std::string &acc, Node *child)
                                                           { return acc + ident_str + child->to_code() + "\n"; }),
                           ident_str);
    }

    void Block::on_after_attach()
    {
        Scope::on_after_attach();

        // TODO we should throw if the block is moved outside current function!!
        // TODO what happen when we double-set because we move something an gets re-attached ??
    }

    void Block::validate()
    {
        Scope::validate();

        auto ident = this->get_identifier();
        if (!ident->is_empty())
        {
            auto result = scope_lookup_all(this, this->get_name());
            if (result.is_success())
            {
                auto list = result.unwrap_success();
                if (list.size())
                {
                    // ups! collisions!
                    int i = 1;
                    std::string redeclarations;
                    for (const auto &redeclaration : list)
                    {
                        redeclarations += std::format("declared {} here:\n{}\n", i++, redeclaration->loc.get_debug_location(0, 0));
                    }

                    throw_semantic_error(this, std::format(LGERR_BLK001, this->get_name(), this->loc.get_debug_location(3, 1), redeclarations));
                }
            }
            // ok, nobody has our id -> register in parent "not in myself"
            logia::AST::scope_set(this->parent_node, this->get_name(), this, true);
        }
    }

    void Block::pre_codegen(logia::Backend *backend)
    {
        if (this->ir_basicblock != nullptr)
        {
            return;
        }

        this->ir_basicblock = llvm::BasicBlock::Create(backend->context, this->get_name(), nullptr);

        LOG(DBG, "{} name = {} ir_basicblock = {}", this->to_string(), this->get_name(), (void *)this->ir_basicblock);
        Node::pre_codegen(backend);
    }

    void Block::post_codegen(logia::Backend *backend)
    {
        if (is_post_codegen)
        {
            throw_compiler_error("retry ?");
        }

        LOG(DBG, "{}", this->to_string());

        // unhandled block inside a function block
        auto previous_block = backend->builder->GetInsertBlock();
        LOG(DBG, "previous_block = {}", (void *)previous_block);

        if (!ast_llvm_block_has_terminator(previous_block))
        {
            backend->builder->CreateBr(this->ir_basicblock); // goto
        }
        // insert block into current function
        auto func = backend->builder->GetInsertBlock()->getParent();
        func->insert(func->end(), this->ir_basicblock);
        backend->builder->SetInsertPoint(this->ir_basicblock);

        this->post_codegen_children(backend);

        return Node::post_codegen(backend);
    }
    void Block::post_codegen_children(logia::Backend *backend)
    {
        if (backend->debug)
        {
            LOG(DBG, "scopes {}", backend->dscopes.size());
            LOGIA_VERIFY(backend->dscopes.size() > 0);
            auto scope = backend->dbuilder->createLexicalBlock(
                backend->dscopes[backend->dscopes.size() - 1],
                backend->dfile,
                this->loc.start_line,
                this->loc.start_column);

            backend->dscopes.push_back(scope);
        }

        auto max = this->children.size();
        auto last = max - 1;
        for (size_t i = 0; i < max; ++i)
        {
        process_child:
            Node *n = this->children[i];
            LOG(DBG, "codegen.statement[{}] = {}", i, n->to_string());

            n->post_codegen(backend);

            // if the current block has a terminator and we continue we run into multiple problems like:
            // fix: All predecessors must be dead!
            // lli could execute the program by jit fail
            // we should skip until next block
            // This is an ill formated program and could lead to problems/cashes later
            // we need a way to detect this "dead code" and raise a semantic_errror -> the coder to remove it!
            if (this->ir_basicblock && i != last && ast_llvm_block_has_terminator(backend->builder->GetInsertBlock()))
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
        if (backend->debug)
        {
            LOGIA_VERIFY(backend->dscopes.size() > 0);
            backend->dscopes.pop_back();
        }
    }
}