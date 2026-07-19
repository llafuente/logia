#pragma once

#include "logia/ast/types/typedecl.h"
#include "logia/ast/types/typedef.h"

#include "utils.h"

namespace logia::AST
{
    class TemplateConstrains : public EnumPure<TemplateConstrains>
    {
    public:
        static const TemplateConstrains IS;
        static const TemplateConstrains EXTENDS;
        static const TemplateConstrains IMPLEMENTS;

        // type_compatibility(uint32_t v) : value(v) {}
        constexpr TemplateConstrains() = default;
        constexpr TemplateConstrains(uint32_t v) : EnumPure(v) {}

        std::string to_string()
        {
            if (this->is(TemplateConstrains::IS))
            {
                return "is";
            }
            if (this->is(TemplateConstrains::EXTENDS))
            {
                return "EXTENDS";
            }
            if (this->is(TemplateConstrains::IMPLEMENTS))
            {
                return "IMPLEMENTS";
            }

            throw_compiler_error("unreahable");
        }
    };

    struct TemplateParameter : TypeDecl
    {
        Identifier *name;

        std::vector<std::tuple<TemplateConstrains, TypeDef *>> constraints = {};

        TemplateParameter(location loc, Identifier *ident);
        ~TemplateParameter();

        std::string to_string() override;

        std::string get_repr() override;

        void on_after_attach() override;

        void validate() override;

        void add_constraints(TemplateConstrains constraint, TypeDef *td);

        bool match(TypeDef *td);
    };
}