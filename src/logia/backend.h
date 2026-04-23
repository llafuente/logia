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

#include "antlr4-runtime.h"

namespace logia
{
    namespace AST
    {
        struct Program;
    }
}

namespace logia
{
    /// @brief Backend is the main entry point for code generation, it holds the LLVM context, module, builder and other data necesary for code generation
    struct Backend
    {
    public:
        /// @brief Enable debug mode -> output DWARF
        bool debug;
        /// @brief Enable code coverage -> output llvm-cov profdata
        bool coverage;
        /**
         * llvm context
         */
        llvm::LLVMContext context;

        // NOTE module is unique_ptr because parseIRFile return it
        /// @brief The main LLVM module
        std::unique_ptr<llvm::Module> module = nullptr;

        /// @brief Used for generating LLVM instructions (globally)
        llvm::IRBuilder<> *builder = nullptr;

        /// @brief Debug scope stack
        std::vector<llvm::DIScope *> dscopes = {};

        /// @brief Debug information builder
        llvm::DIBuilder *dbuilder = nullptr;

        /// @brief Debug information file
        llvm::DIFile *dfile = nullptr;

        /// @brief Debug information compilation unit
        llvm::DICompileUnit *dcompilation_unit = nullptr;

        /// @brief Current LLVM JIT session
        std::unique_ptr<llvm::orc::ExecutionSession> session;

        ::logia::AST::Program *program = nullptr;
        /**
         * Initialize LLVM
         */
        Backend(bool debug, bool coverage);
        /**
         *
         */
        ~Backend();

        /// @brief Loads intrinsics from file, and store them into their own module
        void load_intrinsics(char *filepath = (char *)"intrinsics/intrinsics.ll");

        /// @brief Adds an intrinsic to the current module, this is used to expose intrinsics to comptime/jit
        void add_intrinsic(void *fn_ref, char *fn_name);

        /// @brief Creates a TargetMachine with current host configuration
        llvm::Expected<llvm::TargetMachine *> createHostTargetMachine(llvm::Triple triple);

        /// @brief Applys LLVM optimizers to current module using  API PassBuilder
        void applyLLVMOptimizers();

        /// @brief Generates LLVM IR file
        bool emitTargetLLVMIR(std::string fileName);

        /// @brief Generates object file
        bool emitTargetObjectFile(std::string fileName);

        /// @brief Generates assembly file
        bool emitTargetAssemblyFile(std::string fileName);

        /// @brief Generates module binary and save it to file, this is the final step for a standalone executable
        bool emitTargetExecutable(std::string fileName);

        /// @brief Runs module main function into current process
        int run_jit(const char *fn_name);

        /// @brief Sets debug information for the current context and scope
        /// @param context The parser rule context
        /// @param scope The debug scope, if nullptr is provided, the current scope will be used
        void set_debug_information(antlr4::ParserRuleContext *context, llvm::DIScope *scope = nullptr);

    private:
        void __finalize_module();
    };
}