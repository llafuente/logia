#include "ast/traverse.h"
#include "ast/block.h"

namespace logia::AST
{

    void ast_traverse(Node *current, ast_traverse_callback_t cb)
    {
        // stop on false
        if (!cb(current))
        {
            return;
        }

        for (int i = 0; i < current->children.size(); ++i)
        {
            ast_traverse(current->children[i], cb);
        }
    }

    LOGIA_API Node *ast_find_closest_parent(Node *current, ast_types mask_type)
    {
        LOGIA_ASSERT(current);
        // REVIEW Start at the current node? or 1 level up?
        // LOGIA_ASSERT(current->parentNode);
        // current = current->parentNode;
        do
        {
            if (((int)current->type & (int)mask_type) != 0)
            {
                return current;
            }
            current = current->parent_node;
        } while (current != nullptr);

        return nullptr;
    }

    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name)
    {
        auto body = (Block *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(body); // this shall exists!
        // TODO cast if possible or nullptr!
        return (Type *)body->lookup(name);
    }

    LOGIA_API VarDeclStmt *ast_get_vardecl_by_name(Node *current, char *name)
    {
        auto body = (Block *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(body); // this shall exists!
        do
        {
            // TODO cast if possible or nullptr!
            Node *n = body->lookup(name);
            if (n != nullptr)
            {
                LOGIA_ASSERT((n->type & ast_types::VAR_DECL_STMT) != 0);
                return (VarDeclStmt *)n;
            }
        } while ((body = body->parent) != nullptr);
        // here we expect to be at program!
        LOGIA_ASSERT((body->type & ast_types::BODY) != 0);
        LOGIA_ASSERT((body->type & ast_types::PROGRAM) != 0);

        return nullptr;
    }

}