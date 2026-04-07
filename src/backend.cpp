#include "backend.h"
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

#include "llvm/Support/Compiler.h"
#include "llvm/ExecutionEngine/Orc/MaterializationUnit.h"
#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include <memory>

#include "utils.h"
#include "ast/program.h"

// cross compile support ?
#define CODEGEN_NATIVE

namespace logia
{
    Backend::Backend()
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
        DEBUG() << "Backend()" << std::endl;
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

        auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
        if (!EPC)
        {
            llvm::errs() << EPC.takeError();
            throw std::exception("Error creating SelfExecutorProcessControl");
        }

        session = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));
        session->createBareJITDylib("<main>");

        this->program = AST::ast_create_program(this->context);
    }

    Backend::~Backend()
    {
        llvm::errs() << this->session->endSession();
    }

    void Backend::load_intrinsics(char *filepath)
    {
        llvm::SMDiagnostic diag;
        module = llvm::parseIRFile(filepath, diag, context);
        if (!module)
        {
            diag.print("intrinsics.ll", llvm::errs());
            throw std::exception("could not parse or read intrinsics.ll");
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

    bool Backend::emitTargetLLVMIR(std::string fileName = "main.ll")
    {
        DEBUG() << "(" << fileName << ")" << std::endl;
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
        // & "C:\Program Files\LLVM\bin\clang.exe" .\xxx.obj -o xxx.exe
        return true;
    }

    int Backend::run_jit()
    {
        DEBUG() << "()" << std::endl;
        DEBUG() << this->program->to_string_tree() << std::endl;
        this->program->codegen(this, this->builder);

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
        llvm::orc::ThreadSafeModule TSM(std::move(this->module), context);

        auto err = compileLayer.add(RT, std::move(TSM));
        if (err)
        {
            llvm::errs() << err;
            throw std::exception("Error add module");
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
        auto mangle_name = mangle("main");
        std::vector<llvm::orc::JITDylib *> SearchOrder = {dylib};
        auto symbol = session->lookup(SearchOrder, mangle_name);
        if (!symbol)
        {
            llvm::errs() << symbol.takeError();
            throw std::exception("could not find main function");
        }

        using FuncType = int (*)();
        // auto *main_fn = (FuncType)(symbol->getAddress().getValue());
        auto *main_fn = (FuncType)(symbol->getAddress().getValue());
        int result = main_fn();
        if (result != 0)
        {
            std::cerr << "[Error] Main function run error:" << result << std::endl;
        }

        if (auto Err = session->endSession())
        {
            session->reportError(std::move(Err));
        }

        return result;
    }
}