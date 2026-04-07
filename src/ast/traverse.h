#pragma once

#include "ast/node.h"
#include "ast/stmt.h"

namespace logia::AST
{
    //
    // ast-traverese/search
    //
    /**
     * callback type for traverse function
     */
    typedef bool (*ast_traverse_callback_t)(Node *);
    /**
     * traverse subnodes while cb is true
     */
    void ast_traverse(Node *current, ast_traverse_callback_t cb);
    /**
     * reverse the tree a returns the first node that match the given mask
     */
    LOGIA_API Node *ast_find_closest_parent(Node *current, ast_types mask_type);
    /**
     * reverse the tree searching for given name that match with a Type
     */
    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name);
    /**
     * reverse the tree searching for given name that match with a vardecl
     */
    LOGIA_API VarDeclStmt *ast_get_vardecl_by_name(Node *current, char *name);

}