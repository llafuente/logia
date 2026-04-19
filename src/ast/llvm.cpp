#include "ast/llvm.h"
#include "utils.h"
#include <iostream>

#include "llvm/IR/Instructions.h"

bool ast_llvm_block_has_terminator(llvm::BasicBlock *block)
{
    if (block->size() == 0)
    {
        return false;
    }

    auto terminator = block->getTerminator();
    if (terminator) {
        if (llvm::isa<llvm::ReturnInst>(terminator))
        {
            DEBUG() << "then_block has terminator return" << std::endl;
            return true;
        }
        else if (llvm::isa<llvm::BranchInst>(terminator)) {
            DEBUG() << "then_block has inconditional branch" << std::endl;
            return true;
        }
        else if (llvm::isa<llvm::UnreachableInst>(terminator))
        {
            DEBUG() << "then_block has terminator unreachable" << std::endl;
            return true;
        }
    }

    return false;
}