#include "logia/ast/operators.h"
#include "logia/compiler_error.h"

namespace logia::AST
{
    struct FunctionToOperator
    {
        const char fn_name[36];
        uint32_t op_value;
        const char op_str[4];
        uint32_t args;
        const char enforce_return_type[4];
    };
    uint32_t f2op_length = 29 + 7 + 2;
    FunctionToOperator f2op[29 + 7 + 2] = {
        {"__logia_bin_assign", (uint32_t)Operators::BINARY_ASSIGN, "=", 2, ""},
        {"__logia_bin_add_assign", (uint32_t)Operators::BINARY_ADD_ASSIGN, "+=", 2, ""},
        {"__logia_bin_sub_assign", (uint32_t)Operators::BINARY_SUB_ASSIGN, "-=", 2, ""},
        {"__logia_bin_mul_assign", (uint32_t)Operators::BINARY_MUL_ASSIGN, "*=", 2, ""},
        {"__logia_bin_div_assign", (uint32_t)Operators::BINARY_DIV_ASSIGN, "/=", 2, ""},
        {"__logia_bin_mod_assign", (uint32_t)Operators::BINARY_MOD_ASSIGN, "%=", 2, ""},
        {"__logia_bin_bitwise_and_assign", (uint32_t)Operators::BINARY_BITWISE_AND_ASSIGN, "&=", 2, ""},
        {"__logia_bin_bitwise_or_assign", (uint32_t)Operators::BINARY_BITWISE_OR_ASSIGN, "|=", 2, ""},
        {"__logia_bin_bitwise_xor_assign", (uint32_t)Operators::BINARY_BITWISE_XOR_ASSIGN, "^=", 2, ""},
        {"__logia_bin_bitwise_shr_assign", (uint32_t)Operators::BINARY_BITWISE_RIGHT_SHIFT_ASSIGN, ">>=", 2, ""},
        {"__logia_bin_bitwise_shl_assign", (uint32_t)Operators::BINARY_BITWISE_LEFT_SHIFT_ASSIGN, "<<=", 2, ""},

        {"__logia_bin_add", (uint32_t)Operators::BINARY_ADD, "+", 2, ""},
        {"__logia_bin_sub", (uint32_t)Operators::BINARY_SUB, "-", 2, ""},
        {"__logia_bin_mul", (uint32_t)Operators::BINARY_MUL, "*", 2, ""},
        {"__logia_bin_div", (uint32_t)Operators::BINARY_DIV, "/", 2, ""},
        {"__logia_bin_mod", (uint32_t)Operators::BINARY_MOD, "%", 2, ""},

        {"__logia_bin_cmp_eq", (uint32_t)Operators::BINARY_COMPARISON_EQ, "==", 2, "i1"},
        {"__logia_bin_cmp_neq", (uint32_t)Operators::BINARY_COMPARISON_NEQ, "!=", 2, "i1"},
        {"__logia_bin_cmp_lt", (uint32_t)Operators::BINARY_COMPARISON_LT, "<", 2, "i1"},
        {"__logia_bin_cmp_gt", (uint32_t)Operators::BINARY_COMPARISON_GT, ">", 2, "i1"},
        {"__logia_bin_cmp_lte", (uint32_t)Operators::BINARY_COMPARISON_LTE, "<=", 2, "i1"},
        {"__logia_bin_cmp_gte", (uint32_t)Operators::BINARY_COMPARISON_GTE, ">=", 2, "i1"},

        {"__logia_bin_logical_and", (uint32_t)Operators::BINARY_LOGICAL_AND, "&&", 2, "i1"},
        {"__logia_bin_logical_or", (uint32_t)Operators::BINARY_LOGICAL_OR, "||", 2, "i1"},

        {"__logia_bin_bitwise_and", (uint32_t)Operators::BINARY_BITWISE_AND, "&", 2, ""},
        {"__logia_bin_bitwise_or", (uint32_t)Operators::BINARY_BITWISE_OR, "|", 2, ""},
        {"__logia_bin_bitwise_xor", (uint32_t)Operators::BINARY_BITWISE_XOR, "^", 2, ""},
        {"__logia_bin_bitwise_shl", (uint32_t)Operators::BINARY_BITWISE_LEFT_SHIFT, "<<", 2, ""},
        {"__logia_bin_bitwise_shr", (uint32_t)Operators::BINARY_BITWISE_RIGHT_SHIFT, ">>", 2, ""},

        {"__logia_ref", (uint32_t)Operators::PREFIX_REFERENCE, "&", 1, ""},
        {"__logia_deref", (uint32_t)Operators::PREFIX_DEREFERENCE, "", 1, ""},
        {"__logia_prefix_inc", (uint32_t)Operators::PREFIX_INCREMENT, "++", 1, ""},
        {"__logia_prefix_dec", (uint32_t)Operators::PREFIX_DECREMENT, "--", 1, ""},
        {"__logia_prefix_neg", (uint32_t)Operators::PREFIX_NEGATION, "-", 1, ""},
        {"__logia_prefix_bitwise_not", (uint32_t)Operators::PREFIX_BITWISE_NOT, "~", 1, ""},
        {"__logia_prefix_logical_not", (uint32_t)Operators::PREFIX_LOGICAL_NOT, "!", 1, ""},

        {"__logia_postfix_inc", (uint32_t)Operators::POSTFIX_INCREMENT, "++", 1, ""},
        {"__logia_postfix_dec", (uint32_t)Operators::POSTFIX_DECREMENT, "--", 1, ""},
    };

