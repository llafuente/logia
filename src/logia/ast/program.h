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
    // A Program will define language primitives that could be codegen later, but intrinsics (that are defined at IR file)
    // require a conversion between LLVM Types and out type system (LLVM IR to AST) so we need to codegen primitives in the early stage.
    // Not the end of the world, just leave the note to know, because this is atm,
    // the only node that requires backend early
    // Also note that LLVM Types are unique while Logia don't (at least not at this moment!)

    struct File : public Scope
    {
        /// @brief The entry point file
        const char *entry_point_file = nullptr;
        /// @brief The entry point relative file
        const char *entry_point_reldir = nullptr;
        /// @brief The entry point file contents, used for error reporting and debugging
        const char *file_contents = nullptr;

        File(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents);
    };

    /// @brief Root of the AST, contains all the top level declarations, statements and imports
    struct Program : public File
    {
    public:
        /// @brief Intrinsics are functions that are implemented directly in llvm ir
        /// @remarks Use a property to hide intrinsics tree from user
        Scope *intrinsics = nullptr;

        Program(location loc, const char *entry_point_file, const char *entry_point_reldir, const char *file_contents);

        std::string to_string() override;

        /// @brief Program do not have a type, nullptr is returned
        /// @return nullptr
        Type *get_type() override;

        /// @brief Given an LLVM type retrieves the logia counterpart
        /// @remarks Should be used only with primitives because LLVM types are unique but logia's not
        /// @return
        TypeDecl *get_ast_type(llvm::Type *type);

        /// @brief Adds an intrinsic function to the program
        void add_intrinsic(Intrinsic *fn);

        /// @brief Codegen core module (primitives) to be able to add intrinsics later
        /// @param backend
        void codegen_primitives(logia::Backend *backend);

        /// @brief Codegen the program
        /// @details
        /// Codegen it's done in two phases
        /// pre_codegen: traverse all nodes inorder - create all types, prepare functions
        /// post_codegen: traverse a specific order defined by each node - generates llvm instructions / debug
        /// @param backend
        void codegen(logia::Backend *backend);

        /// @brief do nothing
        void pre_codegen(logia::Backend *backend) override;

        /// @brief codegen all children
        /// @param backend
        /// @return nullptr
        llvm::Value *post_codegen(logia::Backend *backend) override;

        /// @brief do nothing
        void on_after_attach() override;

        /// @brief validates and type inferences the program
        void semantic_analysis();
    };
}