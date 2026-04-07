#include "ast/types.h"

namespace logia::AST
{

    LOGIA_API LOGIA_LEND char *ast_types_to_string(ast_types type)
    {
        const char *a;
        const char *b;

        if ((ast_types::EXPRESSION & type) != 0)
        {
            a = "EXPRESSION.";
        }
        if ((ast_types::STMT & type) != 0)
        {
            a = "STMT.";
        }
        if ((ast_types::TYPE & type) != 0)
        {
            a = "TYPE.";
        }
        if ((ast_types::BODY & type) != 0)
        {
            a = "BODY.";
        }

        // remove all flags
        type = (ast_types)(type & ~ast_types::ALL_FLAGS);

        switch (type)
        {
        case ast_types::PROGRAM:
            b = "PROGRAM";
            break;
        case ast_types::FUNCTION:
            b = "FUNCTION";
            break;
        case ast_types::CALL_EXPRESSION:
            b = "CALL_EXPRESSION";
            break;
        case ast_types::STRING_LITERAL:
            b = "STRING_LITERAL";
            break;
        case ast_types::FLOAT_LITERAL:
            b = "FLOAT_LITERAL";
            break;
        case ast_types::INTEGER_LITERAL:
            b = "INTEGER_LITERAL";
            break;
        case ast_types::RETURN_STMT:
            b = "RETURN_STMT";
            break;
        }
        char *dst = (char *)malloc(strlen(a) + strlen(b) + 1);
        strcpy(dst, a);
        strcpy(dst, b);

        return dst;
    }
}