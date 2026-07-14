#pragma once

namespace logia
{
// it goes from 1-3, 0 means no pass
#define TYPE_INFERENCE_EARLY 1
#define TYPE_INFERENCE_PRE 2
#define TYPE_INFERENCE_POST 3
#define TYPE_INFERENCE_LAST 3

#define TYPE_INFERENCE_MAX 3

    namespace AST
    {
        struct Node;
        struct Program;
    }
    /// @brief type inference the entire program
    void type_inference_program(AST::Program *program);
    /// @brief type inference just given node, should be used only on those AST that are created while type_inference
    /// @details
    /// first it will assign default types for constants
    /// i64, f64, string (type inference will narrrow the type later if needed)
    /// type inference is done in (TYPE_INFERENCE_MAX) passes
    /// each pass will call type_inference with the given id
    /// if the node is able to do the pass it will set Node.type_inference_pass_id
    /// if not, the node is queued and it will retry at the end
    /// Once the pass ends, process the pending queue as many times as needed but each time we should have less nodes or throw
    /// Once all passes ended, we should have all typed and no modifications are allowed
    void type_inference_node(AST::Program *program, AST::Node *start_node);

    /// @brief type inference given node / pass
    void type_inference_pass(AST::Program *program, AST::Node *start_node, size_t pass_id);

    /// @brief remove type information from given node!
    void type_inference_untype(AST::Node *node);
}