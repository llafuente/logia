#pragma once

#include "ast/node.h"
#include "ast/type.h"

namespace logia::AST
{
    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule, ast_types type);
        std::string to_string() override;
    };

    struct CallExpression : Expression
    {
        CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> arguments);
        Expression *get_locator();
        // TODO this should be std::vector<Expression *>
        // but casting fail
        std::vector<Node *> get_arguments();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };
    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments);

    struct MemberAccessExpression : Expression
    {
        MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Node *right);
        Expression *get_left();
        Expression *get_right();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND MemberAccessExpression *ast_create_memberaccess_expr(Node *left, Node *right);

    enum class BinaryOperator
    {
        ADD,        // +
        SUB,        // -
        MUL,        // *
        DIV,        // /
        MOD,        // %
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        GT,         // >
        LTE,        // <=
        GTE,        // >=
        AND,        // &&
        OR,         // ||
        ASSIGN,     // =
        ADD_ASSIGN, // +=
        SUB_ASSIGN, // -=
        MUL_ASSIGN, // *=
        DIV_ASSIGN, // /=
        XOR,        // ^
        SHL,        // <<
        SHR,        // >>
    };

    struct BinaryExpression : CallExpression
    {
        BinaryOperator op;
        BinaryExpression(antlr4::ParserRuleContext *rule, BinaryOperator op, Expression *left, Expression *right);
        Expression *get_left()
        {
            return (Expression *)this->children[0];
        }
        Expression *get_right()
        {
            return (Expression *)this->children[1];
        }
        std::string to_string() override;
    };

    enum class PrefixUnaryOperator
    {
        NEGATION,    // -
        LOGICAL_NOT, // !
        INCREMENT,   // ++
        DECREMENT,   // --
        BITWISE_NOT, // ~
    };

    struct PrefixUnaryExpression : CallExpression
    {
        PrefixUnaryOperator op;
        PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand);
        Expression *get_operand()
        {
            return (Expression *)this->children[0];
        }
        std::string to_string() override;
    };

    enum class PostfixUnaryOperator
    {
        INCREMENT, // ++
        DECREMENT, // --
    };

    struct PostfixUnaryExpression : CallExpression
    {
        PostfixUnaryOperator op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand);
        Expression *get_operand()
        {
            return (Expression *)this->children[0];
        }
        std::string to_string() override;
    };

    struct Identifier : Expression
    {
        char *identifier;
        Identifier(antlr4::ParserRuleContext *rule, char *identifier) : Expression(rule, ast_types::IDENTIFIER)
        {
            LOGIA_ASSERT(type);
            this->identifier = identifier;
        }
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override
        {
            // TODO resolve!
            return nullptr;
        }
    };

    /**
     * Creates an identifier
     */
    LOGIA_API LOGIA_LEND Identifier *ast_create_identifier(Node *current, char *name);

    //
    // utils
    //
    LOGIA_API const char *ast_postfix_unary_operator_to_string(PostfixUnaryOperator op);
    LOGIA_API const char *ast_prefix_unary_operator_to_string(PrefixUnaryOperator op);
    LOGIA_API const char *ast_binary_operator_to_string(BinaryOperator op);
}