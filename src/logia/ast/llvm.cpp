#include "logia/ast/llvm.h"

#include <format>
#include <iostream>

#include "utils.h"
#include "logia/log.h"
#include "logia/backend.h"
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
                LOG(DBG, "then_block has terminator return");
                return true;
            }
            else if (llvm::isa<llvm::BranchInst>(terminator))
            {
                LOG(DBG, "then_block has inconditional branch");
                return true;
            }
            else if (llvm::isa<llvm::UnreachableInst>(terminator))
            {
                LOG(DBG, "then_block has terminator unreachable");
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

            LOG(DBG, "GEP has {} indices", element_ptr->getNumIndices());

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
                        LOG(DBG, "Index {}:{}", idxNum, CI->getSExtValue());
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

            LOG(DBG, "final type {}", llvm_type_to_string(ty));

            value = backend->builder->CreateLoad(ty, value);
        }
        // the rest don't need it!

        return value;
    }

    llvm::Align llvm_get_alignament(const llvm::Value *V)
    {
        if (const auto *GV = llvm::dyn_cast<llvm::GlobalVariable>(V))
        {
            return GV->getAlign().value_or(llvm::Align(1)); // default to 1 if unset
        }
        else if (const auto *AI = llvm::dyn_cast<llvm::AllocaInst>(V))
        {
            return AI->getAlign();
        }
        else if (const auto *LI = llvm::dyn_cast<llvm::LoadInst>(V))
        {
            return LI->getAlign();
        }
        else if (const auto *SI = llvm::dyn_cast<llvm::StoreInst>(V))
        {
            return SI->getAlign();
        }
        else
        {
            throw_compiler_error("Unsupported value type for alignment.");
        }
    }

    std::string llvm_type_to_string(llvm::Type *ty)
    {
        std::string typeStr;
        llvm::raw_string_ostream rso(typeStr);
        ty->print(rso);
        rso.flush();
        return typeStr;
    }
}
