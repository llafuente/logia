#include "ast/program.h"

#include "ast/node.h"
#include "ast/type.h"
#include "ast/traverse.h"

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

    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C, const char *entry_point_file)
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

        Type *f16 = new Type(nullptr, Primitives::F16_TY);
        f16->Float.bits = 16;
        f16->ir_type = llvm::Type::getHalfTy(C);
        f16->parent_node = body;

        body->scope[(char *)"λf16"] = f16;

        Type *f32 = new Type(nullptr, Primitives::F32_TY);
        f32->Float.bits = 32;
        f32->ir_type = llvm::Type::getFloatTy(C);
        f32->parent_node = body;

        body->scope[(char *)"λf32"] = f32;

        Type *f64 = new Type(nullptr, Primitives::F64_TY);
        f64->Float.bits = 64;
        f64->ir_type = llvm::Type::getDoubleTy(C);
        f64->parent_node = body;

        body->scope[(char *)"λf64"] = f64;

        Type *f128 = new Type(nullptr, Primitives::F128_TY);
        f128->Float.bits = 64;
        f128->ir_type = llvm::Type::getFP128Ty(C);
        f128->parent_node = body;

        body->scope[(char *)"λf128"] = f128;

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

        ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_add_i64_i64"), i64);
        ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_mul_i64_i64"), i64);
        ast_create_instrinsic(body, ast_create_identifier("logia_intrinsics_bin_lt_i64_i64"), i1);

        return body;
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
