#pragma once

#include "ast/node.h"
#include "ast/type.h"
#include "ast/stmt.h"
#include "ast/operators.h"

namespace logia::AST
{
    struct VarDeclStmt;
    struct Identifier;
    struct TypeDef;
    struct Function;
    enum class Operators;

    /// @brief Base expression
    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

    /// @brief Call expression, can be a function call, method call, operator call, etc.
    struct CallExpression : Expression
    {
        /// @brief Argument count
        uint32_t argument_count = 0;
        /// @brief Cached return type
        Type *type = nullptr;

        /// @brief Empty constructor for internal usage of CallExpression
        /// @remarks Do not use the constructor to build ASTs
        CallExpression(antlr4::ParserRuleContext *rule);
        CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments);

        /// @brief Get the locator expression of the call
        /// @return The locator expression
        Expression *get_locator();

        /// @brief Get the arguments expressions of the call
        std::vector<Expression *> get_arguments();

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
        void pre_type_inference() override;
        void post_type_inference() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
    };
    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments);

    /// @brief Member access expression, used for struct field access and method calls
    struct MemberAccessExpression : Expression
    {
        Type *type = nullptr;

        MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right);
        /// @brief Retrieves the left expression of the member access
        /// @return The left expression
        Expression *get_left();
        /// @brief Retrieves the right identifier of the member access
        /// @return The right identifier
        Identifier *get_right();

        std::string to_string() override;

        void pre_type_inference() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;
        void set_type(Type *type) override;

        Node *resolve() override;
    };

    /// @brief Creates a member access expression
    /// @param left The left expression of the member access
    /// @param right The right identifier of the member access
    /// @return The created member access expression
    LOGIA_API LOGIA_LEND MemberAccessExpression *ast_create_memberaccess_expr(Node *left, Node *right);

    /// @brief Binary expression, used for binary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct BinaryExpression : Expression
    {
        Operators op = Operators::DEFAULT;
        CallExpression *call_expr = nullptr;
        Type *type = nullptr;

        BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, Operators op, Expression *right);

        bool is_assignament();
        Expression *get_left();
        Expression *get_right();
        std::string to_string() override;

        Type *get_type() override;
        void set_type(Type *type) override;

        void pre_type_inference() override;
        void post_type_inference() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
    };

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, Operators op, Expression *right);

    /// @brief Prefix unary expression, used for prefix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    /// @remarks Reference operator can be an instrinic and it's implemented directly
    struct PrefixUnaryExpression : CallExpression
    {
        Operators op;
        PrefixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
        virtual void post_type_inference() override;
    };
    /// @brief Creates a reference to an expression
    /// @param operand
    /// @return
    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_ref(Expression *operand);

    /// @brief Creates a prefix unary expression
    /// @param op The prefix unary operator
    /// @param operand The operand of the prefix unary expression
    /// @return The created prefix unary expression
    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_preunary_expr(Operators op, Expression *operand);

    /// @brief Postfix unary expression, used for postfix unary operators
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct PostfixUnaryExpression : CallExpression
    {
        Operators op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand);
        Expression *get_operand();
        std::string to_string() override;
        virtual void post_type_inference() override;
    };

    /// @brief Defines an identifier, used for variable names, function names, struct field names, etc.
    struct Identifier : Expression
    {
        /// @brief The name of the identifier
        const char *identifier;
        Type *type = nullptr;

        Identifier(antlr4::ParserRuleContext *rule, const char *identifier);
        bool operator==(const char *id);
        bool operator==(const Identifier *id);

        /// @brief Gets the variable declaration associated with this identifier
        /// @return
        VarDeclStmt *get_var_decl();
        /// @brief Gets the function declaration associated with this identifier
        /// @return
        Function *get_function_decl();

        std::string to_string() override;

        void pre_type_inference() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;
        void set_type(Type *t) override;

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
        uint32_t values = 0;
        Type *type = nullptr;

        StructInitializer(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        void set_type(Type *type) override;
        void add_named_property(Identifier *locator, Expression *value);
        void add_positional_property(Expression *value);
        void set_named_property(Identifier* name, Expression* value, uint32_t index);

        Expression *get_value_by_index(uint32_t index);
        Expression *get_value_by_name(const char *name);

        // Inherited via Expression
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
    };

    //
    // utils
    //

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_postfix_unary_operator_to_string(Operators op, Type *operand);

    /// @brief Converts a binary operator to its string representation
    LOGIA_API const char *ast_prefix_unary_operator_to_string(Operators op, Type *operand);

    /// @brief Converts a binary operator to its string representation
    /// @param op The binary operator
    /// @param left The left operand type
    /// @param right The right operand type
    /// @return The string representation of the binary operator
    LOGIA_API const char *ast_binary_operator_to_string(Operators op, Type *left, Type *right);

}