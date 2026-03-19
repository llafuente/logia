#pragma once

#include <iostream>
#include <cstdlib>
#include <csignal>

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
