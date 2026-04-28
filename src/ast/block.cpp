#include "ast/block.h"
#include "ast/traverse.h"
#include "ast/llvm.h"
#include "ast/semantic_error.h"

namespace logia::AST
{
    //
    // Scope
    //
    Scope::Scope(antlr4::ParserRuleContext *rule) : Node(rule) {}

    void Scope::scope_set(const char *name, Node *node)
    {
        if (!node->is<Type>() && !node->is<Block>() && !node->is<VarDeclStmt>() && !node->is<FunctionParameter>())
        {
            throw std::runtime_error(std::format("invalid node type: {} - {}", typeid(node).name(), node->to_string()));
        }
        this->scope[strdup(name)] = node;
    }

    void Scope::scope_copy(std::vector<Identifier *> list, Scope *target)
    {
        for (const auto &it : list)
        {
            const char *name = it->identifier;
            // TODO handle don't exist -> compiler_error
            auto node = this->scope[name];

            target->scope_set(name, node);
        }
    }
    void Scope::scope_copy_all(Scope *target)
    {
        for (const auto &it : scope)
        {
            target->scope_set(it.first.data(), it.second);
        }
    }

    void Scope::post_attach()
    {
        // keep parent body in sync regardless being already attached, allow blocks to be moved.
        auto parentBody = this->first_parent<Scope>();
        LOGIA_ASSERT(parentBody);
        this->parentScope = parentBody;
    }

    Type *Scope::get_type()
    {
        return nullptr;
    }

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

            if (!is<FunctionBlock>())
            {
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
    }

    void Block::pre_codegen(logia::Backend *backend)
    {
        if (this->llvm_basicblock != nullptr)
        {
            return;
        }

        this->llvm_basicblock = llvm::BasicBlock::Create(backend->context, this->get_name(), nullptr);

        DEBUG() << this->to_string() << " name=" << this->get_name() << " llvm_basicblock = " << this->llvm_basicblock << std::endl;
    }

    llvm::Value *Block::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value)
        {
            LWARNING() << "why ??" << std::endl;
            return this->cg_value;
        }

        DEBUG() << this->to_string() << std::endl;

        // unhandled block inside a function block
        auto previous_block = backend->builder->GetInsertBlock();
        DEBUG() << "previous_block = " << previous_block << std::endl;

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
            DEBUG() << "codegen.statement[" << i << "] " << n->to_string() << std::endl;
            auto inst = n->codegen(backend);

            // if the current block has a terminator and we continue we run into multiple problems like:
            // fix: All predecessors must be dead!
            // lli could execute the program by jit fail
            // we should skip until next block
            // This is an ill formated program and could lead to problems/cashes later
            // we need a way to detect this "dead code" and raise a semantic_errror -> the coder to remove it!
            if (this->llvm_basicblock && i != last && ast_llvm_block_has_terminator(backend->builder->GetInsertBlock()))
            {
                LWARNING() << "skip until next block, current block has terminator" << std::endl;
                ++i;
                do
                {
                    n = this->children[i];
                    if (n->is<Block>())
                    {
                        goto process_child;
                    }

                    LWARNING() << "skip [" << i << "] " << n->to_string() << std::endl;
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

    //
    // FunctionBlock
    //

    FunctionBlock::FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name) : Block(rule, name) {}
    std::string FunctionBlock::to_string()
    {
        return std::format("{}{}", "Function", Block::to_string());
    }

    void FunctionBlock::pre_codegen(logia::Backend *backend)
    {
        if (is_inserted)
        {
            return;
        }
        is_inserted = true;

        Block::pre_codegen(backend);
        auto function = this->first_parent<Function>();
        function->ir_func->insert(function->ir_func->end(), this->llvm_basicblock);
        backend->builder->SetInsertPoint(this->llvm_basicblock);

        this->parent_node->as<Function>()->codegen_parameters(backend);
    }

    llvm::Value *FunctionBlock::post_codegen(logia::Backend *backend)
    {
        backend->builder->SetInsertPoint(this->llvm_basicblock);

        Block::codegen_children(backend);

        this->cg_value = this->llvm_basicblock;
        return Node::post_codegen(backend);
    }

    LOGIA_API Block *ast_create_block(Identifier *name)
    {
        return new Block(nullptr, name);
    }
}