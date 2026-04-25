#include "ast/program.h"

#include "ast/node.h"
#include "ast/type.h"
#include "ast/traverse.h"
#include "ast/expr.h"

#include "logia/backend.h"

namespace logia::AST
{
    Program::Program(antlr4::ParserRuleContext *rule, const char *entry_point_file) : Block(rule, ast_create_identifier("program")), entry_point_file(entry_point_file)
    {
        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly
        this->push_child(new Integer(true, 1));
        auto i1 = this->children[this->children.size() - 1]->as<Type>();
        this->push_child(new Integer(true, 8));
        this->push_child(new Integer(true, 16));
        this->push_child(new Integer(true, 32));
        this->push_child(new Integer(true, 64));
        auto i64 = this->children[this->children.size() - 1]->as<Type>();
        this->push_child(new Integer(true, 128));

        this->push_child(new Integer(false, 8));
        this->push_child(new Integer(false, 16));
        this->push_child(new Integer(false, 32));
        this->push_child(new Integer(false, 64));
        this->push_child(new Integer(false, 128));

        this->push_child(new Void());
        this->push_child(new Pointer());

        this->push_child(new Float(16));
        this->push_child(new Float(32));
        this->push_child(new Float(64));
        this->push_child(new Float(128));

        // int is an alias of i64
        // float is an alias of f64
        this->scope[(char *)"int"] = this->scope[(char *)"λi64"];
        this->scope[(char *)"float"] = this->scope[(char *)"λf64"];
        this->scope[(char *)"bool"] = this->scope[(char *)"λi1"];

        // TODO i64 is in fact a struct to support properties
        // but atm it's just an alias here!
        // struct of all types
        this->scope[(char *)"i64"] = this->scope[(char *)"λi64"];

        // alias
        this->scope[(char *)"void"] = this->scope[(char *)"λvoid"];
        this->scope[(char *)"ptr"] = this->scope[(char *)"λptr"];

        this->primitive_count = this->children.size();
    }

    void Program::codegen_primitives(logia::Backend *backend)
    {
        for (auto i = 0; i < this->primitive_count; ++i)
        {
            this->children[i]->codegen(backend);
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
        this->push_child(f);
        DEBUG() << f->to_string() << std::endl;
    }

    Type *Program::get_ast_type(llvm::Type *type)
    {
        for (Node *node : this->children)
        {
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

        throw std::runtime_error(std::format("llvm type not found: {}", llvm_type_to_string(type)));
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

        // NOTE: overwrite - no override!
        // Block::post_codegen(backend);

        this->codegen_children(backend);

        this->cg_value = nullptr; // nobody need program return type!
        return Node::post_codegen(backend);
    }
}
