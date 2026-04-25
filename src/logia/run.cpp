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

        logia::Frontend::Config config;

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
        logia::Frontend *frontend = new logia::Frontend(argv[0], config);

        frontend->parse();
        auto ret = frontend->run();

        delete frontend;
        return ret;
    }

} // namespace name
