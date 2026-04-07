#pragma once
#include "utils.h"

namespace logia::AST
{

    enum ast_types : uint32_t
    {
        // expression flag
        EXPRESSION = (1 << 30),
        // Is a constant
        CONST = (1 << 29),
        // statement flag
        STMT = (1 << 28),
        // type flag
        TYPE = (1 << 27),
        // body flag
        BODY = (1 << 26),

        // all flag bits, to remove them if necessary
        ALL_FLAGS = EXPRESSION | STMT | TYPE | BODY | CONST,

        // types
        PROGRAM = (1 << 1),
        FUNCTION = (1 << 2),
        BLOCK = (1 << 3),
        CALL_EXPRESSION = (1 << 4),
        STRING_LITERAL = (1 << 5),
        FLOAT_LITERAL = (1 << 6),
        INTEGER_LITERAL = (1 << 7),
        IDENTIFIER = (1 << 8),

        RETURN_STMT = (1 << 9),
        VAR_DECL_STMT = (1 << 10),
        IF_STMT = (1 << 11),
        GOTO_STMT = (1 << 12),
        // NOTE: if modified -> ast_types_to_string
    };

    /**
     * Converts ast_types enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_types_to_string(ast_types type);
}