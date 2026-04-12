#include <iostream>
#include "logia/help.h"
#include "logia/frontend.h"
#include <Windows.h>

#include "utils.h"

namespace logia
{
    int logia_run(int argc, const char *argv[])
    {
        if (!argc)
        {
            print_usage("run");
            return 0;
        }
        logia::Frontend *frontend = new logia::Frontend();
        const char *file_path = argv[0];

        // parse common options
        bool print = false;
        bool print_cst = false;
        bool print_ast = false;
        bool emit_llvm = false;
        bool verbose = false;

        // skip first, it's entry point file
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp("--help", argv[i]) == 0)
            {
                print_usage("run");
                return 0;
            }
            else if (strcmp("--package", argv[i]) == 0)
            {
                frontend->is_program = false;
                continue;
            }
            else if (strcmp("--print", argv[i]) == 0)
            {
                print = true;
                continue;
            }
            else if (strcmp("--print-cst", argv[i]) == 0)
            {
                print_cst = true;
                continue;
            }
            else if (strcmp("--print-ast", argv[i]) == 0)
            {
                print_ast = true;
                continue;
            }
            else if (strcmp("--emit-llvm", argv[i]) == 0)
            {
                emit_llvm = true;
                continue;
            }
            else if (strcmp("--verbose", argv[i]) == 0)
            {
                verbose = true;
                frontend->verbose = true;
                continue;
            }
            std::cerr << "ignore unkown option: " << argv[i] << std::endl;
        }

        if (verbose)
        {
            TCHAR cwd[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, cwd);

            std::cout
                << "Configuration:" << std::endl;
            // << "* cwd: " << std::filesystem::current_path() << std::endl
            std::wcout
                << "* cwd: " << cwd << std::endl;
            std::cout
                << "* file: " << file_path << std::endl
                << "* print: " << (print ? "yes" : "no") << std::endl
                << "* program: " << (frontend->is_program ? "yes" : "no") << std::endl;
        }

        frontend->read(file_path);
        if (print)
        {
            std::cerr << "File Contents:" << std::endl
                      << frontend->text << std::endl;
        }
        antlr4::ParserRuleContext *tree = frontend->parse();

        frontend->print_cst(print_cst ? std::cerr : logia_log_file);

        frontend->build_ast();

        frontend->print_ast(print_ast ? std::cerr : logia_log_file);

        if (emit_llvm)
        {
            // logia_compiler->backend->module->print(llvm::outs(), nullptr);
            frontend->backend->module->print(llvm::errs(), nullptr);
        }

        auto ret = frontend->backend->run_jit("main");
        delete frontend;
        return ret;
    }

} // namespace name
