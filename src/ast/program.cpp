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
    }

    Type *Program::get_type()
    {
        return nullptr;
    }

    std::string Program::to_string()
    {
        return std::format("Program.{}", Block::to_string());
    }

    LOGIA_API Program *ast_create_program(Backend *backend, const char *entry_point_file)
    {
        auto body = new Program(nullptr, entry_point_file);

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly
        body->push_child(new Integer(true, 1));
        auto i1 = body->children[body->children.size() - 1]->as<Type>();
        body->push_child(new Integer(true, 8));
        body->push_child(new Integer(true, 16));
        body->push_child(new Integer(true, 32));
        body->push_child(new Integer(true, 64));
        auto i64 = body->children[body->children.size() - 1]->as<Type>();
        body->push_child(new Integer(true, 128));

        body->push_child(new Integer(false, 8));
        body->push_child(new Integer(false, 16));
        body->push_child(new Integer(false, 32));
        body->push_child(new Integer(false, 64));
        body->push_child(new Integer(false, 128));

        body->push_child(new Void());
        body->push_child(new Pointer());

        body->push_child(new Float(16));
        body->push_child(new Float(32));
        body->push_child(new Float(64));
        body->push_child(new Float(128));

        // int is an alias of i64
        // float is an alias of f64
        body->scope[(char *)"int"] = body->scope[(char *)"λi64"];
        body->scope[(char *)"float"] = body->scope[(char *)"λf64"];
        body->scope[(char *)"bool"] = body->scope[(char *)"λi1"];

        // TODO i64 is in fact a struct to support properties
        // but atm it's just an alias here!
        // struct of all types
        body->scope[(char *)"i64"] = body->scope[(char *)"λi64"];

        // alias
        body->scope[(char *)"void"] = body->scope[(char *)"λvoid"];
        body->scope[(char *)"ptr"] = body->scope[(char *)"λptr"];

        // generate all primitives so we have a logia type to llvm and reverse!
        for (Node *node : body->children)
        {
            node->codegen(backend);
        }

        // ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_add_i64_i64"), i64);
        // ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_mul_i64_i64"), i64);
        // ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_lt_i64_i64"), i1);

        return body;
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
                //DEBUG() << ltype->to_string() << std::endl;
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
