#pragma once

#include "utils.h"

namespace logia::AST
{
    /// @brief Operators for binary expressions expression
    /// @remarks It starts at 128, there shall be no collisions with other operators!
    enum class Operators
    {
        BINARY_ADD = 1,             // +
        BINARY_SUB,                 // -
        BINARY_MUL,                 // *
        BINARY_DIV,                 // /
        BINARY_MOD,                 // %
        BINARY_LOGIAL_EQ,           // ==
        BINARY_LOGIAL_NEQ,          // !=
        BINARY_LOGIAL_LT,           // <
        BINARY_LOGIAL_GT,           // >
        BINARY_LOGIAL_LTE,          // <=
        BINARY_LOGIAL_GTE,          // >=
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

    /// @brief Converts a binary operator to its string representation
    /// @param op The binary operator
    /// @return The string representation of the binary operator
    LOGIA_API const char *ast_operator_to_function_name(Operators op);
}