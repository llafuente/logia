#include "logia/backend.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IRReader/IRReader.h"
#include <optional>

#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/SelfExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Error.h"

// debug
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/DebugInfoMetadata.h"

#include "llvm/Support/Compiler.h"
#include "llvm/ExecutionEngine/Orc/MaterializationUnit.h"
#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include <memory>

#include "utils.h"
#include "logia/frontend.h"
#include "logia/type_inference.h"
#include "ast/program.h"

// cross compile support ?
#define CODEGEN_NATIVE

namespace logia
{
    Backend::Backend(ParseResult *parse_result) : parse_result(parse_result), program(parse_result->ast_tree), debug(logia_config.debug)
    {
#ifdef CODEGEN_NATIVE
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
#else
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();
#endif
        DEBUG() << "()" << std::endl;
        DEBUG() << "List available targets: " << std::endl;
        for (auto &T : llvm::TargetRegistry::targets())
        {
            DEBUG() << "target: " << T.getBackendName() << " | " << T.getName() << std::endl;
        }

        // default empty module!
        this->module = std::make_unique<llvm::Module>("logia", context);

        // REVIEW use linker to have intrinsics + mainModule ?
        // linkInModule https://www.youtube.com/watch?v=h6HkwpE7UqM

        builder = new llvm::IRBuilder<>(context);

        if (logia_config.debug)
        {
            // Create a DIBuilder for debug info
            dbuilder = new llvm::DIBuilder(*this->module);

            // Create a file descriptor for the source file
            // TODO pass this variable, but it's not that easy atp, because could be an import, think about it!
            this->dfile = this->dbuilder->createFile(parse_result->entry_point_filename, parse_result->entry_point_reldir);

            // Create the compile unit
            this->dcompilation_unit = this->dbuilder->createCompileUnit(
                llvm::dwarf::DW_LANG_C, // Language
                this->dfile,            // Source file
                "logia",                // Producer
                false,                  // Optimized?
                "",                     // Flags
                0                       // Runtime version
            );

            // Enable debug info
            this->module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
            this->module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
        }

        auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
        if (!EPC)
        {
            llvm::errs() << EPC.takeError();
            throw std::exception("Error creating SelfExecutorProcessControl");
        }

        session = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));
        session->createBareJITDylib("<main>");
    }

    Backend::~Backend()
    {
        llvm::errs() << this->session->endSession();
    }

    void Backend::load_intrinsics(char *filepath)
    {
        // to found LLVM Type to logia type we need to codegen types first!
        this->program->codegen_primitives(this);

        llvm::SMDiagnostic diag;
        this->intrinsics_module = llvm::parseIRFile(filepath, diag, context);
        if (!this->intrinsics_module)
        {
            diag.print("intrinsics.ll", llvm::errs());
            throw std::exception("could not parse or read intrinsics.ll");
        }

        // Iterate over all functions in the module
        for (const llvm::Function &F : *this->intrinsics_module)
        {
            // REVIEW Skip functions without a body ? that imply libc or compiler libs ?
            if (!F.isDeclaration())
            {
                auto f_args = std::vector<AST::Type *>();
                f_args.reserve(F.arg_size());

                for (const llvm::Argument &argument : F.args())
                {
                    f_args.push_back(this->program->get_ast_type(argument.getType()));
                }

                auto f_ret_type = this->program->get_ast_type(F.getReturnType());
                this->program->add_intrinsic(F.getName().str().c_str(), f_ret_type, f_args);
            }
        }
    }

    void Backend::add_intrinsic(void *fn_ref, char *fn_name)
    {
        auto ptr = llvm::pointerToJITTargetAddress(fn_ref);
        llvm::orc::SymbolMap symbols(10);

        // std::pair<llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef> &&KV
        // llvm::orc::ExecutorSymbolDef ss({llvm::JITEvaluatedSymbol(ptr, llvm::JITSymbolFlags::Callable), llvm::JITSymbolFlags::Exported});
        llvm::orc::ExecutorSymbolDef ss({
            llvm::orc::ExecutorAddr(ptr),
            llvm::JITSymbolFlags::Exported,
        });
        symbols.insert({session->intern(fn_name), ss});

        auto dylib = session->getJITDylibByName("<main>");
        llvm::errs() << dylib->define(
            llvm::orc::absoluteSymbols(std::move(symbols)));
    }

    void Backend::applyLLVMOptimizers()
    {
        // references
        // https://discourse.llvm.org/t/beginner-help-for-llvm-passes/76600/10
        // https://github.com/apache/arrow/blob/34880c0428cac1a7b0d6de7d70b626cdf2e03ba0/cpp/src/gandiva/engine.cc#L454C16-L454C37
        // https://github.com/ferrandi/PandA-bambu/blob/c2ba6936ca2ed63137095fea0b630a1c66e20e63/etc/clang_plugin/plugin_dumpGimpleSSA.cpp#L463
        // RemoveUnusedFunctions

        //
        // function level
        //
        llvm::FunctionPassManager FPM;

        // Add transform passes.
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        // TheFPM->addPass(createInstSimplifyLegacyPass());
        FPM.addPass(llvm::InstSimplifyPass());

        //
        // module level
        // from: https://llvm.org/docs/NewPassManager.html
        // Create the analysis managers.
        // These must be declared in this order so that they are destroyed in the
        // correct order due to inter-analysis-manager references.
        // @llafuente do not use unique_ptr or use-after-free will raise!
        llvm::LoopAnalysisManager LAM;
        llvm::FunctionAnalysisManager FAM;
        llvm::CGSCCAnalysisManager CGAM;
        llvm::ModuleAnalysisManager MAM;

        // Create the new pass manager builder.
        // Take a look at the PassBuilder constructor parameters for more
        // customization, e.g. specifying a TargetMachine or various debugging
        // options.
        llvm::PassBuilder PB;

        // Register all the basic analyses with the managers.
        PB.registerModuleAnalyses(MAM);
        PB.registerCGSCCAnalyses(CGAM);
        PB.registerFunctionAnalyses(FAM);
        PB.registerLoopAnalyses(LAM);
        PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

        // Create the pass manager.
        // This one corresponds to a typical -O2 optimization pipeline.
        llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O0);
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));

        // Simplify the control flow graph (deleting unreachable blocks, etc).
        MPM.addPass(llvm::createModuleToFunctionPassAdaptor(llvm::SimplifyCFGPass(llvm::SimplifyCFGOptions()
                                                                                      .convertSwitchRangeToICmp(true)
                                                                                      .sinkCommonInsts(true)
                                                                                      .hoistCommonInsts(true))));

        // Optimize the IR!
        MPM.run(*module, MAM);
    };

    llvm::Expected<llvm::TargetMachine *> Backend::createHostTargetMachine(llvm::Triple triple)
    {
        DEBUG() << "createHostTargetMachine(" << triple.str() << ")" << std::endl;
        DEBUG() << "getDefaultExceptionHandling(" << (int)triple.getDefaultExceptionHandling() << ")" << std::endl;

        std::string err;
        auto target = llvm::TargetRegistry::lookupTarget(triple, err);

        // Target: x86_64-apple-darwin16.6.0
        if (!target)
        {
            llvm::errs() << err;
            // return llvm::Expected<llvm::TargetMachine*>(llvm::make_error<std::string>(err));
            return llvm::Expected<llvm::TargetMachine *>(llvm::make_error<llvm::StringError>(err, llvm::inconvertibleErrorCode()));
        }

        llvm::SubtargetFeatures features;
        llvm::StringMap<bool> hostFeatures = llvm::sys::getHostCPUFeatures();
        for (auto &feature : hostFeatures)
        {
            features.AddFeature(feature.first(), feature.second);
        }
        // auto Features = "";
        auto Features = features.getString();

        // auto CPU = "generic";
        auto CPU = llvm::sys::getHostCPUName();

        DEBUG() << "CPU = " << CPU.begin() << std::endl;
        DEBUG() << "Features = " << Features << std::endl;

        // defaults: https://reviews.llvm.org/D36241
        llvm::TargetOptions opt;
        auto reloc = std::optional<llvm::Reloc::Model>();
        auto code_model = std::optional<llvm::CodeModel::Model>();
        auto TM = target->createTargetMachine(triple, CPU, Features, opt, reloc, code_model, *llvm::CodeGenOpt::getLevel(0), false);
        return llvm::Expected<llvm::TargetMachine *>(TM);
    }

    bool generateFile(std::string fileName, llvm::CodeGenFileType FileType, llvm::Module *module, llvm::TargetMachine *TheTargetMachine)
    {
        DEBUG() << "(" << fileName << ")" << std::endl;

        std::error_code EC;
        llvm::raw_fd_ostream dest(fileName, EC, llvm::sys::fs::FileAccess::FA_Write);

        if (EC)
        {
            llvm::errs() << "Could not open file: " << EC.message();
            return false;
        }

        llvm::legacy::PassManager pass;

        if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
        {
            llvm::errs() << "TargetMachine can't emit a file of this type";
            return false;
        }

        pass.run(*module);
        dest.flush();

        return true;
    }

    void Backend::__finalize_module()
    {
        if (!program->is_codegen)
        {
            type_inference(this->program);
            this->program->codegen(this);
        }
        if (logia_config.debug)
        {
            // Finalize the debug info
            this->dbuilder->finalize();
        }
    }

    bool Backend::emitTargetLLVMIR(std::string fileName = "main.ll")
    {
        DEBUG() << "(" << fileName << ")" << std::endl;

        this->__finalize_module();

        std::error_code EC;
        llvm::raw_fd_ostream dest(fileName, EC, llvm::sys::fs::FileAccess::FA_Write);

        if (EC)
        {
            llvm::errs() << "Could not open file: " << EC.message();
            return false;
        }

        module->print(dest, nullptr);
        dest.flush();

        return true;
    }

    bool Backend::emitTargetObjectFile(std::string fileName = "main.o")
    {
        DEBUG() << "(" << fileName << ")" << std::endl;

        this->__finalize_module();

        // Initialize the target registry etc.
        auto triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());

        auto TheTargetMachine = this->createHostTargetMachine(triple);
        if (!TheTargetMachine)
        {
            llvm::errs() << TheTargetMachine.takeError() << "createHostTargetMachine failed";
            return false;
        }

        // Specify the target and data layout;
        module->setDataLayout((*TheTargetMachine)->createDataLayout());
        module->setTargetTriple(triple);

        return generateFile(fileName, llvm::CodeGenFileType::ObjectFile, &(*module), *TheTargetMachine);
    }

    bool Backend::emitTargetAssemblyFile(std::string fileName = "main.asm")
    {
        DEBUG() << "(" << fileName << ")" << std::endl;

        this->__finalize_module();

        // Initialize the target registry etc.
        auto triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());

        auto TheTargetMachine = this->createHostTargetMachine(triple);
        if (!TheTargetMachine)
        {
            llvm::errs() << TheTargetMachine.takeError() << "createHostTargetMachine failed";
            return false;
        }

        // Specify the target and data layout;
        module->setDataLayout((*TheTargetMachine)->createDataLayout());
        module->setTargetTriple(triple);

        return generateFile(fileName, llvm::CodeGenFileType::AssemblyFile, &(*module), *TheTargetMachine);
    }

    bool Backend::emitTargetExecutable(std::string fileName)
    {
        this->__finalize_module();

        throw std::runtime_error(__FUNCTION__ "todo");

        // & "C:\Program Files\LLVM\bin\clang.exe" .\xxx.obj -o xxx.exe
        return true;
    }

    int Backend::run_jit(const char *fn_name)
    {
        DEBUG() << "()" << std::endl;
        DEBUG() << std::endl
                << this->program->to_string_tree() << std::endl;

        this->__finalize_module();

        // create orc-jit
        // * execute in the current process -> session
        // * on default triple configuration
        // * add module
        // * find main
        // * execute

        auto triple = session->getExecutorProcessControl().getTargetTriple();

        auto ETM = this->createHostTargetMachine(triple);
        if (!ETM)
        {
            llvm::errs() << ETM.takeError();
            throw std::exception("Error creating createHostTargetMachine");
        }
        auto TM = *ETM;
        auto JIT_builder = llvm::orc::JITTargetMachineBuilder(triple);
        /*
                auto JIT_builder = llvm::orc::JITTargetMachineBuilder::detectHost();
                if (!JIT_builder)
                {
                    llvm::errs() << EPC.takeError();
                    throw std::exception("Error creating JIT_builder");
                }
        */
        /*
                DEBUG() << "CPU" << TM->getTargetCPU().str() << std::endl;
                DEBUG() << "Features" << TM->getTargetFeatureString().str() << std::endl;

                auto JIT_builder = llvm::orc::JITTargetMachineBuilder(TM->getTargetTriple())
                    .setCPU(TM->getTargetCPU().str())
                    .setFeatures(TM->getTargetFeatureString())
                    .setOptions(TM->Options)
                    .setRelocationModel(TM->getRelocationModel())
                    .setCodeModel(TM->getCodeModel())
                    .setCodeGenOptLevel(llvm::CodeGenOptLevel::None);

        */
        // .setCodeGenOptLevel(llvm::CodeGenOptLevel::Aggressive);

        auto dylib = session->getJITDylibByName("<main>");

        auto data_layout = JIT_builder.getDefaultDataLayoutForTarget();
        if (!data_layout)
        {
            llvm::errs() << data_layout.takeError();
            throw std::exception("Error creating DefaultDataLayoutForTarget");
        }
        auto mangle = llvm::orc::MangleAndInterner(*session, *data_layout);
        /*
  auto data_layout = llvm_module->getDataLayout();
  auto mangle = llvm::orc::MangleAndInterner(*session, data_layout);
  */

        auto objectLayer = llvm::orc::RTDyldObjectLinkingLayer(*session,
                                                               [](const llvm::MemoryBuffer &)
                                                               {
                                                                   return std::make_unique<llvm::SectionMemoryManager>();
                                                               });
        auto compileLayer = llvm::orc::IRCompileLayer(*session, objectLayer,
                                                      std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JIT_builder)));

        // session->createJITDylib()
        // auto &cdylib = session->createBareJITDylib("<compiler>");

        dylib->addGenerator(
            llvm::cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
                data_layout->getGlobalPrefix())));

        if (JIT_builder.getTargetTriple().isOSBinFormatCOFF())
        {
            objectLayer.setOverrideObjectFlagsWithResponsibilityFlags(true);
            objectLayer.setAutoClaimResponsibilityForObjectSymbols(true);
        }

        auto RT = dylib->getDefaultResourceTracker();
        llvm::orc::ThreadSafeContext context(std::make_unique<llvm::LLVMContext>());
        {
            llvm::orc::ThreadSafeModule TSM(std::move(this->module), context);
            auto err = compileLayer.add(RT, std::move(TSM));
            if (err)
            {
                llvm::errs() << err;
                throw std::exception("Error add module");
            }
        }

        if (this->intrinsics_module != nullptr)
        {
            llvm::orc::ThreadSafeModule TSM(std::move(this->intrinsics_module), context);
            auto err = compileLayer.add(RT, std::move(TSM));
            if (err)
            {
                llvm::errs() << err;
                throw std::exception("Error add module");
            }
        }

        // it's not needed to execute but this fails with ExceptionHandling missmatch between ASMInfo and triple
        /*
                auto machine = JIT_builder.createTargetMachine();
                if (!machine)
                {
                    llvm::errs() << EPC.takeError();
                    throw std::exception("Error createTargetMachine");
                }
        */
        // llvm::orc::SymbolStringPool SSP;
        // LLVM_ABI Expected<ExecutorSymbolDef> lookup(const JITDylibSearchOrder &SearchOrder, SymbolStringPtr Symbol, SymbolState RequiredState = SymbolState::Ready);
        // auto symbol = session->lookup(llvm::orc::JITDylibSearchOrder(), SSP.intern(func_name_name));
        auto mangle_name = mangle(fn_name);
        std::vector<llvm::orc::JITDylib *> SearchOrder = {dylib};
        auto symbol = session->lookup(SearchOrder, mangle_name);
        if (!symbol)
        {
            llvm::errs() << symbol.takeError();
            throw std::runtime_error(std::format("could not find function {}", fn_name));
        }

        // REVIEW how we support to execute different functions, we just have multiple "entries" ?
        using FuncType = int (*)();
        // auto *main_fn = (FuncType)(symbol->getAddress().getValue());
        auto *main_fn = (FuncType)(symbol->getAddress().getValue());
        int result = main_fn();
        if (result != 0)
        {
            LERROR() << "Main function run error:" << result << std::endl;
        }

        if (auto Err = session->endSession())
        {
            session->reportError(std::move(Err));
        }

        return result;
    }

    void Backend::set_debug_information(antlr4::ParserRuleContext *context, llvm::DIScope *scope)
    {
        if (!logia_config.debug)
        {
            return;
        }
        LOGIA_ASSERT(context);

        if (scope == nullptr)
        {
            LOGIA_ASSERT(this->dscopes.size());
            scope = this->dscopes[this->dscopes.size() - 1];
        }

        auto start_tk = context->getStart();
        auto end_tk = context->getStop();
        auto start_line = start_tk->getLine();
        auto end_line = start_tk->getLine();
        auto start_column = start_tk->getCharPositionInLine();
        auto end_column = end_tk->getCharPositionInLine();

        // Set the current debug location
        builder->SetCurrentDebugLocation(
            llvm::DILocation::get(this->context,
                                  start_line,   // line number
                                  start_column, // column number
                                  scope         // llvm::DIScope* (e.g., from a subprogram)
                                  ));
    }

    void Backend::set_debug_loc(llvm::Instruction *value, antlr4::ParserRuleContext *context)
    {
        if (logia_config.debug)
        {
            // value->setDebugLoc(llvm::DebugLoc::get(1, 0, this->dfile));
            value->setDebugLoc(llvm::DILocation::get(this->context, context->start->getLine(), context->start->getCharPositionInLine(), this->dscopes[this->dscopes.size() - 1]));
        }
    }

    llvm::Function *Backend::getFunction(llvm::StringRef name) const
    {
        auto func = this->intrinsics_module->getFunction(name);
        if (func)
        {
            return func;
        }
        func = this->module->getFunction(name);
        if (func)
        {
            return func;
        }

        throw std::runtime_error(std::format("{}{}", "function not found in current module or intrinsics", name));
    }
}