#pragma once

#include "logia/maybe_error.h"
#include "logia/type_system.h"
#include "ast/type.h"
#include "ast/expr.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    struct md_type_error
    {
        Node *aggressor;
        type_system::type_compatibility reason;
    };
    // success -> points
    typedef utils::maybe_error<size_t, md_type_error> multiple_dispatch_result;

    multiple_dispatch_result match(CallExpression *callexpr, Function *func, bool change = false);
    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *call_expression);
}