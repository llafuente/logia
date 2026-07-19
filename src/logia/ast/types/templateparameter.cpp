#include "logia/ast/types/templateparameter.h"

#include "logia/ast/identifier.h"
#include "logia/ast/scope.h"
#include "utils.h"

namespace logia::AST
{
    const TemplateConstrains TemplateConstrains::IS = TemplateConstrains((uint32_t)0x001);
    const TemplateConstrains TemplateConstrains::EXTENDS = TemplateConstrains((uint32_t)0x002);
    const TemplateConstrains TemplateConstrains::IMPLEMENTS = TemplateConstrains((uint32_t)0x003);

    TemplateParameter::TemplateParameter(location loc, Identifier *ident) : TypeDecl(loc, Primitives::ANY_TY), name(ident)
    {
    }

    TemplateParameter::~TemplateParameter() {}

    std::string TemplateParameter::to_string()
    {
        return std::format("TemplateParameter {}{}", this->name->identifier, TypeDecl::to_string());
    }

    std::string TemplateParameter::get_repr()
    {
        return this->name->identifier;
    }

    void TemplateParameter::on_after_attach()
    {
        logia::AST::scope_set(this, this->name->identifier, this, true);
    }

    void TemplateParameter::validate() {}

    void TemplateParameter::add_constraints(TemplateConstrains constraint, TypeDef *td)
    {
        this->constraints.push_back({constraint, td});
        this->push_child(td); // needed to support type_inference -> typedecl
    }

    bool TemplateParameter::match(TypeDef *td)
    {
        auto tyd = td->get_type_decl();

        if (this->constraints.size() > 0)
        {
            for (auto constraint : this->constraints)
            {
                auto c = std::get<0>(constraint);
                auto td2 = std::get<1>(constraint);
                if (c == TemplateConstrains::IS)
                {
                    auto tyd2 = td2->get_type_decl();

                    if (tyd != tyd2)
                        return false;
                }
                else if (c == TemplateConstrains::EXTENDS)
                {
                    throw_compiler_error("to-do");
                }
                else if (c == TemplateConstrains::IMPLEMENTS)
                {
                    throw_compiler_error("to-do");
                }
                else
                {
                    throw_compiler_error("unreachable");
                }
            }
        }

        return true;
    }

}