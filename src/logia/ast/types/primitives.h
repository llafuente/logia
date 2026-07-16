#pragma once

#include "logia/logia.h"

namespace logia::AST
{
    enum class Primitives
    {
        NONE,

        VOID_TY,

        INTEGER_TY,

        FLOATING_POINT_TY,

        // aliases but very special...
        // Int,
        // size,
        // ptrdiff,
        // address,
        // Typeid,

        PTR_TY,
        // Enum,
        STRUCT_TY,
        // interface,
        FUNCTION_TY,
        // this is a pointer to function, but we may need to declare at this level
        // callable,
        ANY_TY,
    };

    /**
     * Converts Primitives enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim);

}