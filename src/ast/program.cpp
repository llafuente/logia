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
        return std::format("Program[{} statements] ({:p})", this->children.size(), static_cast<void *>(this));
    }

    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C)
    {
        auto body = new Program(nullptr);

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly

        Type *i8 = new Type(nullptr, Primitives::I8_TY);
        i8->Integer.bits = 8;
        i8->Integer.is_signed = true;
        i8->llvm_type = llvm::Type::getInt8Ty(C);
        i8->parent_node = body;

        body->scope[(char *)"λi8"] = i8;

        Type *i16 = new Type(nullptr, Primitives::I16_TY);
        i16->Integer.bits = 16;
        i16->Integer.is_signed = true;
        i16->llvm_type = llvm::Type::getInt16Ty(C);
        i16->parent_node = body;

        body->scope[(char *)"λi16"] = i16;

        Type *i32 = new Type(nullptr, Primitives::I32_TY);
        i32->Integer.bits = 32;
        i32->Integer.is_signed = true;
        i32->llvm_type = llvm::Type::getInt32Ty(C);
        i32->parent_node = body;

        body->scope[(char *)"λi32"] = i32;

        Type *i64 = new Type(nullptr, Primitives::I64_TY);
        i64->Integer.bits = 64;
        i64->Integer.is_signed = true;
        i64->llvm_type = llvm::Type::getInt64Ty(C);
        i64->parent_node = body;

        body->scope[(char *)"λi64"] = i64;

        Type *u8 = new Type(nullptr, Primitives::U8_TY);
        u8->Integer.bits = 8;
        u8->Integer.is_signed = true;
        u8->llvm_type = llvm::Type::getInt8Ty(C);
        u8->parent_node = body;

        body->scope[(char *)"λu8"] = u8;

        Type *u16 = new Type(nullptr, Primitives::U16_TY);
        u16->Integer.bits = 16;
        u16->Integer.is_signed = true;
        u16->llvm_type = llvm::Type::getInt16Ty(C);
        u16->parent_node = body;

        body->scope[(char *)"λu16"] = u16;

        Type *u32 = new Type(nullptr, Primitives::U32_TY);
        u32->Integer.bits = 32;
        u32->Integer.is_signed = true;
        u32->llvm_type = llvm::Type::getInt32Ty(C);
        u32->parent_node = body;

        body->scope[(char *)"λu32"] = u32;

        Type *u64 = new Type(nullptr, Primitives::U64_TY);
        u64->Integer.bits = 64;
        u64->Integer.is_signed = true;
        u64->llvm_type = llvm::Type::getInt64Ty(C);
        u64->parent_node = body;

        body->scope[(char *)"λu64"] = u64;

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
