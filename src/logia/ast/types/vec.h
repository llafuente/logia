#pragma once

#include "logia/ast/types/ref.h"
#include "logia/ast/operators.h"

namespace logia::AST
{
    struct Vec : public Ref
    {
    public:
        size_t size = 0;

        Vec(TypeDecl *pointee, size_t size = 0);
        ~Vec();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };
}