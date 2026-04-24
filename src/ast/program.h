#pragma once

#include <vector>

#include "ast/node.h"
#include "ast/block.h"

#include "utils.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
namespace logia
{
    struct Backend;
}

namespace logia::AST
{
    struct Type;
    struct Identifier;

    // Design
    // A program do not need Backend information, but a logia Program does!
    // When creating a program we fill primitives and a bit later intrinsics,
    // those instrinsics need a llvm type to logia type conversion and that require
    // backend in the early stage. Not the end of the world, just leave the note to know

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

        /// @brief Given an LLVM type retrieves the logia counterpart
        /// @remarks Should be used only with primitives because LLVM types are unique but logia don't
        /// @return
        Type *get_ast_type(llvm::Type *type);

        void add_intrinsic(const char *name, Type *return_type, std::vector<Type *> arguments);

        void pre_codegen(logia::Backend *backend) override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    /// @brief Creates a minimal logia program
    /// @details Primitives and intrinsics included
    /// @remarks Does not include core library
    LOGIA_API LOGIA_LEND Program *ast_create_program(Backend *backend, const char *entry_point_file);
}