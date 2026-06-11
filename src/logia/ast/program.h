#pragma once

#include "logia/ast/block.h"

namespace llvm
{
    // #include "llvm/IR/IRBuilder.h"
    class Type;
}

namespace logia::AST
{
    struct Type;
    struct Identifier;
    struct Intrinsic;

    // Design/Implementation notes
    // bootstraping need a little back and forth to make it work
    // A program do not need Backend information (until codegen), but a logia Program does (before)!
    // Program defines language primitives that will be codegen later, but intrinsics are defined at IR file
    // require LLVM Types to be able to go from LLVM IR to AST so we codegen_primitives before codegen
    // in the early stage. Not the end of the world, just leave the note to know, because this is atm,
    // the only node that requires backend early

    /// @brief Root of the AST, contains all the top level declarations, statements and imports
    struct Program : public Block
    {
    public:
        /// @brief Intrinsics are functions that are implemented directly in llvm ir
        /// @remarks Use a property to hide intrinsics tree from user
        Scope *intrinsics = nullptr;

        /// @brief The entry point file
        const char *entry_point_file = nullptr;
        /// @brief The entry point file contents, used for error reporting and debugging
        const char *file_contents = nullptr;

        Program(location loc, const char *entry_point_file, const char *file_contents);

        std::string to_string() override;

        /// @brief Program do not have a type, nullptr is returned
        /// @return nullptr
        Type *get_type() override;

        /// @brief Given an LLVM type retrieves the logia counterpart
        /// @remarks Should be used only with primitives because LLVM types are unique but logia's not
        /// @return
        Type *get_ast_type(llvm::Type *type);

        /// @brief Adds an intrinsic function to the program
        void add_intrinsic(Intrinsic *fn);

        /// @brief Codegen core module (primitives) to be able to add intrinsics later
        /// @param backend
        void codegen_primitives(logia::Backend *backend);

        void pre_codegen(logia::Backend *backend) override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };
}