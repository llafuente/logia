#pragma once

namespace logia
{
    namespace AST
    {
        struct Program;
    }
    struct Backend;

    void codegen(AST::Program *program, Backend *backend);
}