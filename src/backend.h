#pragma once

#include <map>
#include <iostream>
#include <system_error>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Support/CodeGen.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"


#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

namespace logia
{
    /**
     * 
     */
    class Backend
    {
    public:
        llvm::LLVMContext context;
        std::unique_ptr<llvm::Module> module = nullptr;
        // Used for generating LLVM instructions;
        llvm::IRBuilder<> *builder;
        /*
        std::unique_ptr<llvm::ModuleAnalysisManager> ModuleAnalysis;
        std::unique_ptr<llvm::FunctionAnalysisManager> FunctionAnalysis;
        std::unique_ptr<llvm::LoopAnalysisManager> LoopAnalysis;
        std::unique_ptr<llvm::CGSCCAnalysisManager> CGSCCAnalysis;
        std::unique_ptr<llvm::PassBuilder> PassBuilder;
        */
        /**
         * Initialize LLVM
         */
        Backend();
        /**
         * 
         */
        ~Backend();


        /**
         * Creates a TargetMachine with current host configuration
         */
        llvm::Expected<llvm::TargetMachine*> createHostTargetMachine(llvm::Triple triple);
        /**
         * Applys LLVM optimizers to current module using  API PassBuilder
         */
        void applyLLVMOptimizers();
        /**
         * Generates object file
         */
        bool emitTargetLLVMIR(std::string fileName);
        /**
         * Generates object file
         */
        bool emitTargetObjectFile(std::string fileName);
        /**
         * Generates assembly file
         */
        bool emitTargetAssemblyFile(std::string fileName);
        /**
         * Generates module binary
         */
        bool emitTargetExecutable(std::string fileName);
        /**
         * Runs module main function into current process
         */
        void run_jit();
    };
}