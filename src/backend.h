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
#include "llvm/ExecutionEngine/Orc/Core.h"

namespace logia
{
    /**
     * Data necesary to move from AST to IR/ASM/Binary/JIT
     */
    struct Backend
    {
    public:
        /**
         * llvm context
         */
        llvm::LLVMContext context;
        // NOTE module is unique_ptr because parseIRFile return it
        /**
         * llvm module (global variables, functions, libraries)
         */
        std::unique_ptr<llvm::Module> module = nullptr;
        /**
         * Used for generating LLVM instructions (globally)
         * 
         * Overriden in each function/block scope
         */
        llvm::IRBuilder<> *builder;
        /**
         * Current LLVM JIT session
         */
        std::unique_ptr<llvm::orc::ExecutionSession> session;
        /**
         * Initialize LLVM
         */
        Backend();
        /**
         *
         */
        ~Backend();
        /**
         * Load intrinsics from file
         */
        void load_intrinsics(char* filepath = (char*)"intrinsics/intrinsics.ll");
        /**
         * Add intrinsics to current module
         */
        void add_intrinsic(void *fn_ref, char *fn_name);
        /**
         * Creates a TargetMachine with current host configuration
         */
        llvm::Expected<llvm::TargetMachine *> createHostTargetMachine(llvm::Triple triple);
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
         * Prepare LLVM to JIT
         * This is necesary to expose intrinsics to comptime
         */
        void prepare_jit();
        /**
         * Runs module main function into current process
         */
        int run_jit();
    };
}