#pragma once

#include "logia/logia.h"

namespace logia::AST
{
    /// @brief Operators for binary expressions expression
    /// @remarks It starts at 128, there shall be no collisions with other operators!
    enum class Operators
    {
        DEFAULT = 0,

        BINARY_ADD = 1,             // +
        BINARY_SUB,                 // -
        BINARY_MUL,                 // *
        BINARY_DIV,                 // /
        BINARY_MOD,                 // %
        BINARY_COMPARISON_EQ,       // ==
        BINARY_COMPARISON_NEQ,      // !=
        BINARY_COMPARISON_LT,       // <
        BINARY_COMPARISON_GT,       // >
        BINARY_COMPARISON_LTE,      // <=
        BINARY_COMPARISON_GTE,      // >=
        BINARY_LOGICAL_AND,         // &&
        BINARY_LOGICAL_OR,          // ||
        BINARY_ASSIGN,              // =
        BINARY_ADD_ASSIGN,          // +=
        BINARY_SUB_ASSIGN,          // -=
        BINARY_MUL_ASSIGN,          // *=
        BINARY_DIV_ASSIGN,          // /=
        BINARY_BITWISE_OR,          // |
        BINARY_BITWISE_AND,         // &
        BINARY_BITWISE_XOR,         // ^
        BINARY_BITWISE_LEFT_SHIFT,  // <<
        BINARY_BITWISE_RIGHT_SHIFT, // >>

        BINARY_BITWISE_LEFT_SHIFT_ASSIGN,  // <<=
        BINARY_BITWISE_RIGHT_SHIFT_ASSIGN, // >>=
        BINARY_BITWISE_AND_ASSIGN,         // &=
        BINARY_BITWISE_XOR_ASSIGN,         // ^=
        BINARY_BITWISE_OR_ASSIGN,          // |=
        BINARY_MOD_ASSIGN,                 // %=

        PREFIX_DEREFERENCE = 128, // &
        PREFIX_NEGATION,          // -
        PREFIX_LOGICAL_NOT,       // !
        PREFIX_INCREMENT,         // ++
        PREFIX_DECREMENT,         // --
        PREFIX_BITWISE_NOT,       // ~

        POSTFIX_INCREMENT = 256, // ++
        POSTFIX_DECREMENT,       // --
    };
    /// @brief Returns if given operator is a bitwise
    bool is_bitwise_operator(Operators op);
    /// @brief Returns if given operator is a comparison
    bool is_comparison_operator(Operators op);
    /// @brief Returns if given operator is a logical
    bool is_logical_operator(Operators op);

    /// @brief Converts a binary operator to its string representation
    /// @param op The binary operator
    /// @return The string representation of the binary operator
    LOGIA_API const char *ast_operator_to_function_name(Operators op);
}