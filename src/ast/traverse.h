#pragma once

#include "ast/node.h"
#include "ast/stmt.h"

// TODO when more "file" test are finished, remove test_ast and this file!

namespace logia::AST
{
    //
    // ast-traverese/search
    //
    /**
     * callback type for traverse function
     */
    /// @deprecated
    typedef bool (*ast_traverse_callback_t)(Node *);
    /**
     * traverse subnodes while cb is true
     */
    /// @deprecated
    void ast_traverse(Node *current, ast_traverse_callback_t cb);
    /**
     * reverse the tree searching for given name that match with a Type
     */
    /// @deprecated
    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name);
}