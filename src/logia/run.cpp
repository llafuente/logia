#include "run.h"

#include "utils.h"
#include "logia/help.h"
#include "logia/frontend.h"
#include "logia/backend.h"

#include <iostream>

namespace logia
{
    std::pair<std::unique_ptr<AST::Program>, int> logia_run(int argc, const char *argv[])
    {
        logia_config.reset();

        if (!argc)
        {
            print_usage("run");
            return {nullptr, 0};
        }

        // skip first, it's entry point file
        // parse args -> store in config -> throw on error
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp("--help", argv[i]) == 0)
            {
                print_usage("run");
                return {nullptr, 0};
            }
            else if (strcmp("--package", argv[i]) == 0)
            {
                logia_config.is_program = false;
                continue;
            }
            else if (strcmp("--print", argv[i]) == 0)
            {
                logia_config.print = true;
                continue;
            }
            else if (strcmp("--print-cst", argv[i]) == 0)
            {
                logia_config.print_cst = true;
                continue;
            }
            else if (strcmp("--print-ast", argv[i]) == 0)
            {
                logia_config.print_ast = true;
                continue;
            }
            else if (strcmp("--emit-llvm", argv[i]) == 0)
            {
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-llvm, ignored" << std::endl;
                    continue;
                }
                logia_config.llfile = argv[++i];
                continue;
            }
            else if (strcmp("--emit-obj", argv[i]) == 0)
            {
                if (i + 1 == argc)
                {
                    std::cerr << "expected a file after --emit-obj, ignored" << std::endl;
                    continue;
                }
                logia_config.objfile = argv[++i];
                continue;
            }
            else if (strcmp("--verbose", argv[i]) == 0)
            {
                logia_config.verbose = true;
                continue;
            }
            else if (strcmp("--debug", argv[i]) == 0)
            {
                logia_config.debug = true;
                continue;
            }
            else if (strcmp("--coverage", argv[i]) == 0)
            {
                logia_config.debug = true;
                continue;
            }
            std::cerr << "ignore unkown option: " << argv[i] << std::endl;
        }

        // frontend starts
        std::unique_ptr<AST::Program> program = logia::logia_parse_program(argv[0]);

        if (logia_config.llfile != nullptr)
        {
            if (logia_config.verbose)
            {
                std::cerr << "Emit ir file:" << logia_config.llfile << std::endl;
            }
            program->backend->emitTargetLLVMIR(logia_config.llfile);
        }

        if (logia_config.objfile != nullptr)
        {
            if (logia_config.verbose)
            {
                std::cout << "Emit obj file: " << logia_config.objfile << std::endl;
            }
            program->backend->emitTargetObjectFile(logia_config.objfile);
        }
        auto ret = program->backend->run_jit("main");

        logia_config.reset();

        return {program, ret};
    }

} // namespace name
