#pragma once

#include "llvm/IR/BasicBlock.h"
#include "logia/backend.h"

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
}