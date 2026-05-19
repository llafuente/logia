#pragma one

#include "ast/type.h"
#include "ast/expr.h"

namespace logia::multiple_dispatching
{
    using namespace logia::AST;
    bool match(CallExpression *call_expression, Function *func, bool change = false);
    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *call_expression);
}