#pragma once

namespace logia
{
    namespace AST
    {
        struct Node;
        struct Program;
    }
    /// @brief type inference the entire program
    void type_inference_program(AST::Program *program);
    /// @brief type inference just given node, should be used only on those AST that are created while type_inference
    void type_inference_node(AST::Program *program, AST::Node *node);
}