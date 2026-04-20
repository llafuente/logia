#pragma once

#include "ast/node.h"

namespace logia::AST
{
    struct Identifier;

    // TODO
    // BaseBlock(scope)
    // BaseBlock(scope) -> Block(stmt) -> FunctionBlock
    // BaseBlock -> Program(stmt)

    struct Block : public Node
    {
    public:
        /// @brief Defines the block name -> (goto identifier)
        /// @remarks This cannot be a children of the block because it's a mess, but should be one day.
        Identifier *name;

        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        std::unordered_map<std::string_view, Node *> scope;
        // back pointer to fast access
        Block *parent = nullptr;
        // BasicBlock will be populated at create_llvm_block and cached
        // NOTE BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *llvm_basicblock = nullptr;
        // TODO remove body as we can reverse the tree and search it!
        Block(antlr4::ParserRuleContext *rule, Identifier *name);

        Identifier *get_identifier();
        const char *get_name();

        /**
         * Register a name in the scope
         */
        void set(const char *name, Node *node);

        template <class T>
        bool try_look(const char *name, T **out)
        {
            DEBUG() << name << std::endl;

            std::string_view name_view(name);
            auto it = this->scope.find(name_view);
            if (it != this->scope.end())
            {
                *out = it->second->as<T>();
                return true;
            }

            return false;
        }

        template <class T>
        T *look(const char *name)
        {
            DEBUG() << name << std::endl;

            std::string_view name_view(name);
            auto it = this->scope.find(name_view);
            if (it != this->scope.end())
            {
                return it->second->as<T>();
            }

            throw std::runtime_error(std::format("not found in scope: {}", name));
        }

        template <class T>
        bool try_lookup2(const char *name, T **out)
        {
            DEBUG() << name << std::endl;

            std::string_view name_view(name);
            Block *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    *out = it->second->as<T>();
                    return true;
                }
                p = p->parent;
            } while (p != nullptr);

            return false;
        }

        template <class T>
        T *lookup2(const char *name)
        {
            DEBUG() << name << std::endl;

            std::string_view name_view(name);
            Block *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    return it->second->as<T>();
                }
                p = p->parent;
            } while (p != nullptr);

            throw std::runtime_error(std::format("not found in scope: {} of type {}", name, typeid(T).name()));
        }

        std::string to_string() override;

        /// @brief Inserts block into parent Function, add a jump if needed and codegen children
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void codegen_children(logia::Backend *codegen, llvm::IRBuilder<> *builder);
        Type *get_type() override;
        void post_attach() override;
        void pre_codegen(logia::Backend *codegen);
    };

    struct FunctionBlock : public Block
    {
        FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name);
        std::string to_string() override;
        /// @brief Inserts block into parent Function and codegen children
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    /**
     * Creates a body (function body/block scope)
     */
    LOGIA_API LOGIA_LEND Block *ast_create_block(Identifier *name);

}