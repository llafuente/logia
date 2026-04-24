module.exports = [
    {
        "operator": "+",
        "name": "add",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_add"
    },
    {
        "operator": "-",
        "name": "sub",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_sub"
    },
    {
        "operator": "*",
        "name": "mul",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_mul"
    },
    {
        "operator": "/",
        "name": "div",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_div"
    },
    {
        "operator": "%",
        "name": "mod",
        "type": "binary",
        "category": "arithmetic",
        "func": "logia_intrinsics_bin_mod"
    },

    {
        "operator": "==",
        "name": "eq",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_eq"
    },
    {
        "operator": "!=",
        "name": "neq",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_neq"
    },
    {
        "operator": "<",
        "name": "lt",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_lt"
    },
    {
        "operator": ">",
        "name": "gt",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_gt"
    },
    {
        "operator": "<=",
        "name": "lte",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_lte"
    },
    {
        "operator": ">=",
        "name": "gte",
        "type": "binary",
        "category": "comparison",
        "func": "logia_intrinsics_bin_gte"
    },

    {
        "operator": "=",
        "name": "assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_assign"
    },
    {
        "operator": "+=",
        "name": "add_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_add_assign"
    },
    {
        "operator": "-=",
        "name": "sub_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_sub_assign"
    },
    {
        "operator": "*=",
        "name": "mul_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_mul_assign"
    },
    {
        "operator": "/=",
        "name": "div_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_div_assign"
    },
    {
        "operator": "%=",
        "name": "mod_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_mod_assign"
    },
    {
        "operator": "&=",
        "name": "mod_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_and_assign"
    },
    {
        "operator": "|=",
        "name": "bitwise_or_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_or_assign"
    },
    {
        "operator": "^=",
        "name": "bitwise_or_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_xor_assign"
    },
    {
        "operator": ">>=",
        "name": "bitwise_or_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_shl_assign"
    },
    {
        "operator": "<<=",
        "name": "bitwise_or_assign",
        "type": "binary",
        "category": "assignment",
        "func": "logia_intrinsics_bin_bitwise_shr_assign"
    },
    {
        "operator": "&",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_and"
    },
    {
        "operator": "|",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_or"
    },
    {
        "operator": "^",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_xor",
    },
    {
        "operator": "<<",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_shl"
    },
    {
        "operator": ">>",
        "type": "binary",
        "category": "bitwise",
        "func": "logia_intrinsics_bin_bitwise_shr"
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