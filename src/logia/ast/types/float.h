#pragma once

#include "logia/ast/types/typedecl.h"

namespace logia::AST
{
    /// @brief Represents an floating point number
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Float : public TypeDecl
    {
    public:
        int bits;

        Float(int bits);
        ~Float();

        std::string to_string() override;

        std::string to_code(size_t ident = 0) override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };

}