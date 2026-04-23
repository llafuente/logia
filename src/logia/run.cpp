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
        frontend->set_file(argv[0]);

        // parse common options
        bool print = false;
        bool print_cst = false;
        bool print_ast = false;

        bool emit_llvm = false;
        const char *llfile = nullptr;

        bool emit_obj = false;
        const char *objfile = nullptr;

        // skip first, it's entry point file
        for (int i = 1; i < argc; ++i)
        {
            // std::cout << argv[i] << std::endl;
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
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-llvm, ignored" << std::endl;
                    continue;
                }
                emit_llvm = true;
                llfile = argv[++i];
                continue;
            }
            else if (strcmp("--emit-obj", argv[i]) == 0)
            {
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-obj, ignored" << std::endl;
                    continue;
                }
                emit_obj = true;
                objfile = argv[++i];
                continue;
            }
            else if (strcmp("--verbose", argv[i]) == 0)
            {
                frontend->verbose = true;
                continue;
            }
            else if (strcmp("--debug", argv[i]) == 0)
            {
                frontend->debug = true;
                continue;
            }
            else if (strcmp("--coverage", argv[i]) == 0)
            {
                frontend->debug = true;
                continue;
            }
            std::cerr << "ignore unkown option: " << argv[i] << std::endl;
        }

        if (frontend->verbose)
        {
            TCHAR cwd[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, cwd);

            std::cout
                << "Configuration:" << std::endl;
            // << "* cwd: " << std::filesystem::current_path() << std::endl
            std::wcout
                << "* cwd: " << cwd << std::endl;
            std::cout
                << "* input file: " << frontend->entry_point_path << std::endl
                << "* print: " << (print ? "yes" : "no") << std::endl
                << "* program: " << (frontend->is_program ? "yes" : "no") << std::endl;

            if (emit_llvm)
            {
                std::cout << "* llvm ir file:" << llfile << std::endl;
            }
            if (emit_obj)
            {
                std::cout << "* obj file: " << objfile << std::endl;
            }
        }

        // frontend starts

        if (print)
        {
            std::cerr << "File Contents:" << std::endl
                      << frontend->text << std::endl;
        }
        frontend->parse();

        frontend->print_cst(print_cst ? std::cerr : logia_log_file);

        frontend->build_ast();

        frontend->print_ast(print_ast ? std::cerr : logia_log_file);

        // TODO maybe we should fordward to backend in a far future when api stable
        // backend starts

        if (emit_llvm)
        {
            frontend->backend->emitTargetLLVMIR(llfile);
        }

        if (emit_obj)
        {
            frontend->backend->emitTargetObjectFile(objfile);
        }

        auto ret = frontend->backend->run_jit("main");
        delete frontend;
        return ret;
    }

} // namespace name
