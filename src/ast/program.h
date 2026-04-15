#pragma once

#include <vector>

#include "ast/types.h"
#include "ast/node.h"
#include "ast/block.h"

#include "utils.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

namespace logia::AST
{
    /// @brief Root of the AST, contains all the top level declarations and statements
    class Program : public Block
    {
    public:
        Program(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        Type *get_type() override;

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    /// @brief Creates a minimal logia program
    /// @details Primitives and intrinsics included
    /// @remarks Does not include core library
    LOGIA_API LOGIA_LEND Program *ast_create_program(llvm::LLVMContext &C);
}