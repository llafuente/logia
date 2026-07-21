#pragma once

#include <string>

#include "llvm/Support/Alignment.h"

namespace llvm
{
    class BasicBlock; // #include "llvm/IR/BasicBlock.h"
    class Value;
    class Type;
}
namespace logia
{
    struct Backend; // #include "logia/backend.h"
}

//
// llvm utils - shortcuts
//
namespace logia::AST
{
    /// @brief Retrives if the given block has a valid terminator
    /// @remarks only: ReturnInst, BranchInst and UnreachableInst are valid atm for logia.
    /// @param block
    /// @return
    bool ast_llvm_block_has_terminator(llvm::BasicBlock *block);

    /// @brief Loads given value if necessary
    /// @remarks AllocaInst and GetElementPtrInst
    /// @param block
    /// @return
    llvm::Value *llvm_load_if_required(llvm::Value *value, logia::Backend *backend);

    /// @brief Retrieves alignament of given value
    /// @param v GlobalVariable, AllocaInst, LoadInst and StoreInst
    /// @return alignament
    llvm::Align llvm_get_alignament(const llvm::Value *V);

    /// @brief Convert given type to its string representation
    /// @param v
    /// @return text
    std::string llvm_type_to_string(llvm::Type *ty);
}