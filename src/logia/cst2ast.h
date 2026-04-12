#pragma once

#include "LogiaParser.h"
#include "LogiaParserVisitor.h"
#include "LogiaParserBaseVisitor.h"

#include "ast/program.h"
#include "ast/type.h"

#include <vector>

#define CAST_PTR_TO(type, expr) \
    (type *)((void *)(expr))

namespace logia
{
    /**
     * This class defines an abstract visitor for a parse tree
     * produced by LogiaParser.
     */
    // do not extend LogiaParserVisitor or we will have to impl 100% of methods!
    class CST2AST : public LogiaParserBaseVisitor
    {
    public:
        logia::AST::Program *program;
        logia::AST::Block *block;

        CST2AST(logia::AST::Program *_program);

        std::any visitProgram(LogiaParser::ProgramContext *context) override;

        //
        // Expressions
        //
        std::any visitNumberLiteral(LogiaParser::NumberLiteralContext *context) override;

        // rhsExpr it's just a container not needed
        std::any visitRhsExpr(LogiaParser::RhsExprContext *context) override;
        std::any visitConditionalExpr(LogiaParser::ConditionalExprContext *context) override;
        std::any visitLogicalOrExpr(LogiaParser::LogicalOrExprContext *context) override;
        std::any visitLogicalAndExpr(LogiaParser::LogicalAndExprContext *context) override;
        std::any visitInclusiveOrExpr(LogiaParser::InclusiveOrExprContext *context) override;
        std::any visitExclusiveOrExpr(LogiaParser::ExclusiveOrExprContext *context) override;
        std::any visitAndExpr(LogiaParser::AndExprContext *context) override;
        std::any visitEqualityExpr(LogiaParser::EqualityExprContext *context) override;
        std::any visitRelationalExpr(LogiaParser::RelationalExprContext *context) override;
        std::any visitShiftExpr(LogiaParser::ShiftExprContext *context) override;
        std::any visitAdditiveExpr(LogiaParser::AdditiveExprContext *context) override;
        std::any visitMultiplicativeExpr(LogiaParser::MultiplicativeExprContext *context) override;
        std::any visitCastExpr(LogiaParser::CastExprContext *context) override;
        std::any visitUnaryExpr(LogiaParser::UnaryExprContext *context) override;
        std::any visitPostfixExpr(LogiaParser::PostfixExprContext *context) override;
        std::any postfixCallExpr(LogiaParser::PostfixExprContext *locator, LogiaParser::ArgumentExprListContext *arguments);
        std::any visitPrimaryExpr(LogiaParser::PrimaryExprContext *context) override;
        std::any visitMayBeConstant(LogiaParser::MayBeConstantContext *context) override;
        std::any visitConstant(LogiaParser::ConstantContext *context) override;

        std::any visitIdentifier(LogiaParser::IdentifierContext *context) override;
        std::any visitStringLiteral(LogiaParser::StringLiteralContext *context) override;

        std::any visitReturnStmt(LogiaParser::ReturnStmtContext *context) override;

        std::any visitType(LogiaParser::TypeContext *context) override;
        //
        // functions
        //
        std::any visitFunctionDecl(LogiaParser::FunctionDeclContext *context) override;
        std::any visitFunctionBody(LogiaParser::FunctionBodyContext *context) override;
        std::any visitFunctionBodyStmt(LogiaParser::FunctionBodyStmtContext *context) override;
        std::any visitEndOfStmt(LogiaParser::EndOfStmtContext *context) override;

        // Fallback: delegate to children
        antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override;
    };
}