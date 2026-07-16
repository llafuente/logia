#pragma once

#include "logia/ast/types/type.h"

namespace logia::AST
{
    struct TemplateDef : Type
    {
        TemplateDef(location loc);
        ~TemplateDef();

        std::string get_repr() override;

        void on_after_attach() override;

        void validate() override;
    };
}