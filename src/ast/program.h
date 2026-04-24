#pragma once

#include <vector>

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
        const char *entry_point_file;

        Program(antlr4::ParserRuleContext *rule, const char *entry_point_file);

        std::string to_string() override;

        /// @brief Program do not have a type, nullptr is returned
        /// @return
        Type *get_type() override;

        void pre_codegen(logia::Backend *backend) override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    /// @brief Creates a minimal logia program
    /// @details Primitives and intrinsics included
    /// @remarks Does not include core library
    LOGIA_API LOGIA_LEND Program *ast_create_program(llvm::LLVMContext &C, const char *entry_point_file);
}