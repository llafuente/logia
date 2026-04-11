#pragma once

#include "antlr4-runtime.h"
#include "LogiaParser.h"
#include "LogiaLexer.h"
#include "utils.h"

#include "ast/program.h"

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

    struct Frontend
    {
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

        // options
        bool is_program = true;
        bool verbose = false;

        ~Frontend();

        char *file_read(const char *file_path);
        void read(const char *file_path);
        antlr4::ParserRuleContext *parse();
        void compile();
        void print_cst();

        void build_ast();
        void print_ast();
    };

}