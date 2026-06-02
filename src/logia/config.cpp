#include "logia/config.h"
#include <format>

namespace logia
{
    Config logia_config = {.cst_log_level = 3};

    std::string Config::to_string()
    {
        return std::format("is_program = {}\nverbose = {}\ndebug = {}\ncoverage = {}\nprint = {}\nprint_cst = {}\nprint_ast = {}\nemit_llvm = {}\nllfile = {}\nobjfile = {}, cst_log_level = {}\n", this->is_program, this->verbose, this->debug, this->coverage, this->print, this->print_cst, this->print_ast, this->emit_llvm, (this->llfile == nullptr ? "" : this->llfile), (this->objfile == nullptr ? "" : this->objfile), this->cst_log_level);
    }

    void Config::reset()
    {
        this->llfile = nullptr;
        this->objfile = nullptr;
    }
}