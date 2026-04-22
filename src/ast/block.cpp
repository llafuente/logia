#include "ast/block.h"
#include "ast/traverse.h"
#include "ast/llvm.h"

namespace logia::AST
{

    Block::Block(antlr4::ParserRuleContext *rule, Identifier *name) : Node(rule), name(name)
    {
    }

    Identifier *Block::get_identifier()
    {
        return this->name;
    }

    const char *Block::get_name()
    {
        return this->get_identifier()->identifier;
    }

    void Block::set(const char *name, Node *node)
    {
        if (!node->is<Type>() && !node->is<Block>() && !node->is<VarDeclStmt>() && !node->is<FunctionParameter>())
        {
            throw std::runtime_error(std::format("invalid node type: {} - {}", typeid(node).name(), node->to_string()));
        }
        this->scope[strdup(name)] = node;
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

        return std::format("Block[{} statements] scope[{}] ({:p})", this->children.size(), list, static_cast<void *>(this));
    }

    void Block::post_attach()
    {
        auto parentBody = this->first_parent<Block>();
        LOGIA_ASSERT(parentBody);
        this->parent = parentBody;

        if (!this->is_attached)
        {
            this->is_attached = true;

            if (!is<FunctionBlock>())
            {

                auto fblock = this->first_parent<FunctionBlock>();
                LOGIA_ASSERT(fblock);

                fblock->set(this->get_name(), this);
            }
        }
    }

    void Block::pre_codegen(logia::Backend *codegen)
    {
        if (this->llvm_basicblock != nullptr)
        {
            return;
        }

        this->llvm_basicblock = llvm::BasicBlock::Create(codegen->context, this->get_name(), nullptr);

        DEBUG() << this->to_string() << " name=" << this->get_name() << " llvm_basicblock = " << this->llvm_basicblock << std::endl;
    }

    llvm::Value *Block::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        this->pre_codegen(codegen);

        DEBUG() << this->to_string() << std::endl;
        this->is_codegen = true;

        // unhandled block inside a function block
        auto previous_block = builder->GetInsertBlock();
        DEBUG() << "previous_block = " << previous_block << std::endl;

        if (!ast_llvm_block_has_terminator(previous_block))
        {
            builder->CreateBr(this->llvm_basicblock); // goto
        }
        // insert block into current function
        auto func = builder->GetInsertBlock()->getParent();
        func->insert(func->end(), this->llvm_basicblock);
        builder->SetInsertPoint(this->llvm_basicblock);

        this->codegen_children(codegen, builder);

        return this->llvm_basicblock;
    }
    void Block::codegen_children(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        int max = this->children.size();
        int last = max - 1;
        for (size_t i = 0; i < max; ++i)
        {
        process_child:
            Node *n = this->children[i];
            DEBUG() << "codegen.statement[" << i << "] " << n->to_string() << std::endl;
            auto inst = n->codegen(codegen, builder);

            // if the current block has a terminator and we continue we run into multiple problems like:
            // fix: All predecessors must be dead!
            // lli could execute the program by jit fail
            // we should skip until next block
            // This is an ill formated program and could lead to problems/cashes later
            // we need a way to detect this "dead code" and raise a semantic_errror -> the coder to remove it!
            if (this->llvm_basicblock && i != last && ast_llvm_block_has_terminator(builder->GetInsertBlock()))
            {
                WARNING() << "skip until next block, current block has terminator" << std::endl;
                ++i;
                do
                {
                    n = this->children[i];
                    if (n->is<Block>())
                    {
                        goto process_child;
                    }

                    WARNING() << "skip [" << i << "] " << n->to_string() << std::endl;
                    ++i;
                } while (i < max);
                --i;
            }
        }
    }

    FunctionBlock::FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name) : Block(rule, name) {}
    std::string FunctionBlock::to_string()
    {
        return std::format("{}{}", "Function", Block::to_string());
    }

    llvm::Value *FunctionBlock::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        this->pre_codegen(codegen);
        this->is_codegen = true;

        auto function = this->first_parent<Function>();
        function->cg_value->insert(function->cg_value->end(), this->llvm_basicblock);
        builder->SetInsertPoint(this->llvm_basicblock);

        this->parent_node->as<Function>()->codegen_parameters(codegen, builder);

        Block::codegen_children(codegen, builder);
        return this->llvm_basicblock;
    }

    LOGIA_API Block *ast_create_block(Identifier *name)
    {

        return new Block(nullptr, name);
    }
}