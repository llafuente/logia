#include "ast/function.h"
#include "ast/expr.h"

namespace logia::AST
{
    struct FunctionToOperator
    {
        const char fn_name[36];
        uint32_t op_value;
    };
    uint32_t f2op_length = 23 + 6 + 2;
    FunctionToOperator f2op[23 + 6 + 2] = {
        {"logia_intrinsics_bin_assign", (uint32_t)BinaryOperator::ASSIGN},
        {"logia_intrinsics_bin_add_assign", (uint32_t)BinaryOperator::ADD_ASSIGN},
        {"logia_intrinsics_bin_sub_assign", (uint32_t)BinaryOperator::SUB_ASSIGN},
        {"logia_intrinsics_bin_div_assign", (uint32_t)BinaryOperator::DIV_ASSIGN},
        {"logia_intrinsics_bin_mul_assign", (uint32_t)BinaryOperator::MUL_ASSIGN},
        {"logia_intrinsics_bin_add", (uint32_t)BinaryOperator::ADD},
        {"logia_intrinsics_bin_sub", (uint32_t)BinaryOperator::SUB},
        {"logia_intrinsics_bin_mul", (uint32_t)BinaryOperator::MUL},
        {"logia_intrinsics_bin_div", (uint32_t)BinaryOperator::DIV},
        {"logia_intrinsics_bin_mod", (uint32_t)BinaryOperator::MOD},
        {"logia_intrinsics_bin_eq", (uint32_t)BinaryOperator::LOGIAL_EQ},
        {"logia_intrinsics_bin_neq", (uint32_t)BinaryOperator::LOGIAL_NEQ},
        {"logia_intrinsics_bin_lt", (uint32_t)BinaryOperator::LOGIAL_LT},
        {"logia_intrinsics_bin_gt", (uint32_t)BinaryOperator::LOGIAL_GT},
        {"logia_intrinsics_bin_lte", (uint32_t)BinaryOperator::LOGIAL_LTE},
        {"logia_intrinsics_bin_gte", (uint32_t)BinaryOperator::LOGIAL_GTE},
        {"logia_intrinsics_bin_logical_and", (uint32_t)BinaryOperator::LOGICAL_AND},
        {"logia_intrinsics_bin_logical_or", (uint32_t)BinaryOperator::LOGICAL_OR},
        {"logia_intrinsics_bin_bitwise_or", (uint32_t)BinaryOperator::BITWISE_OR},
        {"logia_intrinsics_bin_bitwise_and", (uint32_t)BinaryOperator::BITWISE_AND},
        {"logia_intrinsics_bin_bitwise_xor", (uint32_t)BinaryOperator::BITWISE_XOR},
        {"logia_intrinsics_bin_bitwise_shl", (uint32_t)BinaryOperator::BITWISE_LEFT_SHIFT},
        {"logia_intrinsics_bin_bitwise_shr", (uint32_t)BinaryOperator::BITWISE_RIGHT_SHIFT},

        {"logia_intrinsics_deref", (uint32_t)PrefixUnaryOperator::DEREFERENCE},
        {"logia_intrinsics_prefix_inc", (uint32_t)PrefixUnaryOperator::INCREMENT},
        {"logia_intrinsics_prefix_dec", (uint32_t)PrefixUnaryOperator::DECREMENT},
        {"logia_intrinsics_prefix_neg", (uint32_t)PrefixUnaryOperator::NEGATION},
        {"logia_intrinsics_prefix_bitwise_not", (uint32_t)PrefixUnaryOperator::BITWISE_NOT},
        {"logia_intrinsics_prefix_logical_not", (uint32_t)PrefixUnaryOperator::LOGICAL_NOT},

        {"logia_intrinsics_postfix_inc", (uint32_t)PostfixUnaryOperator::INCREMENT},
        {"logia_intrinsics_postfix_dec", (uint32_t)PostfixUnaryOperator::DECREMENT},
    };

}