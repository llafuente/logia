#pragma once

#include "logia/ast/block.h"

namespace logia::AST
{
    /// @brief A block that is directly attached to a function, used for function body
    struct FunctionBlock : public Block
    {
        /// @brief Marks if the block is already inserted into the function
        unsigned char is_inserted : 1 = false;

        FunctionBlock(antlr4::ParserRuleContext *rule, Identifier *name);

        std::string to_string() override;

        /// @brief Inserts block into parent Function
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };
}