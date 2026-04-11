#include <iostream>
#include <iomanip>

namespace logia
{
    void print_usage(const char *command)
    {
        std::cout << "logia compiler. Pre-alpha version" << std::endl;

        std::cout << "Usage: logia command [options]" << std::endl;
        std::cout << "Commands" << std::endl;
        auto LEFT_COLUMN = 16;
        std::cout << std::left << std::setw(LEFT_COLUMN) << "    run" << "Run a logia file" << std::endl;
        std::cout << std::left << std::setw(LEFT_COLUMN) << "" << "logia run main.logia" << std::endl;
        if (command != nullptr && strcmp(command, "run") == 0)
        {
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    [Options]" << std::endl;
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    --print" << "Prints logia file at stderr" << std::endl;
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    --print-cst" << "Prints CST at stderr" << std::endl;
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    --print-ast" << "Prints AST at stderr" << std::endl;
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    --emit-llvm" << "Prints LLVM IR at stderr" << std::endl;
            std::cout << std::left << std::setw(LEFT_COLUMN) << "    --verbose" << "Prints debug information at stderr" << std::endl;
        }

        /*
            std::cout << std::setw(16) << "    check" << "Check syntax and semantics of a logia file" << std::endl;
            std::cout << std::setw(16) << "" << "logia check main.logia" << std::endl;


            std::cout << "  check <file> [options]" << std::endl;
            std::cout << "    checks syntax and semantics of given file" << std::endl;
            std::cout << "    [options]" << std::endl;
            std::cout << "      --json prints output as json" << std::endl;
            std::cout << "  build <file> <output> [options]" << std::endl;
            std::cout << "    [options]" << std::endl;
            std::cout << "    --emit-llvm prints llvm intermediary representation" << std::endl;
            std::cout << "  run <file>" << std::endl;
            std::cout << "    JIT compiles and runs main function" << std::endl;
            std::cout << "  test <file.logia|file.clogia> [filter] [options]" << std::endl;
            std::cout << "    JIT compiles and runs all tests found in given file" << std::endl;
            std::cout << "    if file is a logia-compiler-test it will check process output match the expected" << std::endl;
            std::cout << "    [filter] only executes tests that contains given filter (string|regex)" << std::endl;
            std::cout << "    [options]" << std::endl;
            std::cout << "      --all-test Run all test found in all compiled files" << std::endl;
            std::cout << "      --update Updates test output" << std::endl;
        */
    }
}