#include "logia/ast/scope.h"

#include "utils.h"

#include "logia/ast/block.h"
#include "logia/ast/type.h"
#include "logia/ast/stmt.h"
#include "logia/ast/identifier.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/function.h"
#include "logia/ast/llvm.h"
#include "logia/ast/semantic_error.h"
#include "logia/ast/functionblock.h"
#include "logia/ast/program.h"

namespace logia::AST
{
    //
    // Scope
    //
    Scope::Scope(location loc) : Node(loc) { this->has_type = false; }

    std::string Scope::to_string()
    {
        std::string list;
        for (const auto &pair : this->scope)
        {
            list += list.empty() ? "" : ", ";
            list += pair.first;
        }
        return std::format("scope[{}]{}", list, Node::to_string());
    }

    void Scope::scope_set(const char *name, Node *node)
    {
        if (!node->is<Type>() && !node->is<Block>() && !node->is<VarDeclStmt>() && !node->is<FunctionParameter>())
        {
            throw std::runtime_error(std::format("invalid node type: {} - {}", typeid(node).name(), node->to_string()));
        }
        if (!this->scope.contains(name))
        {
            this->scope[_strdup(name)] = {node};
        }
        else
        {
            this->scope[name].push_back(node);
        }
    }

    void Scope::scope_copy(std::vector<Identifier *> list, Scope *target)
    {
        for (const auto &it : list)
        {
            const char *name = it->identifier;
            // TODO handle don't exist -> compiler_error
            auto vec = this->scope[name];

            for (const auto &it2 : vec)
            {
                target->scope_set(name, it2);
            }
        }
    }
    void Scope::scope_copy_all(Scope *target)
    {
        for (const auto &it : scope)
        {
            auto vec = it.second;
            for (const auto &it2 : vec)
            {
                target->scope_set(it.first.data(), it2);
            }
        }
    }

    void Scope::post_attach()
    {
        // keep parent body in sync regardless being already attached, allow blocks to be moved.
        auto parentBody = this->first_parent<Scope>();
        LOGIA_VERIFY(parentBody != nullptr);
        this->parentScope = parentBody;
        Node::post_attach();
    }

    Type *Scope::get_type()
    {
        return nullptr;
    }

    void Scope::_set_type(Type *ty) {}

    LOGIA_API Scope *scope_closest(Node *node)
    {
        Scope *scope;
        if (!node->try_cast<Scope>(&scope))
        {
            return node->first_parent<Scope>();
        }
        return scope;
    }

    LOGIA_API scope_search_result scope_lookup_all(Node *node, const char *name)
    {
        LOG(DBG, "({}) from = {}", name, node->to_string());

        if (!node->is_attached)
        {
            LOG_ERR("{}", node->to_string());
            return logia::utils::make_error<std::vector<Node *>, bool>("Cannot search from a detached node", false);
        }

        auto out = std::vector<Node *>();

        std::string_view name_view(name);
        Scope *p = scope_closest(node);
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

        return logia::utils::make_success<std::vector<Node *>, bool>(out);
    }

    LOGIA_API Node *scope_lookup_first(Node *node, const char *name)
    {
        LOG(DBG, "({}) from = {}", name, node->to_string());

        if (!node->is_attached)
        {
            LOG_ERR("{}", node->to_string());
            return nullptr;
        }

        auto out = std::vector<Node *>();

        std::string_view name_view(name);
        Scope *p = scope_closest(node);
        do
        {
            auto it = p->scope.find(name_view);
            if (it != p->scope.end())
            {
                return it->second.begin()[0];
            }
            p = p->parentScope;
        } while (p != nullptr);

        return nullptr;
    }

    LOGIA_API Node *scope_lookup_one(Node *node, const char *name)
    {
        auto result = scope_lookup_all(node, name);
        if (result.is_error())
        {
            return nullptr;
        }
        auto list = result.unwrap_success();
        if (list.size())
        {
            return list[0];
        }
        return nullptr;
    }

    bool scope_set(Node *node, const char *name, Node *what, bool unique)
    {
        if (!what->is<Type>() && !what->is<Block>() && !what->is<VarDeclStmt>() && !what->is<FunctionParameter>())
        {
            throw_compiler_error(std::format("invalid node type: {} expected Type, Block, VarDeclStmt or FunctionParameter \n{}", typeid(node).name(), node->to_string()));
        }

        Scope *p = scope_closest(node);
        bool contains = p->scope.contains(name);
        if (contains)
        {
            if (unique)
            {
                return false;
            }

            p->scope[name].push_back(node);
            return true;
        }
        p->scope[_strdup(name)] = {node};
        return true;
    }

    bool function_set(Node *node, const char *name, Node *what, bool unique)
    {
        if (!what->is<Type>() && !what->is<Block>() && !what->is<VarDeclStmt>() && !what->is<FunctionParameter>())
        {
            throw_compiler_error(std::format("invalid node type: {} expected Type, Block, VarDeclStmt or FunctionParameter \n{}", typeid(node).name(), node->to_string()));
        }

        FunctionBlock *fblock;
        Scope *p = scope_closest(node);
        while (!p->try_cast<FunctionBlock>(&fblock))
        {
            p = scope_closest(p->parent_node); // use parent because scope_closest can return "p"
            if (p->is<Program>())
            {
                throw_compiler_error(std::format("Could not find a function block for: {}", node->to_string()));
            }
        }
        bool contains = p->scope.contains(name);
        if (contains)
        {
            if (unique)
            {
                return false;
            }

            p->scope[name].push_back(node);
            return true;
        }

        p->scope[_strdup(name)] = {node};
        return true;
    }
}