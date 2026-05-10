#include "ast/llvm.h"
#include "utils.h"
#include <iostream>

#include "llvm/IR/Instructions.h"

namespace logia::AST
{

    bool ast_llvm_block_has_terminator(llvm::BasicBlock *block)
    {
        if (block->size() == 0)
        {
            return false;
        }

        auto terminator = block->getTerminator();
        if (terminator)
        {
            if (llvm::isa<llvm::ReturnInst>(terminator))
            {
                DEBUG() << "then_block has terminator return" << std::endl;
                return true;
            }
            else if (llvm::isa<llvm::BranchInst>(terminator))
            {
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

    llvm::Value *llvm_load_if_required(llvm::Value *value, logia::Backend *backend)
    {
        if (llvm::isa<llvm::AllocaInst>(value))
        {
            auto alloca = llvm::dyn_cast<llvm::AllocaInst>(value);
            value = backend->builder->CreateLoad(alloca->getAllocatedType(), value);
        }
        else if (llvm::isa<llvm::GetElementPtrInst>(value))
        {
            auto element_ptr = llvm::dyn_cast<llvm::GetElementPtrInst>(value);
            // source struct <-- element_ptr->getSourceElementType()
            // getType() -> ptr, not really!

            // TODO how do we know the target type here!!!
            value = backend->builder->CreateLoad(element_ptr->getSourceElementType()->getStructElementType(0), value);
        }
        return value;
    }
}