#pragma once

#include <cstdlib>
#include <csignal>
#include <format>
#include <fstream>
#include <string>
#include <iomanip> // setw

#include <iostream>

// Step 1: Helper macro to convert a value to a string
#define STRINGIFY(x) #x

// Step 2: Ensure macro arguments are expanded before stringizing
#define TOSTRING(x) STRINGIFY(x)

void print_stack_trace();

#include <csignal>
#include <cstdlib>

// Cross-platform debug break
inline void debug_break()
{
#if defined(_MSC_VER)
    __debugbreak(); // MSVC intrinsic
#elif defined(__clang__) || defined(__GNUC__)
#if defined(__i386__) || defined(__x86_64__)
    __asm__ volatile("int3");
#elif defined(__aarch64__)
    __asm__ volatile(".inst 0xd4200000"); // ARM64 BRK instruction
#elif defined(__arm__)
    __asm__ volatile(".inst 0xe7f001f0"); // ARM BKPT instruction
#else
    // Fallback: send SIGTRAP
    raise(SIGTRAP);
#endif
#else
    // Last resort
    raise(SIGTRAP);
#endif
}

// Custom assert macro with stack trace

// Helper macros to detect if an argument is provided
#define SELECT_MACRO(_1, _2, NAME, ...) NAME

// Main macro that chooses the correct version based on arg count
// #define LOGIA_VERIFY(...) SELECT_MACRO(__VA_ARGS__, LOGIA_VERIFY_1, LOGIA_VERIFY_2)(__VA_ARGS__)
#define LOGIA_VERIFY(...) SELECT_MACRO(__VA_ARGS__, LOGIA_VERIFY_2, LOGIA_VERIFY_1)(__VA_ARGS__)

#define LOGIA_VERIFY_1(expr)                                                   \
    do                                                                         \
    {                                                                          \
        if (!(!!(expr)))                                                       \
        {                                                                      \
            std::cerr << "Assertion failed: " << #expr                         \
                      << "\nAt file: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                               \
            debug_break();                                                     \
            std::abort();                                                      \
        }                                                                      \
    } while (0)

#define LOGIA_VERIFY_2(expr, message)                                       \
    do                                                                      \
    {                                                                       \
        if (!(!!(expr)))                                                    \
        {                                                                   \
            std::cerr << "Assertion failed: " << #expr                      \
                      << message                                            \
                      << "\nFile: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                            \
            debug_break();                                                  \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

namespace logia
{
    template <class T>
    class EnumPure
    {
    public:
        uint32_t value = 0;

        constexpr EnumPure() = default;
        constexpr EnumPure(uint32_t v) : value(v) {}

        // Allow switch and comparisons.
        constexpr operator uint32_t() const { return value; }

        // Prevent usage: if(fruit)
        explicit operator bool() const = delete;

        constexpr bool operator==(T a) const { return value == a.value; }
        constexpr bool operator==(uint32_t v) const { return value == v; }

        constexpr bool is(T a) const { return value == a.value; }
        constexpr bool is(uint32_t v) const { return value == v; }

        constexpr bool operator!=(T a) const { return value != a.value; }
        constexpr T operator|(T a) const { return T((uint32_t)((uint32_t)value | (uint32_t)a.value)); }
    };

    template <typename T>
    class EnumFlags : EnumPure<T>
    {
    public:
        constexpr EnumFlags() = default;
        constexpr EnumFlags(uint32_t v) : EnumPure<T>(v) {}

        constexpr bool contains(T v) const { return ((uint32_t)value & (uint32_t)v.value) != 0; }
        constexpr EnumFlags add(T v) const { return ((uint32_t)value & (uint32_t)v.value) != 0; }
    };
}