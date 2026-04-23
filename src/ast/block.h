#pragma once

#include "ast/node.h"

namespace logia::AST
{
    struct Identifier;

    /// @brief A block of code aka scope
    struct Block : public Node
    {
    public:
        /// @brief Defines the block name -> (goto identifier)
        /// @remarks This cannot be a children of the block because it's a mess, but should be one day.
        Identifier *name;

        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        /// @brief Defines the block scope, used for name resolution
        std::unordered_map<std::string_view, Node *> scope;
        /// @brief Back pointer to fast access
        Block *parent = nullptr;
        /// @brief BasicBlock will be populated at pre_codegen and cached
        /// @remarks BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *llvm_basicblock = nullptr;

        Block(antlr4::ParserRuleContext *rule, Identifier *name);

        /// @brief Retrieves the block name, used for goto and debugging
        Identifier *get_identifier();

        /// @brief Shortcut to name as cstring
        const char *get_name();

        /// @brief Register a name in the scope
        void set(const char *name, Node *node);

        /// @brief lookup a name into the current scope gracefully
        /// @example
        /// VarDeclStmt* var_decl = nullptr;
        /// if (block->try_look<VarDeclStmt>("x", &var_decl)) {
        ///     // var_decl is now set
        /// }
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

        /// @brief lookup a name in the current scope throws if not found
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

        /// @brief lookup a name into the scope chain gracefully
        /// @example
        /// VarDeclStmt* var_decl = nullptr;
        /// if (block->try_lookup<VarDeclStmt>("x", &var_decl)) {
        ///     // var_decl is now set
        /// }
        template <class T>
        bool try_lookup(const char *name, T **out)
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
        /// @brief lookup a name into the scope chain throws if not found
        /// @example
        /// VarDeclStmt* var_decl = block->lookup<VarDeclStmt>("x");
        template <class T>
        T *lookup(const char *name)
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

        void pre_codegen(logia::Backend *codegen) override;
        /// @brief Inserts block into parent Function, add a jump if needed and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;

        /// @brief Codegen all children into current block
        /// @param backend
        void codegen_children(logia::Backend *backend);

        /// @brief Notice that block do not have type, nullptr is returned
        Type *get_type() override;

        /// @brief Register block name at function scope
        void post_attach() override;
    };

    struct FunctionBlock : public Block
    {
        /// @brief Marks if the block is already inserted into the function
        unsigned char is_inserted : 1 = false;

        FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name);

        std::string to_string() override;

        /// @brief Inserts block into parent Function
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

    // Program is in fact a Block should be here, but it's important as it's the root, give him his own file :D

    /// @brief Creates a block scope
    /// @deprecated
    LOGIA_API LOGIA_LEND Block *ast_create_block(Identifier *name);

}