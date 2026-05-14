#pragma once

#include "ast/block.h"

namespace logia::AST
{
    struct Identifier;

    /// @brief Represents a package, which is a collection of functions and variables. It is the top-level scope of a program.
    struct Package : public Scope
    {
        const char *entry_point_file = nullptr;
        const char *file_contents = nullptr;

        Package(antlr4::ParserRuleContext *rule, const char *entry_point_file, const char *file_contents);

        std::string to_string() override;

        Type *get_type(void) override;

        /// @brief Inserts block into parent Function
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
}