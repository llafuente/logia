#include "logia/ast/operators.h"
#include "logia/compiler_error.h"

namespace logia::AST
{
    struct FunctionToOperator
    {
        const char fn_name[36];
        uint32_t op_value;
    };
    uint32_t f2op_length = 23 + 6 + 2;
    FunctionToOperator f2op[23 + 6 + 2] = {
        {"__logia_bin_assign", (uint32_t)Operators::BINARY_ASSIGN},
        {"__logia_bin_add_assign", (uint32_t)Operators::BINARY_ADD_ASSIGN},
        {"__logia_bin_sub_assign", (uint32_t)Operators::BINARY_SUB_ASSIGN},
        {"__logia_bin_div_assign", (uint32_t)Operators::BINARY_DIV_ASSIGN},
        {"__logia_bin_mul_assign", (uint32_t)Operators::BINARY_MUL_ASSIGN},
        {"__logia_bin_add", (uint32_t)Operators::BINARY_ADD},
        {"__logia_bin_sub", (uint32_t)Operators::BINARY_SUB},
        {"__logia_bin_mul", (uint32_t)Operators::BINARY_MUL},
        {"__logia_bin_div", (uint32_t)Operators::BINARY_DIV},
        {"__logia_bin_mod", (uint32_t)Operators::BINARY_MOD},
        {"__logia_bin_eq", (uint32_t)Operators::BINARY_LOGIAL_EQ},
        {"__logia_bin_neq", (uint32_t)Operators::BINARY_LOGIAL_NEQ},
        {"__logia_bin_lt", (uint32_t)Operators::BINARY_LOGIAL_LT},
        {"__logia_bin_gt", (uint32_t)Operators::BINARY_LOGIAL_GT},
        {"__logia_bin_lte", (uint32_t)Operators::BINARY_LOGIAL_LTE},
        {"__logia_bin_gte", (uint32_t)Operators::BINARY_LOGIAL_GTE},
        {"__logia_bin_logical_and", (uint32_t)Operators::BINARY_LOGICAL_AND},
        {"__logia_bin_logical_or", (uint32_t)Operators::BINARY_LOGICAL_OR},
        {"__logia_bin_bitwise_or", (uint32_t)Operators::BINARY_BITWISE_OR},
        {"__logia_bin_bitwise_and", (uint32_t)Operators::BINARY_BITWISE_AND},
        {"__logia_bin_bitwise_xor", (uint32_t)Operators::BINARY_BITWISE_XOR},
        {"__logia_bin_bitwise_shl", (uint32_t)Operators::BINARY_BITWISE_LEFT_SHIFT},
        {"__logia_bin_bitwise_shr", (uint32_t)Operators::BINARY_BITWISE_RIGHT_SHIFT},

        {"__logia_deref", (uint32_t)Operators::PREFIX_DEREFERENCE},
        {"__logia_prefix_inc", (uint32_t)Operators::PREFIX_INCREMENT},
        {"__logia_prefix_dec", (uint32_t)Operators::PREFIX_DECREMENT},
        {"__logia_prefix_neg", (uint32_t)Operators::PREFIX_NEGATION},
        {"__logia_prefix_bitwise_not", (uint32_t)Operators::PREFIX_BITWISE_NOT},
        {"__logia_prefix_logical_not", (uint32_t)Operators::PREFIX_LOGICAL_NOT},

        {"__logia_postfix_inc", (uint32_t)Operators::POSTFIX_INCREMENT},
        {"__logia_postfix_dec", (uint32_t)Operators::POSTFIX_DECREMENT},
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
}
