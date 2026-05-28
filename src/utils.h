#pragma once

#include <cstdlib>
#include <csignal>
#include <format>
#include <fstream>
#include <string>
#include <iomanip> // setw

// Step 1: Helper macro to convert a value to a string
#define STRINGIFY(x) #x

// Step 2: Ensure macro arguments are expanded before stringizing
#define TOSTRING(x) STRINGIFY(x)

#define LOGIA_LEND
#define LOGIA_CLONE

// Define export/import macros for Windows
#ifdef _WIN32
#define LOGIA_EXPORT __declspec(dllexport)
#define LOGIA_API extern "C" LOGIA_EXPORT
#else
// On Linux/macOS, use GCC visibility attributes
#define LOGIA_EXPORT __attribute__((visibility("default")))
#define LOGIA_API extern "C" LOGIA_EXPORT
#endif

void print_stack_trace();

// Custom assert macro with stack trace

// Helper macros to detect if an argument is provided
#define SELECT_MACRO(_1, _2, NAME, ...) NAME

// Main macro that chooses the correct version based on arg count
#define LOGIA_ASSERT(...) SELECT_MACRO(__VA_ARGS__, LOGIA_ASSERT2, LOGIA_ASSERT1)(__VA_ARGS__)

#define LOGIA_VERIFY(expr)                                                     \
    do                                                                         \
    {                                                                          \
        if (!!(expr))                                                           \
        {                                                                      \
            std::cerr << "Verify failed: " << #expr                            \
                      << "\nAt file: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                               \
            std::abort();                                                      \
        }                                                                      \
    } while (0)

#define LOGIA_ASSERT1(expr)                                                    \
    do                                                                         \
    {                                                                          \
        if (!!(expr))                                                          \
        {                                                                      \
            std::cerr << "Assertion failed: " << #expr                         \
                      << "\nAt file: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                               \
            std::abort();                                                      \
        }                                                                      \
    } while (0)

#define LOGIA_ASSERT2(expr, message)                                        \
    do                                                                      \
    {                                                                       \
        if (!!(expr))                                                       \
        {                                                                   \
            std::cerr << "Assertion failed: " << #expr                      \
                      << message                                            \
                      << "\nFile: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                            \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

// NOTE do not uncooment -> cpp nonsense detected!
// #include "llvm/IR/Type.h"
namespace llvm
{
    class Type;
}
std::string llvm_type_to_string(llvm::Type *ty);