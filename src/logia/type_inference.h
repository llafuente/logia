#pragma once

namespace logia
{
    namespace AST
    {
        struct Program;
    }

    void type_inference(AST::Program *program);
}