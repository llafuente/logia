#pragma once

#include "ast/node.h"
#include "ast/type.h"
#include "ast/stmt.h"

namespace logia::AST
{
    struct VarDeclStmt;
    struct Identifier;
    struct TypeDef;

    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule, ast_types type);
        std::string to_string() override;
    };

    /// @brief Call expression, can be a function call, method call, operator call, etc.
    struct CallExpression : Expression
    {
        /// @brief Empty constructor for internal usage of CallExpression
        /// @remarks Do not use the constructor to build ASTs
        CallExpression();
        CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments);

        /// @brief Get the locator expression of the call
        /// @return The locator expression
        Expression *get_locator();

        // TODO this should be std::vector<Expression *> but casting fail
        /// @brief Get the arguments expressions of the call
        std::vector<Node *> get_arguments();

        /// @brief Get the argument at the specified position
        /// @param pos The position of the argument
        /// @return The expression of the argument
        Expression *get_argument(uint32_t pos);

        /// @brief Get the name of the argument at the specified position
        /// @param pos The position of the argument
        /// @return The identifier of the argument
        Identifier *get_argument_name(uint32_t pos);

        /// @brief Add a named argument to the call expression
        /// @param id The identifier of the argument
        /// @param expr The expression of the argument
        void add_named_argument(Identifier *id, Expression *expr);

        /// @brief Add a positional argument to the call expression
        /// @param expr The expression of the argument
        void add_positional_argument(Expression *expr);

        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };
    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments);

    /// @brief Member access expression, used for struct field access and method calls
    struct MemberAccessExpression : Expression
    {
        MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right);
        /// @brief Get the left expression of the member access
        /// @return The left expression
        Expression *get_left();
        /// @brief Get the right identifier of the member access
        /// @return The right identifier
        Identifier *get_right();

        std::string to_string() override;

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;

        Type *get_type() override;

        Node *resolve() override;
    };

    /// @brief Creates a member access expression
    /// @param left The left expression of the member access
    /// @param right The right identifier of the member access
    /// @return The created member access expression
    LOGIA_API LOGIA_LEND MemberAccessExpression *ast_create_memberaccess_expr(Node *left, Node *right);

    enum class BinaryOperator
    {
        ADD,                 // +
        SUB,                 // -
        MUL,                 // *
        DIV,                 // /
        MOD,                 // %
        LOGIAL_EQ,           // ==
        LOGIAL_NEQ,          // !=
        LOGIAL_LT,           // <
        LOGIAL_GT,           // >
        LOGIAL_LTE,          // <=
        LOGIAL_GTE,          // >=
        LOGICAL_AND,         // &&
        LOGICAL_OR,          // ||
        ASSIGN,              // =
        ADD_ASSIGN,          // +=
        SUB_ASSIGN,          // -=
        MUL_ASSIGN,          // *=
        DIV_ASSIGN,          // /=
        BITWISE_OR,          // |
        BITWISE_AND,         // &
        BITWISE_XOR,         // ^
        BITWISE_LEFT_SHIFT,  // <<
        BITWISE_RIGHT_SHIFT, // >>
    };

    /// @brief Binary expression, used for binary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct BinaryExpression : CallExpression
    {
        BinaryOperator op;
        BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, BinaryOperator op, Expression *right);
        Expression *get_left();
        Expression *get_right();
        std::string to_string() override;
        virtual bool pre_type_inference() override;
        virtual void post_type_inference() override;
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
    /// @brief Prefix unary expression, used for prefix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    /// @remarks Reference operator can be an instrinic and it's implemented directly
    struct PrefixUnaryExpression : CallExpression
    {
        PrefixUnaryOperator op;
        PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };
    /// @brief Creates a reference to an expression
    /// @param operand
    /// @return
    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_ref(Expression *operand);

    /// @brief Creates a prefix unary expression
    /// @param op The prefix unary operator
    /// @param operand The operand of the prefix unary expression
    /// @return The created prefix unary expression
    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_preunary_expr(PrefixUnaryOperator op, Expression *operand);

    enum class PostfixUnaryOperator
    {
        INCREMENT, // ++
        DECREMENT, // --
    };

    /// @brief Postfix unary expression, used for postfix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct PostfixUnaryExpression : CallExpression
    {
        PostfixUnaryOperator op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;
    };

    /// @brief Defines an identifier, used for variable names, function names, struct field names, etc.
    struct Identifier : Expression
    {
        /// @brief The name of the identifier
        const char *identifier;

        Identifier(antlr4::ParserRuleContext *rule, const char *identifier);

        /// @brief Gets the variable declaration associated with this identifier
        /// @return
        VarDeclStmt *get_var_decl();
        /// @brief Gets the function declaration associated with this identifier
        /// @return
        Function *get_function_decl();

        std::string to_string() override;

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;

        Type *get_type() override;

        void post_attach() override;

        Node *resolve() override;
    };

    /// @brief Creates an identifier
    /// @param name The name of the identifier
    /// @return The created identifier
    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name);

    /// @brief Defines a struct initializer, used for struct literals and struct field default values
    struct StructInitializer : Expression
    {
    public:
        uint32_t length = 0;

        StructInitializer(antlr4::ParserRuleContext *rule);
        void set_type(Type *type);
        void add_named_property(TypeDef *locator, Expression *value);
        void add_positional_property(Expression *value);

        bool pre_type_inference() override;

        // Inherited via Expression
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

    //
    // utils
    //

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_postfix_unary_operator_to_string(PostfixUnaryOperator op);

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_prefix_unary_operator_to_string(PrefixUnaryOperator op);

    /// @brief Converts a binary operator to its string representation
    /// @param op The binary operator
    /// @param left The left operand type
    /// @param right The right operand type
    /// @return The string representation of the binary operator
    LOGIA_API const char *ast_binary_operator_to_string(BinaryOperator op, Type *left, Type *right);

}