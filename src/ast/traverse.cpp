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

        for (size_t i = 0; i < current->children.size(); ++i)
        {
            ast_traverse(current->children[i], cb);
        }
    }

    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name)
    {
        // XXX
        return nullptr;
    }
}