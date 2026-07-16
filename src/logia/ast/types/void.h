#pragma once

#include "logia/ast/types/typedecl.h"

namespace logia::AST
{
    /// @brief Represents void aka no-type
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Void : public TypeDecl
    {
    public:
        Void();
        ~Void();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;
        void validate() override;
    };
}