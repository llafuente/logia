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

namespace logia::AST
{
    //
    // Scope
    //
    Scope::Scope(antlr4::ParserRuleContext *rule) : Node(rule) { this->has_type = false; }

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
        LOGIA_ASSERT(parentBody == nullptr);
        this->parentScope = parentBody;
        Node::post_attach();
    }

    Type *Scope::get_type()
    {
        return nullptr;
    }

    void Scope::_set_type(Type *ty) {}
}