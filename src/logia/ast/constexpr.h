#pragma once

#include "logia/ast/node.h"
#include "logia/ast/block.h"
#include "logia/ast/expr.h"
#include "utils.h"

#include "llvm/ADT/APSInt.h"

namespace logia::AST
{
    /// @brief A constant expression that can be evaluated/used at compile time
    struct ConstExpression : Expression
    {
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

        StringLiteral(antlr4::ParserRuleContext *rule, LOGIA_CLONE const char *text);

        std::string to_string() override;

        // TODO generate our string data not cstring
        llvm::Value *post_codegen(logia::Backend *backend) override;

        // TODO return out type!!
        /// @brief Retrieves the type of the string literal
        /// @return
        Type *get_type() override;

    protected:
        void _set_type(Type *t) override;
        void _pre_type_inference() override;
    };

    /// @brief A floating point literal constant expression
    struct FloatLiteral : ConstExpression
    {
    public:
        /// @brief The integer value as text, we will parse it at codegen to support different bases and sizes
        char *value_str = nullptr;
        /// @brief "Biggest" float
        llvm::APFloat value = llvm::APFloat::IEEEdouble();
        // llvm::APFloat value = llvm::APFloat((double)0.0);

        /// @brief the type
        Type *type = nullptr;

        FloatLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type = nullptr);
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;

    protected:
        void _set_type(Type *t) override;
    };

    /// @brief An integer literal constant expression
    struct IntegerLiteral : ConstExpression
    {
        /// @brief The integer value as text, we will parse it at codegen to support different bases and sizes
        char *value_str = nullptr;

        // Create the biggest APSInt possible, check if it fits into the target width, then truncate
        // TODO REVIEW 128 is possible ?!
        llvm::APSInt value = llvm::APSInt(64, /*isUnsigned=*/true);
        /// @brief the type
        Type *type = nullptr;

        IntegerLiteral(antlr4::ParserRuleContext *rule, const char *number_as_text, Type *type = nullptr);

        /// @brief negates current value and value_str
        void negate();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;

    protected:
        void _set_type(Type *t) override;
    };
}