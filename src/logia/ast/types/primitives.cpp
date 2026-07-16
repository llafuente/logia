#include "logia/ast/types/primitives.h"

#include <string>

namespace logia::AST
{
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim)
    {
        switch (prim)
        {
        case Primitives::VOID_TY:
            return _strdup("void");
        case Primitives::INTEGER_TY:
            return _strdup("int");
        case Primitives::FLOATING_POINT_TY:
            return _strdup("float");
        case Primitives::PTR_TY:
            return _strdup("ptr");
        case Primitives::STRUCT_TY:
            return _strdup("struct");
        case Primitives::FUNCTION_TY:
            return _strdup("function");
        default:
            throw std::exception("unreachable");
        }
    }
}