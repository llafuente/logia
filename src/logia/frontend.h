#pragma once

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"
#include "utils.h"

#include "ast/program.h"
#include "logia/config.h"

#include <Windows.h>

namespace logia
{
    using namespace std;

    class ErrorListener : antlr4::ANTLRErrorListener
    {
        std::string input;
        std::string inputFile;

    public:
        ErrorListener(std::string inputFile, std::string input);
        void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                         size_t column, const std::string &msg, std::exception_ptr e);

        // Inherited via ANTLRErrorListener
        void reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts, antlr4::atn::ATNConfigSet *configs) override;
        void reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet &conflictingAlts, antlr4::atn::ATNConfigSet *configs) override;
        void reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet *configs) override;
    };
    /// @brief Compiler frontend, parsing and AST construction
    struct Frontend
    {
        ::logia::Config config;

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

        // CST - antlr
        LogiaParser *parser;
        antlr4::ANTLRErrorListener *errorListener;
        antlr4::CommonTokenStream *tokens;
        LogiaLexer *lexer;
        antlr4::ANTLRInputStream *input;
        antlr4::ParserRuleContext *cst_tree;

        // AST
        AST::Program *ast_tree;

        // CODEGEN
        Backend *backend;

        // aux
        char *text;

        Frontend(const char *file_path, ::logia::Config config);
        ~Frontend();

        /// @brief Parses the input file and builds the CST tree then transform it into AST
        AST::Program *parse();

        /// @brief run current AST
        int run();

    private:
        /// @brief Prints CST tree
        /// @param out The output stream to print the CST tree
        void print_cst(std::ostream &out);

        /// @brief Builds the AST from the CST tree
        void build_ast();
        /// @brief Prints AST tree
        /// @param out The output stream to print the AST tree
        void print_ast(std::ostream &out);
        char *__file_read(const char *file_path);
    };

}