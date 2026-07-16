#include "logia/ast/types/templatedef.h"

#include "logia/ast/types/typedef.h"

#include "llvm/IR/Type.h"              // Type
#include "llvm/IR/DebugInfoMetadata.h" // dwarf

namespace logia::AST
{
    TemplateDef::TemplateDef(location loc) : Type(loc) {}
    TemplateDef::~TemplateDef() {}

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
}