module.exports = [
    {
        "operator": "+",
        "name": "add",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia__intrinsics_bin_add",
        "logia_fn": "__logia_bin_add"
    },
    {
        "operator": "-",
        "name": "sub",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_sub",
        "logia_fn": "__logia_bin_sub"
    },
    {
        "operator": "*",
        "name": "mul",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_mul",
        "logia_fn": "__logia_bin_mul"
    },
    {
        "operator": "/",
        "name": "div",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_div",
        "logia_fn": "__logia_bin_div"
    },
    {
        "operator": "%",
        "name": "mod",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_mod",
        "logia_fn": "__logia_bin_mod"
    },

    {
        "operator": "==",
        "name": "eq",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_eq",
        "logia_fn": "__logia_bin_eq"
    },
    {
        "operator": "!=",
        "name": "neq",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_neq",
        "logia_fn": "__logia_bin_neq"
    },
    {
        "operator": "<",
        "name": "lt",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_lt",
        "logia_fn": "__logia_bin_lt"
    },
    {
        "operator": ">",
        "name": "gt",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_gt",
        "logia_fn": "__logia_bin_gt"
    },
    {
        "operator": "<=",
        "name": "lte",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_lte",
        "logia_fn": "__logia_bin_lte"
    },
    {
        "operator": ">=",
        "name": "gte",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_gte",
        "logia_fn": "__logia_bin_gte"
    },

    {
        "operator": "=",
        "name": "assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_assign",
        "logia_fn": "__logia_bin_assign"
    },
    {
        "operator": "+=",
        "name": "add_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_add_assign",
        "logia_fn": "__logia_bin_add_assign"
    },
    {
        "operator": "-=",
        "name": "sub_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_sub_assign",
        "logia_fn": "__logia_bin_sub_assign"
    },
    {
        "operator": "*=",
        "name": "mul_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_mul_assign",
        "logia_fn": "__logia_bin_mul_assign"
    },
    {
        "operator": "/=",
        "name": "div_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_div_assign",
        "logia_fn": "__logia_bin_div_assign"
    },
    {
        "operator": "%=",
        "name": "mod_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_mod_assign",
        "logia_fn": "__logia_bin_mod_assign"
    },
    {
        "operator": "&=",
        "name": "bitwise_and_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_and_assign",
        "logia_fn": "__logia_bin_bitwise_and_assign"
    },
    {
        "operator": "|=",
        "name": "bitwise_or_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_or_assign",
        "logia_fn": "__logia_bin_bitwise_or_assign"
    },
    {
        "operator": "^=",
        "name": "bitwise_xor_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_xor_assign",
        "logia_fn": "__logia_bin_bitwise_xor_assign"
    },
    {
        "operator": ">>=",
        "name": "bitwise_shr_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_shr_assign",
        "logia_fn": "__logia_bin_bitwise_shr_assign"
    },
    {
        "operator": "<<=",
        "name": "bitwise_shl_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_shl_assign",
        "logia_fn": "__logia_bin_bitwise_shl_assign"
    },

    {
        "operator": "&",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_and",
        "logia_fn": "__logia_bin_bitwise_and"
    },
    {
        "operator": "|",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_or",
        "logia_fn": "__logia_bin_bitwise_or"
    },
    {
        "operator": "^",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_xor",
        "logia_fn": "__logia_bin_bitwise_xor"
    },
    {
        "operator": "<<",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_shl",
        "logia_fn": "__logia_bin_bitwise_shl"
    },
    {
        "operator": ">>",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_shr",
        "logia_fn": "__logia_bin_bitwise_shr"
    },
    /*
    // implemented inside compiler
    {
        "operator": "&&",
        "type": "binary",
        "category": "logical",
        "func": "logia_intrinsics_bin_logical_and"
    },
    {
        "operator": "||",
        "type": "binary",
        "category": "logical",
        "func": "logia_intrinsics_bin_logical_or",
    },
    */
]