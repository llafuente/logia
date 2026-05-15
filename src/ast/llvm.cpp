#include "ast/llvm.h"
#include "utils.h"
#include <iostream>
#include "logia/compiler_error.h"

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
            // GEP is used on structs and arrays, each index can be either one
            auto element_ptr = llvm::dyn_cast<llvm::GetElementPtrInst>(value);
            auto source_ty = element_ptr->getSourceElementType();

            DEBUG() << "GEP has " << element_ptr->getNumIndices() << " indices:\n";

            // auto ty = llvm::dyn_cast<llvm::StructType>(source_ty);
            llvm::Type *ty = source_ty; // final type, resolve index by index!

            auto IdxIt = element_ptr->idx_begin();

            // ignore the first one, it's the pointer deref
            unsigned idxNum = 1;
            ++IdxIt;

            for (; IdxIt != element_ptr->idx_end(); ++IdxIt, ++idxNum)
            {
                if (ty->isStructTy())
                {
                    if (auto *CI = llvm::dyn_cast<llvm::ConstantInt>(IdxIt->get()))
                    {
                        DEBUG() << "  Index " << idxNum << ": " << CI->getSExtValue() << "\n";
                        ty = ty->getStructElementType(CI->getSExtValue());
                    }
                    else
                    {
                        throw_compiler_error("Expected GEP indexes to be constant for struct types");
                    }
                }
                else if (ty->isArrayTy())
                {
                    throw_compiler_error("TODO");
                }
            }

            DEBUG() << "final type" << llvm_type_to_string(ty);

            value = backend->builder->CreateLoad(ty, value);
        }
        // the rest don't need it!

        return value;
    }
}
