#include "logia/frontend.h"

#include <llvm/Support/TargetSelect.h>

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "logia/cst2ast.h"
#include "logia/compiler_error.h"

#if _WIN32
#include "windows.h"
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib") // Link with Shlwapi.lib
#else
// TODO On Linux/macOS
#endif

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
                for (size_t i = 0; i < column; ++i)
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

    Frontend::Frontend(const char *file_path, ::logia::Config config) : config(config)
    {
        CHAR **lppPart = {NULL};
        GetCurrentDirectoryA(MAX_PATH, this->cwd);
        auto retval = GetFullPathNameA(file_path,
                                       MAX_PATH,
                                       this->entry_point_fullpath,
                                       lppPart);

        if (retval == 0)
        {
            throw_compiler_error(std::format("GetFullPathName failed: {}. Error: {}", file_path, GetLastError()));
        }

        // retval -> last back slash backwards :)
        do
        {
            --retval;
        } while (retval > 0 && this->entry_point_fullpath[retval] != '\\' && this->entry_point_fullpath[retval] != '/');

        strcpy_s(this->entry_point_filename, &this->entry_point_fullpath[retval + 1]);
        strcpy_s(this->entry_point_absdir, MAX_PATH, this->entry_point_fullpath);
        this->entry_point_absdir[retval + 1] = '\0';

        BOOL success = PathRelativePathToA(
            this->entry_point_reldir, // Output buffer
            this->cwd,                // From path
            FILE_ATTRIBUTE_DIRECTORY, // Base is a directory
            this->entry_point_absdir, // To path
            FILE_ATTRIBUTE_DIRECTORY  // Target is a file
        );

        if (!success)
        {
            throw_compiler_error(std::format("PathRelativePathToA failed: {}. Error: {}", file_path, GetLastError()));
        }

        DEBUG() << "entry_point_fullpath = " << entry_point_fullpath << std::endl
                << "entry_point_absdir = " << entry_point_absdir << std::endl
                << "entry_point_reldir = " << entry_point_reldir << std::endl
                << "entry_point_filename = " << entry_point_filename << std::endl

                << "verbose = " << config.verbose << std::endl
                << "print = " << config.print << std::endl
                << "print_cst = " << config.print_cst << std::endl
                << "print_ast = " << config.print_ast << std::endl

                << "llfile = " << (config.llfile == nullptr ? "no" : config.llfile) << std::endl
                << "objfile = " << (config.objfile == nullptr ? "no" : config.objfile) << std::endl

                << "is_program = " << config.is_program << std::endl
                << "debug = " << config.debug << std::endl
                << "coverage = " << config.coverage << std::endl;
    }

    Frontend::~Frontend()
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

    char *Frontend::__file_read(const char *file_path)
    {
        FILE *file;
        auto err = fopen_s(&file, file_path, "rb");
        if (err)
        {
            throw_compiler_error(std::format("Error opening file: {}", file_path));
        }

        // Seek to the end to determine file size
        fseek(file, 0, SEEK_END);
        auto fileSize = ftell(file);
        rewind(file);

        // Allocate memory for the file content
        char *buffer = (char *)malloc(fileSize + 2);
        if (buffer == NULL)
        {
            fclose(file);
            throw_compiler_error(std::format("Memory allocation failed: {}", file_path));
        }

        // Read the file into the buffer
        fread(buffer, 1, fileSize, file);
        buffer[fileSize] = '\n';     // Null-terminate the string
        buffer[fileSize + 1] = '\0'; // Null-terminate the string

        fclose(file);
        return buffer;
    }

    AST::Program *Frontend::parse()
    {
        if (!this->entry_point_filename)
        {
            throw_compiler_error("Frontend: File not specified");
        }
        if (this->config.verbose)
        {
            std::cout << "parse(" << this->entry_point_fullpath << ")" << std::endl;
        }
        this->text = this->__file_read(this->entry_point_fullpath);

        if (this->config.print)
        {
            std::cerr << "File Contents:" << std::endl
                      << this->text << std::endl;
        }

        this->input = new antlr4::ANTLRInputStream(text);

        this->lexer = new LogiaLexer(input);
        this->tokens = new antlr4::CommonTokenStream(this->lexer);
        this->parser = new LogiaParser(this->tokens);
        this->errorListener = (antlr4::ANTLRErrorListener *)new ErrorListener(this->entry_point_fullpath, text);
        this->parser->addErrorListener(this->errorListener);

        if (this->config.is_program)
        {
            this->cst_tree = this->parser->program();
        }
        else
        {
            this->cst_tree = this->parser->packageProgram();
        }

        this->print_cst(this->config.print_cst ? std::cerr : logia_log_file);

        this->build_ast();

        this->print_ast(this->config.print_ast ? std::cerr : logia_log_file);

        return this->ast_tree;
    }

    void Frontend::print_cst(std::ostream &out)
    {
        out << "cst:" << std::endl
            << this->cst_tree->toStringTree(this->parser, true) << std::endl;
    }

    void Frontend::build_ast()
    {
        this->ast_tree = new AST::Program(nullptr, this->entry_point_fullpath);

        CST2AST *llvmVisitor = new CST2AST(this->ast_tree);
        llvmVisitor->visit(this->cst_tree);
    }

    void Frontend::print_ast(std::ostream &out)
    {
        out << "ast:" << std::endl
            << this->ast_tree->to_string_tree() << std::endl;
    }
}