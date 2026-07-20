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

        PREFIX_REFERENCE = 128, // &
        PREFIX_DEREFERENCE,     //
        PREFIX_NEGATION,        // -
        PREFIX_LOGICAL_NOT,     // !
        PREFIX_INCREMENT,       // ++
        PREFIX_DECREMENT,       // --
        PREFIX_BITWISE_NOT,     // ~

        POSTFIX_INCREMENT = 256, // ++
        POSTFIX_DECREMENT,       // --
    };
    /// @brief Returns if given operator is an assignment
    /// @param op The operator
    /// @return True if the operator is an assignment, false otherwise
    LOGIA_API bool is_assignment_operator(Operators op);
    /// @brief Returns if given operator is a bitwise
    /// @param op The operator
    /// @return True if the operator is a bitwise, false otherwise
    LOGIA_API bool is_bitwise_operator(Operators op);
    /// @brief Returns if given operator is a comparison
    /// @param op The operator
    /// @return True if the operator is a comparison, false otherwise
    LOGIA_API bool is_comparison_operator(Operators op);
    /// @brief Returns if given operator is a logical
    /// @param op The operator
    /// @return True if the operator is a logical, false otherwise
    LOGIA_API bool is_logical_operator(Operators op);
    /// @brief Returns if given operator is a prefix
    /// @param op The operator
    /// @return True if the operator is a prefix, false otherwise
    LOGIA_API bool is_prefix_operator(Operators op);
    /// @brief Returns if given operator is a postfix
    /// @param op The operator
    /// @return True if the operator is a postfix, false otherwise
    LOGIA_API bool is_postfix_operator(Operators op);
    /// @brief Converts an operator to its associated function name
    /// @param op The operator
    /// @return Function name
    LOGIA_API const char *ast_operator_to_function_name(Operators op);
    /// @brief Converts an operator to its string representation
    /// @param op The operator
    /// @return The string representation of the operator
    LOGIA_API const char *ast_operator_to_str(Operators op);
}