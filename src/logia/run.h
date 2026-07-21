#pragma once

#include <utility> // std::pair
#include <memory>  // std::unique_ptr
#include "logia.h" // LOGIA_API

namespace logia
{
    namespace AST
    {
        struct Program;
    }

    typedef std::pair<std::unique_ptr<AST::Program>, int> logia_run_result;
    LOGIA_API logia_run_result logia_run(int argc, const char *argv[]);
}