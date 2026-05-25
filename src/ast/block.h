#pragma once

#include "ast/node.h"
#include "ast/semantic_error.h"

namespace logia::AST
{
    struct Identifier;

    /// @brief A Scope is a Node that contains a symbol table, used for name resolution
    struct Scope : public Node
    {
    public:
        /// @brief Back pointer to fast access
        Scope *parentScope = nullptr;

        Scope(antlr4::ParserRuleContext *rule);

        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        /// @brief Defines the block scope, used for name resolution
        std::unordered_map<std::string_view, std::vector<Node *>> scope;

        /// @brief Register a name in the scope
        void scope_set(const char *name, Node *node);

        /// @brief Copy given identifiers into target
        void scope_copy(std::vector<Identifier *> list, Scope *target);

        /// @brief Copy everything into target
        void scope_copy_all(Scope *target);

        void post_attach() override;

        /// @brief A Scope do not have type, nullptr is returned
        /// @return nullptr
        Type *get_type() override;

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
                auto vec = it->second;
                *out = vec[0]->as<T>();
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
                auto vec = it->second;
                return vec[0]->as<T>();
            }

            throw_semantic_error(this, std::format("Identifier '{}' not found in current scope.", name));
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
            Scope *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    *out = it->second->as<T>();
                    return true;
                }
                p = p->parentScope;
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
            Scope *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    auto vec = it->second;
                    return vec[0]->as<T>();
                }
                p = p->parentScope;
            } while (p != nullptr);

            throw_semantic_error(this, std::format("Identifier '{}' of type '{}' Not found in scope.", name, typeid(T).name()));
        }

        /// @brief lookup a name into the scope chain throws if not found
        /// @example
        /// VarDeclStmt* var_decl = block->lookup<VarDeclStmt>("x");
        std::vector<Node *> lookup_all(const char *name)
        {
            DEBUG() << name << std::endl;

            auto out = std::vector<Node *>();

            std::string_view name_view(name);
            Scope *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    out.insert(out.end(), it->second.begin(), it->second.end());
                }
                p = p->parentScope;
            } while (p != nullptr);

            // TODO this unique enforcement may be caused by other bug!
            std::sort(out.begin(), out.end());
            out.erase(std::unique(out.begin(), out.end()), out.end());

            return out;
        }

    protected:
        /// @brief Do nothing
        void _set_type(Type *ty) override;
    };

    /// @brief A block
    struct Block : public Scope
    {
    public:
        /// @brief Defines the block name -> (goto identifier)
        /// @remarks This cannot be a children of the block because it's a mess, but should be one day.
        Identifier *name;

        /// @brief BasicBlock will be populated at pre_codegen and cached
        /// @remarks BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *llvm_basicblock = nullptr;

        Block(antlr4::ParserRuleContext *rule, Identifier *name);

        /// @brief Retrieves the block name, used for goto and debugging
        Identifier *get_identifier();

        /// @brief Shortcut to name as cstring
        const char *get_name();

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

    /// @brief A block that is directly attached to a function, used for function body
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