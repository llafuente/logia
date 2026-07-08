#pragma once

#include "logia/ast/program.h"

namespace logia::AST
{
    struct Identifier;

    /// @brief Represents a package, which is a collection of functions and variables. It is the top-level scope of a program.
    struct Package : public File
    {
        Package(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents);

        std::string to_string() override;

        Type *get_type(void) override;

        /// @brief Inserts block into parent Function
        void pre_codegen(logia::Backend *backend) override;

        /// @brief Sets current block and codegen children
        void post_codegen(logia::Backend *backend) override;
    };
}