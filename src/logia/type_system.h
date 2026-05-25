#pragma once

#include "logia/maybe_error.h"
#include "logia/ast/type.h"
#include <cstdio>

namespace logia::type_system
{

    enum class type_compatibility : uint32_t
    {
        NO = 0x000,
        YES = 0x001,
        EXPLICIT_CAST = 0x010,
        AUTOCAST_CAST = 0x020,
        LAYOUT_COMPATIBLE = 0x100,
        CODE_COMPATIBLE = 0x200,
    };

    typedef utils::maybe_error<type_compatibility, type_compatibility> type_compatibility_result;

    /// @brief Determines if two types are compatible. An error does mean it's not compatible, it may require an explicit cast or an autocast
    /// @param lhs
    /// @param rhs
    /// @return
    type_compatibility_result type_is_compatible(::logia::AST::Type *lhs, ::logia::AST::Type *rhs);

}