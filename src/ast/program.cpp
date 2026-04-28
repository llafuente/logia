#include "ast/program.h"

#include "ast/node.h"
#include "ast/type.h"
#include "ast/traverse.h"
#include "ast/expr.h"
#include "ast/import.h"

#include "logia/backend.h"

namespace logia::AST
{
    Program::Program(antlr4::ParserRuleContext *rule, const char *entry_point_file, const char *file_contents) : Block(rule, ast_create_identifier("program")), entry_point_file(entry_point_file), file_contents(file_contents)
    {
        intrinsics = new Scope(nullptr);
        this->push_child(intrinsics);
        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly
        intrinsics->push_child(new Integer(true, 1));
        intrinsics->push_child(new Integer(true, 8));
        intrinsics->push_child(new Integer(true, 16));
        intrinsics->push_child(new Integer(true, 32));
        intrinsics->push_child(new Integer(true, 64));
        intrinsics->push_child(new Integer(true, 128));

        intrinsics->push_child(new Integer(false, 8));
        intrinsics->push_child(new Integer(false, 16));
        intrinsics->push_child(new Integer(false, 32));
        intrinsics->push_child(new Integer(false, 64));
        intrinsics->push_child(new Integer(false, 128));

        intrinsics->push_child(new Void());
        intrinsics->push_child(new Pointer());

        intrinsics->push_child(new Float(16));
        intrinsics->push_child(new Float(32));
        intrinsics->push_child(new Float(64));
        intrinsics->push_child(new Float(128));

        auto imp = new Import(nullptr);
        imp->set_import_into_scope();
        imp->set_package({new Identifier(nullptr, "core"), new Identifier(nullptr, "primitives")});
        imp->set_scope_target(this);

        intrinsics->push_child(imp);

        intrinsics->scope_copy_all(this);

        this->children.pop_back(); // safe to remove now

        DEBUG() << intrinsics->to_string_tree() << std::endl;

        // alias
        this->scope[(char *)"int"] = this->scope[(char *)"λi64"];
        this->scope[(char *)"float"] = this->scope[(char *)"λf64"];
        this->scope[(char *)"bool"] = this->scope[(char *)"λi1"];
        this->scope[(char *)"void"] = this->scope[(char *)"λvoid"];
        this->scope[(char *)"ptr"] = this->scope[(char *)"λptr"];
    }

    void Program::codegen_primitives(logia::Backend *backend)
    {
        for (const auto it : intrinsics->children)
        {
            it->codegen(backend);
        }
    }

    Type *Program::get_type()
    {
        return nullptr;
    }

    std::string Program::to_string()
    {
        return std::format("Program.{}", Block::to_string());
    }

    void Program::add_intrinsic(const char *name, Type *return_type, std::vector<Type *> arguments)
    {
        auto f = new Function(nullptr, new Identifier(nullptr, name), return_type, true);
        for (auto t : arguments)
        {
            f->add_param(new FunctionParameter(new Identifier(nullptr, ""), t, nullptr));
        }
        intrinsics->push_child(f);
        DEBUG() << f->to_string() << std::endl;
    }

    Type *Program::get_ast_type(llvm::Type *type)
    {
        // search in the scope, not in children
        for (const auto &it : scope)
        {
            auto node = it.second;
            if (node->is<Type>())
            {
                auto ltype = node->as<Type>();
                // DEBUG() << ltype->to_string() << std::endl;
                if (ltype->ir_type == type)
                {
                    return ltype;
                }
            }
        }

        throw_compiler_error(std::format("llvm type not found: {}", llvm_type_to_string(type)));
    }

    void Program::post_attach()
    {
        // do nothing, parentBody should be empty
    }

    void Program::pre_codegen(logia::Backend *backend)
    {
        if (!is_typed)
        {
            is_typed = true;
            this->type_inference();
        }
    }

    llvm::Value *Program::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;

        this->intrinsics->codegen(backend); // forward, it's hidden from tree

        // NOTE: overwrite - no override!
        // Block::post_codegen(backend);
        this->codegen_children(backend);

        this->cg_value = nullptr; // nobody need program return type!
        return Node::post_codegen(backend);
    }
}
