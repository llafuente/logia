#pragma once

#include "logia/ast/scope.h"

namespace llvm
{
    //  #include "llvm/BasicBlock.h"
    class BasicBlock;
}

namespace logia::AST
{
    struct Identifier;

    /// @brief A block
    struct Block : public Scope
    {
    public:
        /// @brief Defines a named block -> (goto/break identifier)
        /// @llafuente This cannot be a children of the block because it's a mess, but should be one day.
        Identifier *name;

        /// @brief BasicBlock will be populated at pre_codegen and cached
        /// @remarks BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *ir_basicblock = nullptr;
        /// @brief Constructs a block with a given name
        /// @param rule The parser rule context
        /// @param name The identifier for the block, could be empty (no nullptr)
        Block(location loc, Identifier *name);

        /// @brief Retrieves the block name, used for goto and debugging
        Identifier *get_identifier();

        /// @brief Shortcut to name as cstring
        const char *get_name();

        std::string to_string() override;

        void pre_codegen(logia::Backend *codegen) override;
        /// @brief Inserts block into parent Function, add a jump if needed and codegen children
        llvm::Value *post_codegen(logia::Backend *backend) override;

        /// @brief Codegen children without touching block insertion and jump, used for continue block and if stmt codegen
        /// @param backend
        void post_codegen_children(logia::Backend *backend);

        /// @brief Notice that block do not have type, nullptr is returned
        Type *get_type() override;

        /// @brief Register block name at function scope
        void on_after_attach() override;
        /// @brief Checks block name is unique
        void validate() override;
    };
}