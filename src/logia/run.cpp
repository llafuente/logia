#include <iostream>
#include "logia/help.h"
#include "logia/frontend.h"

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

        logia::Config config;

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
                config.is_program = false;
                continue;
            }
            else if (strcmp("--print", argv[i]) == 0)
            {
                config.print = true;
                continue;
            }
            else if (strcmp("--print-cst", argv[i]) == 0)
            {
                config.print_cst = true;
                continue;
            }
            else if (strcmp("--print-ast", argv[i]) == 0)
            {
                config.print_ast = true;
                continue;
            }
            else if (strcmp("--emit-llvm", argv[i]) == 0)
            {
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-llvm, ignored" << std::endl;
                    continue;
                }
                config.llfile = argv[++i];
                continue;
            }
            else if (strcmp("--emit-obj", argv[i]) == 0)
            {
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-obj, ignored" << std::endl;
                    continue;
                }
                config.objfile = argv[++i];
                continue;
            }
            else if (strcmp("--verbose", argv[i]) == 0)
            {
                config.verbose = true;
                continue;
            }
            else if (strcmp("--debug", argv[i]) == 0)
            {
                config.debug = true;
                continue;
            }
            else if (strcmp("--coverage", argv[i]) == 0)
            {
                config.debug = true;
                continue;
            }
            std::cerr << "ignore unkown option: " << argv[i] << std::endl;
        }

        // frontend starts
        logia::ParseResult *parse_result = logia::logia_parse_program(argv[0]);

        auto backend = new logia::Backend(parse_result);
        backend->load_intrinsics();
        if (config.llfile != nullptr)
        {
            if (config.verbose)
            {
                std::cerr << "Emit ir file:" << config.llfile << std::endl;
            }
            backend->emitTargetLLVMIR(config.llfile);
        }

        if (config.objfile != nullptr)
        {
            if (config.verbose)
            {
                std::cout << "Emit obj file: " << config.objfile << std::endl;
            }
            backend->emitTargetObjectFile(config.objfile);
        }
        auto ret = backend->run_jit("main");

        delete parse_result;
        delete backend;
        return ret;
    }

} // namespace name
