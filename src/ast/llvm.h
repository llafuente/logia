#pragma once

#include "llvm/IR/BasicBlock.h"

//
// llvm utils - shortcuts
//

/// @brief Retrives if the given block has a valid terminator
/// @remarks only: ReturnInst, BranchInst and UnreachableInst are valid atm for logia.
/// @param block
/// @return
bool ast_llvm_block_has_terminator(llvm::BasicBlock *block);