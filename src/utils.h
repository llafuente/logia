#pragma once

#include <iostream>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <string>

// Step 1: Helper macro to convert a value to a string
#define STRINGIFY(x) #x

// Step 2: Ensure macro arguments are expanded before stringizing
#define TOSTRING(x) STRINGIFY(x)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define __FILENAME_LINE__ (__FILE__ ":" TOSTRING(__LINE__))

#define DEBUG() logia_log_file << "DBG | " << std::setw(16) << __FILENAME_LINE__ << " " << __FUNCTION__ << " "
#define ERROR() logia_log_file << "ERR | " << std::setw(16) << __FILENAME_LINE__ << " " << __FUNCTION__ << " "
// #define DEBUG() logia_log_file << __FUNCTION__ << ":" << __LINE__ << " "

#define LOGIA_LEND

extern std::ofstream logia_log_file;
/**
 * Initialize log
 */
bool logia_init_log(char *file_name, bool append = false);
void logia_deinit_log();

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

#define LOGIA_ASSERT1(expr)                                                 \
    do                                                                      \
    {                                                                       \
        if (!(expr))                                                        \
        {                                                                   \
            std::cerr << "Assertion failed: " << #expr                      \
                      << "\nFile: " << __FILE__ << ":" << __LINE__ << "\n"; \
            print_stack_trace();                                            \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#define LOGIA_ASSERT2(expr, message)                                        \
    do                                                                      \
    {                                                                       \
        if (!(expr))                                                        \
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