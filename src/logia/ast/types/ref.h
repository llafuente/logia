#pragma once

#include "logia/ast/types/pointer.h"
#include "logia/ast/operators.h"

namespace logia::AST
{

    /// @brief Represents a referente to a single object (it's a pointer that chan't move!!)
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Ref : public Pointer
    {
    public:
        TypeDecl *pointee = nullptr;
        /// @brief Reference to an unkown type
        Ref();
        /// @brief Reference to a kown type!
        /// @param pointee
        Ref(TypeDecl *pointee);

        ~Ref();

        TypeDecl *get_pointee();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };
}