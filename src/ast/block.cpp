#include "ast/block.h"
#include "ast/traverse.h"

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

        return nullptr;
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

    void Block::on_after_attach()
    {
        if (!this->is_attached)
        {
            this->is_attached = true;
            auto parentBody = (Block *)ast_find_closest_parent(this->parent_node, ast_types::BODY);
            LOGIA_ASSERT(parentBody);
            this->parent = parentBody;
        }
    }

    llvm::BasicBlock *Block::create_llvm_block(logia::Backend *codegen, char *name)
    {
        if (this->llvm_basicblock)
        {
            return this->llvm_basicblock;
        }

        DEBUG() << this->to_string() << std::endl;

        this->llvm_basicblock = llvm::BasicBlock::Create(codegen->context, name, nullptr);
        return this->llvm_basicblock;
    }

    llvm::Value *Block::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        this->is_codegen = true;

        // not the main program -> override block!
        if ((type & ast_types::PROGRAM) == 0)
        {
            DEBUG() << "override SetInsertPoint" << std::endl;
            // builder = new llvm::IRBuilder<>(codegen->context);
            builder->SetInsertPoint(this->create_llvm_block(codegen, nullptr));
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