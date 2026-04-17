#include "ast/llvm.h"
#include "utils.h"
#include <iostream>

#include "llvm/IR/Instructions.h"

bool ast_llvm_block_has_terminator(llvm::BasicBlock *block)
{
    auto terminator = block->getTerminator();
    if (terminator && llvm::isa<llvm::ReturnInst>(terminator))
    {
        DEBUG() << "then_block has terminator return" << std::endl;
        return true;
    }
    else if (terminator && llvm::isa<llvm::UnreachableInst>(terminator))
    {
        DEBUG() << "then_block has terminator unreachable" << std::endl;
        return true;
    }

    return false;
}