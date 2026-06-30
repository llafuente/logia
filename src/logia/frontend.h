#pragma once

#include "utils.h"
#include "logia/ast/program.h"
#include "logia/ast/package.h"
#include "logia/config.h"

#include "LogiaParser.h"
#include "LogiaLexer.h"

#include "antlr4-runtime.h"

// do not include <windows.h>
#define MAX_PATH 260

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
                         size_t column, const std::string &msg, std::exception_ptr e) override;

        // Inherited via ANTLRErrorListener
        void reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts, antlr4::atn::ATNConfigSet *configs) override;
        void reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet &conflictingAlts, antlr4::atn::ATNConfigSet *configs) override;
        void reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet *configs) override;
    };

    AST::Package *logia_parse_package(const char *file_path);
    AST::Program *logia_parse_program(const char *file_path);

}