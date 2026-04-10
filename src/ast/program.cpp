#include "ast/program.h"

#include "ast/node.h"
#include "ast/type.h"
#include "ast/traverse.h"

namespace logia::AST
{
    Program::Program(antlr4::ParserRuleContext *rule) : Block(rule)
    {
        this->type = (ast_types)(ast_types::PROGRAM | ast_types::BODY);
    }

    Type *Program::get_type()
    {
        return nullptr;
    }

    std::string Program::to_string()
    {
        return std::format("Program.{}", Block::to_string());
    }

    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C)
    {
        auto body = new Program(nullptr);

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly

        body->push_child(new Integer(true, 8));
        body->push_child(new Integer(true, 16));
        body->push_child(new Integer(true, 32));
        body->push_child(new Integer(true, 64));
        body->push_child(new Integer(true, 128));

        body->push_child(new Integer(false, 8));
        body->push_child(new Integer(false, 16));
        body->push_child(new Integer(false, 32));
        body->push_child(new Integer(false, 64));
        body->push_child(new Integer(false, 128));

        Type *f16 = new Type(nullptr, Primitives::F16_TY);
        f16->Float.bits = 16;
        f16->llvm_type = llvm::Type::getHalfTy(C);
        f16->parent_node = body;

        body->scope[(char *)"λf16"] = f16;

        Type *f32 = new Type(nullptr, Primitives::F32_TY);
        f32->Float.bits = 32;
        f32->llvm_type = llvm::Type::getFloatTy(C);
        f32->parent_node = body;

        body->scope[(char *)"λf32"] = f32;

        Type *f64 = new Type(nullptr, Primitives::F64_TY);
        f64->Float.bits = 64;
        f64->llvm_type = llvm::Type::getDoubleTy(C);
        f64->parent_node = body;

        body->scope[(char *)"λf64"] = f64;

        Type *f128 = new Type(nullptr, Primitives::F128_TY);
        f128->Float.bits = 64;
        f128->llvm_type = llvm::Type::getFP128Ty(C);
        f128->parent_node = body;

        body->scope[(char *)"λf128"] = f128;

        Type *lvoid = new Type(nullptr, Primitives::VOID_TY);
        lvoid->llvm_type = llvm::Type::getVoidTy(C);
        lvoid->parent_node = body;

        body->scope[(char *)"λvoid"] = lvoid;

        // TODO study opaque pointers, while seem what we need
        Type *ptr = new Type(nullptr, Primitives::PTR_TY);
        // opaque pointer, do not store information about pointee
        ptr->llvm_type = llvm::PointerType::get(C, 0);
        body->scope[(char *)"λptr"] = ptr;
        ptr->parent_node = body;

        return body;
    }
}
