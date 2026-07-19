#pragma once

#include "logia/ast/types/type.h"
#include "logia/ast/types/typedef.h"

namespace logia::AST
{
    struct TemplateDef : TypeDef
    {
        TemplateDef(location loc);
        ~TemplateDef();

        std::string to_string() override;

        std::string get_repr() override;

        void on_after_attach() override;

        void validate() override;

        bool type_inference(size_t pass_id) override;
    };
}