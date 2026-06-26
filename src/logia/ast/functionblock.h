#pragma once

#include "logia/ast/block.h"

namespace logia::AST
{
    /// @brief A block that is directly attached to a function, used for function body
    struct FunctionBlock : public Block
    {
        /// @brief Marks if the block is already inserted into the function
        unsigned char is_inserted : 1 = false;
        /// @brief Constructs a function block
        /// @param rule The parser rule context
        /// @param name The name of the function block
        FunctionBlock(location loc, Identifier *name);

        std::string to_string() override;

        /// @brief Inserts block into parent Function
        /// @param backend The backend used for code generation
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        /// @param backend The backend used for code generation
        llvm::Value *post_codegen(logia::Backend *backend) override;

        /// @remarks while FunctionBlock is a Block, we should ignore it's post_attach
        void on_after_attach() override;
        /// @remarks while FunctionBlock is a Block, we should ignore it's validation
        void validate() override;
    };
}