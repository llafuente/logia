#include "logia/ast/import.h"
#include "logia/ast/block.h"
#include "logia/ast/expr.h"
#include "logia/ast/package.h"
#include "logia/ast/identifier.h"

#include "logia/frontend.h"

namespace logia::AST
{

    Import::Import(location loc) : Scope(loc)
    {
    }

    std::string Import::to_string()
    {
        // join package by dot
        std::string package_name;
        for (auto id : package)
        {
            if (!package_name.empty())
            {
                package_name += ".";
            }
            package_name += id->identifier;
        }
        return std::format("Import[{}]{}", package_name, Scope::to_string());
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

    void Import::set_scope_target(Scope *target)
    {
        this->target = target;
    }

    void Import::validate()
    {
        if (this->is_import_all_package && !this->import_list.empty())
        {
            throw_semantic_error(this, LGERR_IMP001);
        }

        if (this->is_import_into_scope && !this->import_list.empty())
        {
            throw_semantic_error(this, LGERR_IMP002);
        }
#ifndef _DEBUG
        if (this->target->is<Scope>() == false)
        {
            throw_semantic_error(this, LGERR_IMP003);
        }
        // check import list contents are identifiers
        auto i = 0;
        for (auto id : this->import_list)
        {
            if (id->is<Identifier>() == false)
            {
                throw_semantic_error(this, std::format(LGERR_IMP004, i));
            }
            i++;
        }
        // check package list contents are identifiers
        auto j = 0;
        for (auto id : this->package)
        {
            if (id->is<Identifier>() == false)
            {
                throw_semantic_error(this, std::format(LGERR_IMP005, j));
            }
            j++;
        }
#endif
    }

    void Import::on_after_attach()
    {
        Scope::on_after_attach();

        // join package by slash -> file to import
        std::string package_path;
        for (auto id : package)
        {
            if (!package_path.empty())
            {
                package_path += "/";
            }
            package_path += id->identifier;
        }
        package_path += ".logia";

        // parse
        auto x = logia_parse_package(package_path.c_str());
        this->push_child(x); // program attached? :P

        // handle scope target
        if (target == nullptr)
        {
            target = this->first_parent<Scope>();
        }

        if (this->is_import_into_scope)
        {
            auto package = this->get_child<Package>(0);
            package->scope_copy_all(target);
        }
    }
}