    LOGIA_API const char *ast_operator_to_function_name(Operators op)
    {
        for (int i = 0; i < f2op_length; ++i)
        {
            if ((Operators)f2op[i].op_value == op)
            {
                return f2op[i].fn_name;
            }
        }

        throw_compiler_error("unkown operator");
    }

    bool is_assignament_operator(Operators op)
    {
        switch (op)
        {
        case Operators::BINARY_BITWISE_LEFT_SHIFT_ASSIGN:
        case Operators::BINARY_BITWISE_RIGHT_SHIFT_ASSIGN:
        case Operators::BINARY_BITWISE_AND_ASSIGN:
        case Operators::BINARY_BITWISE_XOR_ASSIGN:
        case Operators::BINARY_BITWISE_OR_ASSIGN:
        case Operators::BINARY_MOD_ASSIGN:
        case Operators::BINARY_ASSIGN:
        case Operators::BINARY_ADD_ASSIGN:
        case Operators::BINARY_SUB_ASSIGN:
        case Operators::BINARY_MUL_ASSIGN:
        case Operators::BINARY_DIV_ASSIGN:
            return true;
        }
        return false;
    }

    bool is_bitwise_operator(Operators op)
    {
        switch (op)
        {
        case Operators::BINARY_BITWISE_AND_ASSIGN:
        case Operators::BINARY_BITWISE_OR_ASSIGN:
        case Operators::BINARY_BITWISE_XOR_ASSIGN:
        case Operators::BINARY_BITWISE_RIGHT_SHIFT_ASSIGN:
        case Operators::BINARY_BITWISE_LEFT_SHIFT_ASSIGN:
        case Operators::BINARY_BITWISE_AND:
        case Operators::BINARY_BITWISE_OR:
        case Operators::BINARY_BITWISE_XOR:
        case Operators::BINARY_BITWISE_LEFT_SHIFT:
        case Operators::BINARY_BITWISE_RIGHT_SHIFT:
            return true;
        }
        return false;
    }

    bool is_comparison_operator(Operators op)
    {
        switch (op)
        {

        case Operators::BINARY_COMPARISON_EQ:
        case Operators::BINARY_COMPARISON_NEQ:
        case Operators::BINARY_COMPARISON_LT:
        case Operators::BINARY_COMPARISON_GT:
        case Operators::BINARY_COMPARISON_LTE:
        case Operators::BINARY_COMPARISON_GTE:
            return true;
        }
        return false;
    }

    bool is_logical_operator(Operators op)
    {
        switch (op)
        {
        case Operators::BINARY_LOGICAL_AND:
        case Operators::BINARY_LOGICAL_OR:
        case Operators::PREFIX_LOGICAL_NOT:
            return true;
        }
        return false;
    }
}
