#pragma once

#include "ast/node.h"
#include "ast/type.h"
#include "ast/stmt.h"

namespace logia::AST
{
    struct VarDeclStmt;
    struct Identifier;

    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule, ast_types type);
        std::string to_string() override;
    };

    struct CallExpression : Expression
    {
        /**!
         * Empty constructor for internal usage of CallExpression
         * Do not use the constructor to build ASTs
         */
        CallExpression();
        CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments);
        Expression *get_locator();
        // TODO this should be std::vector<Expression *>
        // but casting fail
        std::vector<Node *> get_arguments();

        void add_named_argument(Identifier *id, Expression *expr);
        void add_positional_argument(Expression *expr);

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
        ADD,         // +
        SUB,         // -
        MUL,         // *
        DIV,         // /
        MOD,         // %
        EQ,          // ==
        NEQ,         // !=
        LT,          // <
        GT,          // >
        LTE,         // <=
        GTE,         // >=
        AND,         // &&
        OR,          // ||
        ASSIGN,      // =
        ADD_ASSIGN,  // +=
        SUB_ASSIGN,  // -=
        MUL_ASSIGN,  // *=
        DIV_ASSIGN,  // /=
        BITWISE_OR,  // |
        BITWISE_AND, // &
        XOR,         // ^
        SHL,         // <<
        SHR,         // >>
    };

    struct BinaryExpression : CallExpression
    {
        BinaryOperator op;
        BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, BinaryOperator op, Expression *right);
        Expression *get_left();
        Expression *get_right();
        std::string to_string() override;
    };

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, BinaryOperator op, Expression *right);

    enum class PrefixUnaryOperator
    {
        DEREFERENCE, // &
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
        Expression *get_operand();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_ref(Expression *operand);

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_preunary_expr(PrefixUnaryOperator op, Expression *operand);

    enum class PostfixUnaryOperator
    {
        INCREMENT, // ++
        DECREMENT, // --
    };

    struct PostfixUnaryExpression : CallExpression
    {
        PostfixUnaryOperator op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;
    };

    struct Identifier : Expression
    {
        char *identifier;
        Identifier(antlr4::ParserRuleContext *rule, char *identifier);
        VarDeclStmt *get_var_decl();
        Type *get_function_decl();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
        void on_after_attach() override;
    };

    /**
     * Creates an identifier
     */
    LOGIA_API LOGIA_LEND Identifier *ast_create_identifier(char *name);

    //
    // utils
    //
    LOGIA_API const char *ast_postfix_unary_operator_to_string(PostfixUnaryOperator op);
    LOGIA_API const char *ast_prefix_unary_operator_to_string(PrefixUnaryOperator op);
    LOGIA_API const char *ast_binary_operator_to_string(BinaryOperator op);
}