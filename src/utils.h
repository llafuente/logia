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
            std::abort();                                                   \
        }                                                                   \
    } while (0)
