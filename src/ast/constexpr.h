#pragma once

#include "ast/node.h"
#include "ast/expr.h"
#include "ast/block.h"
#include "utils.h"

namespace logia::AST
{
    struct ConstExpression : Expression
    {
        // REVIEW strange  why do i need to declare this ?
        ConstExpression(antlr4::ParserRuleContext *rule, ast_types type);
        std::string to_string() override;
    };

    struct StringLiteral : ConstExpression
    {
        char *text;
        StringLiteral(antlr4::ParserRuleContext *rule, char *text);
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

    struct FloatLiteral : ConstExpression
    {
    public:
        double value;

        FloatLiteral(antlr4::ParserRuleContext *rule, Type *type, double value);
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

    struct IntegerLiteral : ConstExpression
    {
        char *number_str;

        IntegerLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type);

        uint64_t as_unsigned();
        int64_t as_signed();

        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

    /**
     * Creates a string literal
     */
    LOGIA_API LOGIA_LEND StringLiteral *ast_create_string_lit(LOGIA_CLONE const char *text);
    /**
     * Creates a floating point literal
     */
    LOGIA_API LOGIA_LEND FloatLiteral *ast_create_float_lit(Block *body, double value);
    /**
     * Creates a signed integer literal
     */
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_int_lit(Block *body, const char *numberstr);
    /**
     * Creates an unsigned integer literal
     */
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_uint_lit(Block *body, const char *numberstr);
}