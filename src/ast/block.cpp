#include "ast/block.h"
#include "ast/traverse.h"
#include "ast/llvm.h"

namespace logia::AST
{

    Block::Block(antlr4::ParserRuleContext *rule) : Node(rule, ast_types::BODY)
    {
    }

    void Block::set(const char *name, Node *node)
    {
        // TODO check for valid ast_types
        this->scope[strdup(name)] = node;
    }

    Node *Block::lookup(const char *name)
    {
        DEBUG() << name << std::endl;

        std::string_view name_view(name);
        Block *p = this;
        Node *f;
        do
        {
            // f = p->scope[name_view];
            auto it = p->scope.find(name_view);
            if (it != p->scope.end())
            {
                return it->second;
            }
            p = p->parent;
        } while (p != nullptr);

        throw std::runtime_error(std::format("not found in scope: {}", name));
    }

    Type *Block::get_type()
    {
        return nullptr;
    }

    std::string Block::to_string()
    {
        char buffer[36];
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
        if (!this->is_attached)
        {
            this->is_attached = true;
            auto parentBody = (Block *)ast_find_closest_parent(this->parent_node, ast_types::BODY);
            LOGIA_ASSERT(parentBody);
            this->parent = parentBody;
        }
    }

    llvm::BasicBlock *Block::create_llvm_block(logia::Backend *codegen, const char *name)
    {
        if (this->llvm_basicblock)
        {
            return this->llvm_basicblock;
        }

        this->llvm_basicblock = llvm::BasicBlock::Create(codegen->context, name, nullptr);

        DEBUG() << this->to_string() << " name=" << name << " llvm_basicblock = " << this->llvm_basicblock << std::endl;

        return this->llvm_basicblock;
    }

    llvm::Value *Block::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        this->is_codegen = true;

        // not the main program -> override block!
        if (!this->is<Program>())
        {
            DEBUG() << "override SetInsertPoint llvm_basicblock = " << this->llvm_basicblock << std::endl;
            if (this->llvm_basicblock == nullptr)
            {
                // unhandled block inside a function block

                auto previous_block = builder->GetInsertBlock();
                DEBUG() << "previous_block = " << previous_block << std::endl;
                auto block = this->create_llvm_block(codegen, "");

                if (!ast_llvm_block_has_terminator(previous_block))
                {
                    builder->CreateBr(block); // goto
                }

                auto func = builder->GetInsertBlock()->getParent();
                func->insert(func->end(), block);
            }
            builder->SetInsertPoint(this->llvm_basicblock);
        }

        for (int i = 0; i < this->children.size(); i++)
        {
            Node *n = this->children[i];
            DEBUG() << "codegen.statement[" << i << "] " << n->to_string() << std::endl;
            auto inst = n->codegen(codegen, builder);
        }

        return this->llvm_basicblock;
    }

    LOGIA_API Block *ast_create_block()
    {
        return new Block(nullptr);
    }

}