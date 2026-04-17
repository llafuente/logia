#pragma once

#include "llvm/IR/BasicBlock.h"

bool ast_llvm_block_has_terminator(llvm::BasicBlock *block);