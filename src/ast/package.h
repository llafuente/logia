#pragma once

#include "ast/block.h"

namespace logia::AST
{
    struct Identifier;

    struct Package : public Block
    {
        const char *entry_point_file = nullptr;
        const char *file_contents = nullptr;

        Package(antlr4::ParserRuleContext *rule, const char *entry_point_file, const char *file_contents);

        std::string to_string() override;

        /// @brief Inserts block into parent Function
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
}