#include "compiler.h"

#include <llvm/Support/TargetSelect.h>

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "llvmvisitor.h"
#include "windows.h"

#include "ast/constexpr.h"

namespace logia
{

    ErrorListener::ErrorListener(std::string inputFile, std::string input)
    {
        this->input = input;
        this->inputFile = inputFile;
    }
    // ErrorListener implementation
    void ErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                                    size_t column, const std::string &msg, std::exception_ptr e)
    {
        // Create a stringstream object
        // to str
        stringstream ss(this->input);

        // Temporary object to store
        // the splitted string
        string str;

        // Delimiter
        char del = '\n';

        // Splitting the str string
        // by delimiter
        int count = 1;
        auto start = max(0, line - 5);
        auto end = line + 3;
        while (getline(ss, str, del))
        {
            if (count >= start && count < end)
            {
                cout << str << std::endl;
            }
            if (count == line)
            {
                for (int i = 0; i < column; ++i)
                {
                    cout << " ";
                }
                // console.log(Array(column).fill("-").join("") + "^\x1B[31m", msg, "\x1B[39m")
                cout << "^\x1B[31m" << msg << "\x1B[39m" << std::endl;
            }

            ++count;
        }

        std::cout << this->inputFile << ":" << line << ":" << column;

        // console.log(recognizer.getLiteralNames())
        // console.log(recognizer.getSymbolicNames())
        std::cout << offendingSymbol->toString();

        exit(1);
    }

    void ErrorListener::reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts, antlr4::atn::ATNConfigSet *configs) {}
    void ErrorListener::reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet &conflictingAlts, antlr4::atn::ATNConfigSet *configs) {}
    void ErrorListener::reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet *configs) {}

    Compiler::~Compiler()
    {
        // parser will remove this
        this->cst_tree = nullptr;

        this->parser->removeErrorListeners();
        delete this->errorListener;

        delete this->parser;

        delete this->tokens;
        delete this->lexer;
        delete this->input;
        free(this->text);
    }

    char *Compiler::file_read(const char *file_path)
    {
        FILE *file;
        auto err = fopen_s(&file, file_path, "rb");
        if (err)
        {
            std::stringstream ss;
            ss << "Error opening file: " << file_path;

            perror(ss.str().c_str());
            return nullptr;
        }

        // Seek to the end to determine file size
        fseek(file, 0, SEEK_END);
        auto fileSize = ftell(file);
        rewind(file);

        // Allocate memory for the file content
        char *buffer = (char *)malloc(fileSize + 2);
        if (buffer == NULL)
        {
            perror("Memory allocation failed");
            fclose(file);

            return nullptr;
        }

        // Read the file into the buffer
        fread(buffer, 1, fileSize, file);
        buffer[fileSize] = '\n';     // Null-terminate the string
        buffer[fileSize + 1] = '\0'; // Null-terminate the string

        fclose(file);
        return buffer;
    }

    void Compiler::read(const char *file_path)
    {
        if (this->verbose)
        {
            std::cout << "parse(" << file_path << ")" << std::endl;
        }
        this->text = this->file_read(file_path);

        this->input = new antlr4::ANTLRInputStream(text);

        this->lexer = new LogiaLexer(input);
        this->tokens = new antlr4::CommonTokenStream(this->lexer);
        this->parser = new LogiaParser(this->tokens);
        this->errorListener = (antlr4::ANTLRErrorListener *)new ErrorListener(file_path, text);
        this->parser->addErrorListener(this->errorListener);
    }

    void Compiler::compile()
    {
        /*
            llvm::InitializeAllTargetInfos();
            llvm::InitializeAllTargets();
            llvm::InitializeAllTargetMCs();
            llvm::InitializeAllAsmParsers();
            llvm::InitializeAllAsmPrinters();
        */
    }

    antlr4::ParserRuleContext *Compiler::parse()
    {
        if (this->is_program)
        {
            this->cst_tree = this->parser->program();
        }
        else
        {
            this->cst_tree = this->parser->packageProgram();
        }

        return this->cst_tree;
    }

    void Compiler::print_cst()
    {
        std::cout << "cst:" << std::endl
                  << this->cst_tree->toStringTree(this->parser, true) << std::endl;
    }

    void Compiler::build_ast()
    {
        this->backend = new Backend();
        this->backend->load_intrinsics();

        LLVMVisitor *llvmVisitor = new LLVMVisitor(this->backend->program);
        llvmVisitor->visit(this->cst_tree);
        this->ast_tree = this->backend->program;
    }

    void Compiler::print_ast()
    {
        std::cout << "ast:" << std::endl
                  << this->ast_tree->to_string_tree() << std::endl;
    }
}