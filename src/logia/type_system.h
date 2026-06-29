#pragma once

#include "logia/maybe_error.h"
#include "logia/ast/type.h"
#include <cstdio>

namespace logia::type_system
{
    // this is exactly what will end up being an enum/mask in logia
    class type_compatibility
    {
        uint32_t value;

    public:
        static type_compatibility NO;
        static type_compatibility YES;
        static type_compatibility EXPLICIT_CAST;
        static type_compatibility AUTOCAST_CAST;
        static type_compatibility AUTOCAST_REF;
        static type_compatibility AUTOCAST_DEREF;
        static type_compatibility LAYOUT_COMPATIBLE;
        static type_compatibility CODE_COMPATIBLE;

        // type_compatibility(uint32_t v) : value(v) {}
        constexpr type_compatibility() = default;
        constexpr type_compatibility(uint32_t v) : value(v) {}

        // Allow switch and comparisons.
        constexpr operator uint32_t() const { return value; }

        // Prevent usage: if(fruit)
        explicit operator bool() const = delete;

        constexpr bool operator==(type_compatibility a) const { return value == a.value; }
        constexpr bool operator!=(type_compatibility a) const { return value != a.value; }
        constexpr type_compatibility operator|(type_compatibility a) const { return type_compatibility((uint32_t)((uint32_t)value | (uint32_t)a.value)); }

        constexpr bool contains(type_compatibility v) const { return ((uint32_t)value & (uint32_t)v.value) != 0; }

        std::string to_string()
        {
            std::string out;
            if (this->contains(type_compatibility::NO))
            {
                out += "NO";
            }
            if (this->contains(type_compatibility::YES))
            {
                out += (out.length() ? "," : "");
                out += "YES";
            }
            if (this->contains(type_compatibility::EXPLICIT_CAST))
            {
                out += (out.length() ? "," : "");
                out += "EXPLICIT_CAST";
            }
            if (this->contains(type_compatibility::AUTOCAST_CAST))
            {
                out += (out.length() ? "," : "");
                out += "AUTOCAST_CAST";
            }
            if (this->contains(type_compatibility::LAYOUT_COMPATIBLE))
            {
                out += (out.length() ? "," : "");
                out += "LAYOUT_COMPATIBLE";
            }
            if (this->contains(type_compatibility::CODE_COMPATIBLE))
            {
                out += (out.length() ? "," : "");
                out += "CODE_COMPATIBLE";
            }
            return out;
        }
    };

    typedef utils::maybe_error<type_compatibility, type_compatibility> type_compatibility_result;

    /// @brief Determines if two types are compatible. An error does mean it's not compatible, it may require an explicit cast or an autocast
    /// @param lhs
    /// @param rhs
    /// @return
    type_compatibility_result type_is_compatible(::logia::AST::Type *lhs, ::logia::AST::Type *rhs);

}