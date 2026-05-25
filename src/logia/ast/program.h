#pragma once

#include <vector>

#include "logia/ast/node.h"
#include "logia/ast/block.h"

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

    /// @brief Root of the AST, contains all the top level declarations, statements and imports
    class Program : public Block
    {
    public:
        /// @brief Intrinsics are functions that are implemented directly in llvm ir
        /// @remarks Use a property to hide intrinsics tree from user
        Scope *intrinsics = nullptr;

        /// @brief The entry point file
        const char *entry_point_file = nullptr;
        /// @brief The entry point file contents, used for error reporting and debugging
        const char *file_contents = nullptr;

        Program(antlr4::ParserRuleContext *rule, const char *entry_point_file, const char *file_contents);

        std::string to_string() override;

        /// @brief Program do not have a type, nullptr is returned
        /// @return nullptr
        Type *get_type() override;

        /// @brief Given an LLVM type retrieves the logia counterpart
        /// @remarks Should be used only with primitives because LLVM types are unique but logia's not
        /// @return
        Type *get_ast_type(llvm::Type *type);

        /// @brief Adds an intrinsic function to the program
        /// @param name The name of the intrinsic function
        /// @param return_type The return type of the intrinsic function
        /// @param arguments The argument types of the intrinsic function
        void add_intrinsic(const char *name, Type *return_type, std::vector<Type *> arguments);

        /// @brief Codegen core module (primitives) to be able to add intrinsics later
        /// @param backend
        void codegen_primitives(logia::Backend *backend);

        void pre_codegen(logia::Backend *backend) override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };
}