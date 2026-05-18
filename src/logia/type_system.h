#pragma once

#include "logia/maybe_error.h"
#include "ast/type.h"
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

    /// @brief Type is compatible at layout level
    /// @param lhs
    /// @param rhs
    /// @return
    maybe_error<type_compatibility> type_is_compatible(::logia::AST::Type *lhs, ::logia::AST::Type *rhs);

}