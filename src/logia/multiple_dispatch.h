#pragma once

#include "logia/maybe_error.h"
#include "logia/type_system.h"
#include "logia/ast/types/type.h"
#include "logia/ast/expr.h"
#include "logia/ast/types/function.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    struct match_error
    {
        Node *aggressor;
        type_system::type_compatibility reason;
    };
    // success -> points
    typedef utils::maybe_error<float, match_error> match_result;

    /// @brief See <find>
    /// @param callexpr
    /// @param func
    /// @param change
    /// @return
    match_result match(CallExpression *callexpr, Function *func, bool change = false);

    struct find_error
    {
        std::vector<std::tuple<float, Function *>> candidates = {};
        CallExpression *callexpr = nullptr;
    };

    typedef utils::maybe_error<Function *, find_error> find_one_result;

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
    /// @param callexpr The call expression to match against available overloads.
    find_one_result find_one(std::vector<Function *> candidates, CallExpression *callexpr);
}