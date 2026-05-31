#pragma once

#include "logia/maybe_error.h"
#include "logia/type_system.h"
#include "logia/ast/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/function.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    struct md_type_error
    {
        Node *aggressor;
        type_system::type_compatibility reason;
    };
    // success -> points
    typedef utils::maybe_error<float, md_type_error> multiple_dispatch_result;

    /// @brief See <find>
    /// @param callexpr
    /// @param func
    /// @param change
    /// @return
    multiple_dispatch_result match(CallExpression *callexpr, Function *func, bool change = false);

    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// First it will search all functions with given callexpr name
    /// Then match all those functions with given callexpr
    /// * casting halve the points when no constexpr
    /// * any incompatibility lower points to zero (not compatible)
    /// If there is only one options left, it's used
    /// If there are more choose those with 1 point (the maximum)
    /// if there is one, it's used
    /// if there is more, semantic error
    /// At the end only one option can be used and should be no questions about it. Full compatibility one wins.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *call_expression);
}