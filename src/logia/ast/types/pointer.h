#pragma once

#include "logia/ast/types/typedecl.h"

namespace logia::AST
{

    /// @brief Represents an opaque pointer
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Pointer : public TypeDecl
    {
    public:
        llvm::PointerType *ir_ptype;

        Pointer();
        ~Pointer();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;
        void validate() override;
    };

}