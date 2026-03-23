#pragma once

#include <iostream>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <string>

#define DEBUG() logia_log_file
#define LOGIA_LEND

extern std::ofstream logia_log_file;
/**
 * Initialize log
 */
bool logia_init_log(char* file_name);
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
#define LOGIA_ASSERT(expr)                                                  \
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
