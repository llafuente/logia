#pragma once

#include "logia/log.h"
#include "logia/ast/node.h"
#include "logia/ast/semantic_error.h"
#include "logia/maybe_error.h"
#include <algorithm>

namespace logia::AST
{
    struct Identifier;

    /// @brief A Scope is a Node that contains a symbol table, used for name resolution
    struct Scope : public Node
    {
    public:
        /// @brief Back pointer to fast access
        Scope *parentScope = nullptr;

        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        /// @brief Defines the block scope, used for name resolution
        std::unordered_map<std::string_view, std::vector<Node *>> scope;

        Scope(location loc);

        std::string to_string() override;

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

        /// @brief lookup a name into the scope chain gracefully
        /// @example
        /// VarDeclStmt* var_decl = nullptr;
        /// if (block->try_lookup<VarDeclStmt>("x", &var_decl)) {
        ///     // var_decl is now set
        /// }
        template <class T>
        bool try_lookup(const char *name, T **out)
        {
            LOG(DBG, "{}", name);

            std::string_view name_view(name);
            Scope *p = this;
            do
            {
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    std::vector<logia::AST::Node *> list = it->second;
                    *out = list[0]->as<T>();
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
            LOG(DBG, "{}", name);

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

            // Log all scopes to be able to see if something is missing!
            p = this;
            do
            {
                LOG_ERR("Scope {}", p->to_string());
                p = p->parentScope;
            } while (p != nullptr);

            throw_semantic_error(this, std::format("Identifier '{}' of type '{}' Not found in scope.", name, typeid(T).name()));
        }

        /// @brief lookup a name into the scope chain throws if not found
        /// @example
        /// VarDeclStmt* var_decl = block->lookup<VarDeclStmt>("x");
        std::vector<Node *> lookup_all(const char *name)
        {
            LOG(DBG, "{}", name);

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

    LOGIA_API Scope *scope_closest(Node *node);

    /// @brief lookup a name in the current scope
    /// @throws if not found
    /// @throws if found more than one
    template <class T>
    T *scope_look_one(Node *node, const char *name)
    {
        LOG(DBG, "{}", name);

        if (!node->is_attached)
        {
            LOG_ERR("{}", node->to_string());
            throw_compiler_error("Cannot search from a detached node");
        }

        Scope *scope = scope_closest(node);

        std::string_view name_view(name);
        auto it = scope->scope.find(name_view);
        if (it != scope->scope.end())
        {
            auto vec = it->second;
            return vec[0]->as<T>();
        }

        throw_semantic_error(scope, std::format("Identifier '{}' not found in current scope.", name));
    }

    /// @brief lookup a name into the current scope gracefully
    /// @example
    /// VarDeclStmt* var_decl = nullptr;
    /// if (scope_try_look_one<VarDeclStmt>(anynode, "x", &var_decl)) {
    ///     // var_decl is now set
    /// }
    template <class T>
    bool scope_try_look_one(Node *node, const char *name, T **out)
    {
        LOG(DBG, "{}", name);
        if (!node->is_attached)
        {
            LOG_ERR("{}", node->to_string());
            throw_compiler_error("Cannot search from a detached node");
        }

        Scope *scope = scope_closest(node);

        std::string_view name_view(name);
        auto it = scope->scope.find(name_view);
        if (it != scope->scope.end())
        {
            auto vec = it->second;
            *out = vec[0]->as<T>();
            return true;
        }

        return false;
    }

    typedef logia::utils::maybe_error<std::vector<Node *>, bool> scope_search_result;

    // constexpr auto make_error = logia::utils::make_error<std::vector<Node *>, void>;
    // constexpr auto make_success = logia::utils::make_success<std::vector<Node *>, void>;
    // constexpr auto make_chained_error = logia::utils::make_chained_error<std::vector<Node *>, void>;

    /// @brief lookup a name into the scope chain
    /// @throws Cannot search from a detached node
    LOGIA_API scope_search_result scope_lookup_all(Node *node, const char *name);
    /// @brief lookup a name into the scope chain returning the first found
    LOGIA_API Node *scope_lookup_first(Node *node, const char *name);

    /// @brief lookup a name into the scope chain
    /// @throws Cannot search from a detached node
    LOGIA_API Node *scope_lookup_one(Node *node, const char *name);

    /// @brief Register 'what' in the closest scope to 'node' with given 'name'
    /// @returns if could be set (with/out uniqueness)
    bool scope_set(Node *node, const char *name, Node *what, bool unique);
    /// @brief Register 'what' in the closest function scope to 'node' with given 'name'
    /// @returns if could be set (with/out uniqueness)
    bool function_set(Node *node, const char *name, Node *what, bool unique);

}