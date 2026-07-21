#include "logia/frontend.h"

#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "logia/log.h"
#include "logia/cst2ast.h"
#include "logia/compiler_error.h"
#include "logia/ast/program.h"
#include "logia/ast/package.h"

#include "llvm/Support/TargetSelect.h"
#include "antlr4-runtime.h"

#if _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib") // Link with Shlwapi.lib
#else
// TODO On Linux/macOS
#endif

namespace logia
{
    // helper functions
    void logia_parse_print_cst(LogiaParser *parser, antlr4::ParserRuleContext *tree, std::ostream &out)
    {
        out << "cst:" << std::endl
            << tree->toStringTree(parser, true) << std::endl;
    }

    void logia_parse_print_ast(AST::Node *node, std::ostream &out)
    {
        out << "ast:" << std::endl
            << node->to_string_tree() << std::endl;
    }

    char *logia_file_read(const char *file_path)
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
        auto start = std::max<size_t>(0, line - 5);
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

    // the real "parser"
    template <class T>
    std::unique_ptr<T> logia_parse_file(const char *file_path)
    {
        /// @brief Current working directory
        char cwd[MAX_PATH];
        /// @brief Main entry point file
        char entry_point_fullpath[MAX_PATH];
        /// @brief Absolute directory to entry point
        char entry_point_absdir[MAX_PATH];
        /// @brief Relative directory to entry point
        char entry_point_reldir[MAX_PATH];
        /// @brief entry point filename
        char entry_point_filename[MAX_PATH];

        // handle cstring ownership
        struct FreeDeleter
        {
            void operator()(char *p) const noexcept { free(p); }
        };
        std::unique_ptr<char, FreeDeleter> textOwner;
        char *text = nullptr;

        LOG(DBG, "{}", logia_config.to_string());

        CHAR **lppPart = {NULL};
        GetCurrentDirectoryA(MAX_PATH, cwd);
        auto retval = GetFullPathNameA(file_path,
                                       MAX_PATH,
                                       entry_point_fullpath,
                                       lppPart);

        if (retval == 0)
        {
            throw_compiler_error(std::format("GetFullPathName failed: {}. Error: {}", file_path, GetLastError()));
        }

        // retval -> last back slash backwards :)
        do
        {
            --retval;
        } while (retval > 0 && entry_point_fullpath[retval] != '\\' && entry_point_fullpath[retval] != '/');

        strcpy_s(entry_point_filename, &entry_point_fullpath[retval + 1]);
        strcpy_s(entry_point_absdir, MAX_PATH, entry_point_fullpath);
        entry_point_absdir[retval + 1] = '\0';

        BOOL success = PathRelativePathToA(
            entry_point_reldir,       // Output buffer
            cwd,                      // From path
            FILE_ATTRIBUTE_DIRECTORY, // Base is a directory
            entry_point_absdir,       // To path
            FILE_ATTRIBUTE_DIRECTORY  // Target is a file
        );

        if (!success)
        {
            throw_compiler_error(std::format("PathRelativePathToA failed: {}. Error: {}", file_path, GetLastError()));
        }

        LOG(DBG, "entry_point_fullpath = '{}'\nentry_point_absdir = '{}'\nentry_point_reldir = '{}'\nentry_point_filename = '{}'", entry_point_fullpath, entry_point_absdir, entry_point_reldir, entry_point_filename);

        if (entry_point_filename[0] == '\0') // empty?
        {
            throw_compiler_error("ParseResult: File not specified");
        }
        if (logia_config.verbose)
        {
            std::cout << "parse(" << entry_point_fullpath << ")" << std::endl;
        }
        textOwner.reset(logia_file_read(entry_point_fullpath));
        text = textOwner.get();

        if (logia_config.print)
        {
            std::cerr << "File Contents:" << std::endl
                      << text << std::endl;
        }

        // CST - antlr
        auto input = std::make_unique<antlr4::ANTLRInputStream>(text);
        auto lexer = std::make_unique<LogiaLexer>(input.get());
        auto tokens = std::make_unique<antlr4::CommonTokenStream>(lexer.get());
        auto parser = std::make_unique<LogiaParser>(tokens.get());
        auto errorListener = std::make_unique<ErrorListener>(entry_point_fullpath, text);
        parser->addErrorListener((antlr4::ANTLRErrorListener *)errorListener.get());

        antlr4::ParserRuleContext *cst_tree = parser->program();

        // logia_log_level = logia_config.cst_log_level;
        if (logia_config.print_cst)
        {
            logia_parse_print_cst(parser.get(), cst_tree, std::cerr);
        }
        else if (logia_log_level >= DBG)
        {
            logia_parse_print_cst(parser.get(), cst_tree, logia_log_file);
        }

        auto ast_tree = std::make_unique<T>((AST::location){entry_point_fullpath, 0, 0, 0, 0, 0, 0, text}, entry_point_fullpath, entry_point_reldir, text);
        textOwner.release(); // preserve previous ownership semantics (AST now owns/uses text)

        LOGIA_VERIFY(ast_tree->loc.file != nullptr);
        LOGIA_VERIFY(ast_tree->loc.text != nullptr);

        LOG(DBG, "start CST2AST");
        auto llvmVisitor = std::make_unique<CST2AST>(ast_tree.get());
        llvmVisitor->visit(cst_tree);

        LOGIA_VERIFY(ast_tree->loc.file != nullptr);
        LOGIA_VERIFY(ast_tree->loc.text != nullptr);

        if (logia_config.print_cst)
        {
            logia_parse_print_ast(ast_tree.get(), std::cerr);
        }
        else if (logia_log_level >= DBG)
        {
            logia_parse_print_ast(ast_tree.get(), logia_log_file);
        }

        // parser will remove this
        cst_tree = nullptr;

        parser->removeErrorListeners();
        return ast_tree.release();
    }

    LOGIA_LEND std::unique_ptr<AST::Package> logia_parse_package(const char *file_path)
    {
        return logia_parse_file<AST::Package>(file_path);
    }

    LOGIA_LEND std::unique_ptr<AST::Program> logia_parse_program(const char *file_path)
    {
        return logia_parse_file<AST::Program>(file_path);
    }
}
