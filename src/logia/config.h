#pragma once

namespace logia
{
    struct Config
    {
        bool is_program = true;
        bool verbose = false;
        bool debug = false;
        bool coverage = false;
        bool print = false;
        bool print_cst = false;
        bool print_ast = false;
        bool emit_llvm = false;
        const char *llfile = nullptr;
        const char *objfile = nullptr;
    };
}