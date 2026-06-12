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

    ParseResult::ParseResult(const char *file_path)
    {
        LOG(DBG, "{}", logia_config.to_string());

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

        LOG(DBG, "entry_point_fullpath = '{}'\nentry_point_absdir = '{}'\nentry_point_reldir = '{}'\nentry_point_filename = '{}'", entry_point_fullpath, entry_point_absdir, entry_point_reldir, entry_point_filename);
    }

    ParseResult::~ParseResult()
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

        delete this->ast_tree;
    }

    char *ParseResult::__file_read(const char *file_path)
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

    AST::Program *ParseResult::parse(bool is_program)
    {
        if (this->entry_point_filename[0] == '\0') // empty?
        {
            throw_compiler_error("ParseResult: File not specified");
        }
        if (logia_config.verbose)
        {
            std::cout << "parse(" << this->entry_point_fullpath << ")" << std::endl;
        }
        this->text = this->__file_read(this->entry_point_fullpath);

        if (logia_config.print)
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

        auto rule = this->parser->program();
        this->cst_tree = rule;

        logia_log_level = logia_config.cst_log_level;
        this->print_cst(logia_config.print_cst ? std::cerr : logia_log_file);

        if (is_program)
        {
            this->ast_tree = new AST::Program({this->entry_point_fullpath, 0, 0, 0, 0, this->text}, this->entry_point_fullpath, this->text);
        }
        else
        {
            // TODO @llafuente invalid cast -> try no to use "package/program" staff inside CST2AST
            this->ast_tree = (AST::Program *)new AST::Package({this->entry_point_fullpath, 0, 0, 0, 0, this->text}, this->entry_point_fullpath, this->text);
        }

        LOGIA_VERIFY(this->ast_tree->loc.file != nullptr);
        LOGIA_VERIFY(this->ast_tree->loc.text != nullptr);

        LOG(DBG, "start CST2AST");
        CST2AST *llvmVisitor = new CST2AST(this->ast_tree);
        llvmVisitor->visit(this->cst_tree);

        LOGIA_VERIFY(this->ast_tree->loc.file != nullptr);
        LOGIA_VERIFY(this->ast_tree->loc.text != nullptr);

        logia_log_level = logia_config.ast_log_level;
        this->print_ast(logia_config.print_ast ? std::cerr : logia_log_file);

        return this->ast_tree;
    }

    void ParseResult::print_cst(std::ostream &out)
    {
        out << "cst:" << std::endl
            << this->cst_tree->toStringTree(this->parser, true) << std::endl;
    }

    void ParseResult::print_ast(std::ostream &out)
    {
        out << "ast:" << std::endl
            << this->ast_tree->to_string_tree() << std::endl;
    }

    LOGIA_LEND ParseResult *logia_parse_package(const char *file_path)
    {
        auto parse_result = new ParseResult(file_path);
        parse_result->parse(false);
        return parse_result;
    }

    LOGIA_LEND ParseResult *logia_parse_program(const char *file_path)
    {
        auto parse_result = new ParseResult(file_path);
        parse_result->parse(true);
        return parse_result;
    }
}
