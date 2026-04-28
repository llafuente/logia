#include "ast/import.h"
#include "ast/block.h"
#include "ast/expr.h"
#include "ast/package.h"

#include "logia/frontend.h"

namespace logia::AST
{

    Import::Import(antlr4::ParserRuleContext *rule) : Scope(rule)
    {
    }

    std::string Import::to_string()
    {
        return std::format("Import");
    }

    void Import::set_package(std::vector<AST::Identifier *> package)
    {
        this->package = package;
    }
    void Import::set_import_into_scope()
    {
        this->is_import_into_scope = true;
    }
    void Import::set_import_all()
    {
        this->is_import_all_package = true;
    }
    void Import::set_import_list(std::vector<AST::Identifier *> list)
    {
        this->import_list = list;
    }

    Type *Import::get_type()
    {
        throw std::runtime_error("??");
    }
    void Import::set_scope_target(Scope *target)
    {
        this->target = target;
    }

    void Import::post_attach()
    {
        parse_result = logia_parse_package("core/primitives.logia");
        this->push_child(parse_result->ast_tree); // program attached? :P
        // parse
        // append to parent block everything!
        if (target == nullptr)
        {
            target = this->first_parent<Scope>();
        }

        if (this->is_import_into_scope)
        {
            this->scope_copy_all(target);
        }
    }
}