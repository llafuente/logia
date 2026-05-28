#pragma once

#include <string>

namespace logia
{
    struct Config;

    extern Config logia_config;

    struct Config
    {
        bool is_program = true;
        bool verbose = false;
        bool debug = false;
        bool coverage = false;
        bool print = false;

        int cst_log_level = 6;
        bool print_cst = false;

        int ast_log_level = 6;
        bool print_ast = false;

        int type_system_log_level = 6;

        int codegen_log_level = 6;
        bool emit_llvm = false;
        const char *llfile = nullptr;
        const char *objfile = nullptr;

        Config *instance()
        {
            return &logia_config;
        }
        std::string to_string();
    };
}