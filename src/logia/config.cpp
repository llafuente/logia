#include "logia/config.h"
#include <format>

namespace logia
{
    Config logia_config = {};

    std::string Config::to_string()
    {
        return std::format("is_program = {}\nverbose = {}\ndebug = {}\ncoverage = {}\nprint = {}\nprint_cst = {}\nprint_ast = {}\nemit_llvm = {}\nllfile = {}\nobjfile = {}\n", this->is_program, this->verbose, this->debug, this->coverage, this->print, this->print_cst, this->print_ast, this->emit_llvm, this->llfile, this->objfile);
    }
}