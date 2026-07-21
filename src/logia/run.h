#pragma once

namespace logia
{
    typedef std::pair<std::unique_ptr<AST::Program>, int> logia_run_result;
    logia_run_result logia_run(int argc, const char *argv[]);
}