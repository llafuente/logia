#include "logia/ast/types/templatedef.h"

#include "logia/ast/types/typedef.h"

#include "llvm/IR/Type.h"              // Type
#include "llvm/IR/DebugInfoMetadata.h" // dwarf

namespace logia::AST
{
    TemplateDef::TemplateDef(location loc) : TypeDef(loc)
    {
        // We cannot store multiple types inside a single node!
        // we just skip the has test type
        this->has_type = false;
    }

    TemplateDef::~TemplateDef() {}

    std::string TemplateDef::to_string()
    {
        return std::format("TemplateDef {}{}", this->get_repr(), Type::to_string());
    }

    std::string TemplateDef::get_repr()
    {
        std::string list = "";

        TypeDef *td;
        for (const auto &ptr : this->children)
        {
            if (ptr->try_cast(&td))
            {
                if (!list.empty())
                {
                    list += ", ";
                }
                list += td->get_repr();
            }
        }

        return std::format("<{}>", list);
    }

    void TemplateDef::on_after_attach() {}

    void TemplateDef::validate() {}

    bool TemplateDef::type_inference(size_t pass_id)
    {
        // Makes sure all my children are beyong current pass and continue!
        // Typedef itself will handle my children to implement the template
        for (const auto &node : this->children)
        {
            if (node->type_inference_pass_id < pass_id)
            {
                return false;
            }
        }

        return true;
    }
}