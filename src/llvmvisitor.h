#pragma once

#include "LogiaParser.h"
#include "LogiaParserVisitor.h"
#include "LogiaParserBaseVisitor.h"

namespace logia {
    /**
     * This class defines an abstract visitor for a parse tree
     * produced by LogiaParser.
     */
    // do not extend LogiaParserVisitor or we will have to impl 100% of methods!
    class LLVMVisitor: public LogiaParserBaseVisitor {
        public:
        LLVMVisitor() {
        }

        virtual std::any visitProgram(LogiaParser::ProgramContext *context) override  {
            std::cout << "visitProgram" << std::endl;
            // return this->visitProgramStmsList(context->statements());
            return this->visitChildren(context);
        }



        // Fallback: delegate to children
        antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override {
            std::any result = 0;
            for (size_t i = 0; i < node->children.size(); i++) {
                result = visit(node->children[i]);
            }
            return result;
        }

    };
}