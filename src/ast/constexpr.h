#pragma once

#include "ast/node.h"
#include "ast/block.h"
#include "ast/expr.h"
#include "utils.h"

namespace logia::AST
{
    /// @brief A constant expression that can be evaluated at compile time
    struct ConstExpression : Expression
    {
        // REVIEW strange  why do i need to declare this ?
        ConstExpression(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

    /// @brief A string literal constant expression
    struct StringLiteral : ConstExpression
    {
        /// @brief The string value (utf-8)
        char *text = nullptr;
        /// @brief string type, atm a false type cstring
        Type *type = nullptr;

        StringLiteral(antlr4::ParserRuleContext *rule, char *text);

        std::string to_string() override;

        void pre_type_inference() override;

        // TODO generate our string data not cstring
        llvm::Value *post_codegen(logia::Backend *backend) override;

        // TODO return out type!!
        /// @brief Retrieves the type of the string literal
        /// @return
        Type *get_type() override;
    };

    /// @brief A floating point literal constant expression
    struct FloatLiteral : ConstExpression
    {
    public:
        double value;

        FloatLiteral(antlr4::ParserRuleContext *rule, Type *type, double value);
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
    };

    /// @brief An integer literal constant expression
    struct IntegerLiteral : ConstExpression
    {
        /// @brief The integer value as text, we will parse it at codegen to support different bases and sizes
        char *number_str;

        IntegerLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type = nullptr);
        /// @brief Retrieves the integer value as the biggest unsigned value
        /// @return
        uint64_t as_unsigned();
        /// @brief Retrieves the integer value as the biggest signed value
        /// @return
        int64_t as_signed();

        std::string to_string() override;

        void set_type(Type *t) override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

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