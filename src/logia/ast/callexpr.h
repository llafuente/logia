#pragma once

#include "logia/ast/node.h"
#include "logia/ast/expr.h"

namespace logia::AST
{
    struct FunctionParameter;
    struct Identifier;
    struct Function;

    /// @brief Argument of a call expression, can be named or positional
    struct CallExpressionArgument : Node
    {
        /// @brief The index of the argument in the call expression, used for positional arguments
        /// @remarks Handled by CallExpression, should not be set manually here
        size_t index = 0;
        /// @brief Set at type inference to match the function parameter
        FunctionParameter *parameter = nullptr;

        CallExpressionArgument(
            size_t index,
            Identifier *name,
            Expression *value);
        /// @brief Check if the argument is named
        /// @return True if the argument is named, false otherwise
        bool is_named();
        /// @brief Get the name of the argument
        /// @return The identifier of the argument
        Identifier *get_name();
        /// @brief Get the value of the argument
        /// @return The expression of the argument
        Expression *get_value();

        std::string to_string() override;

        Type *get_type() override;

        void post_attach() override;

        void validate() override;

    protected:
        void _set_type(Type *type) override;
        void _post_type_inference() override;
    };

    // TODO REVIEW It uses multiple dispatch, but while usefull maybe it's better to have single dispatch for "non-operator" functions

    /// @brief Call expression, can be a function call, method call, operator call, etc.
    struct CallExpression : Expression
    {
        /// @brief The call is known at compile time
        /// @return
        bool is_direct_call = false;
        /// @brief The call is unknown at compile time, via function pointer
        /// @return
        bool is_indirect_call = false;
        /// @brief Special case in wich lhs is "this", rhs is the method name/function pointer
        /// @return
        bool is_method_call = false;
        /// @brief Argument count
        uint32_t argument_count = 0;
        /// @brief enforced type at type inference
        Type *return_type_hint = nullptr;
        /// @brief Callee function
        Function *callee = nullptr;

        /// @brief Empty constructor for internal usage of CallExpression
        /// @remarks Do not use the constructor to build ASTs
        CallExpression(location loc);
        CallExpression(location loc, Expression *locator, std::vector<Expression *> positional_arguments);

        /// @brief Get the locator expression of the call
        /// @return The locator expression
        Expression *get_locator();

        /// @brief Get the arguments expressions of the call
        std::vector<Expression *> get_arguments();

        /// @brief Get the argument at the specified position
        /// @param pos The position of the argument
        /// @return The expression of the argument
        Expression *get_argument_expr(uint32_t pos);

        /// @brief Get the name of the argument at the specified position
        /// @param pos The position of the argument
        /// @return The identifier of the argument
        Identifier *get_argument_name(uint32_t pos);

        /// @brief Add a named argument to the call expression
        /// @param name The identifier of the argument
        /// @param expr The expression of the argument
        void push_named_argument(Identifier *name, Expression *expr);

        /// @brief Add a positional argument to the call expression
        /// @param expr The expression of the argument
        void push_positional_argument(Expression *expr);

        /// @brief Insert a named argument at the specified argument position (pushing the others to the right)
        /// @param position The position to insert the argument at
        /// @param name The identifier of the argument
        /// @param expr The expression of the argument
        void insert_named_argument(size_t position, Identifier *name, Expression *expr);

        /// @brief Insert a positional argument at the specified argument position (pushing the others to the right)
        /// @param position The position to insert the argument at
        /// @param name The identifier of the argument
        /// @param expr The expression of the argument
        void insert_positional_argument(size_t position, Expression *expr);

        /// @brief Remove the argument at the specified position
        /// @param position The position of the argument to remove
        void remove_argument_at(size_t position);

        /// @brief Get the argument by its name, nullptr if not found
        /// @param name The name of the argument
        /// @return The call expression argument
        CallExpressionArgument *get_argument_by_name(const char *name);

        /// @brief Get the argument by its index, nullptr if not found
        /// @param index The index of the argument
        /// @return The call expression argument
        CallExpressionArgument *get_argument_by_index(uint32_t index);

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void enforce_return_type(Type *ty);

        std::vector<Function *> find_candidates();

        Type *get_type() override;

        void post_attach() override;

        void validate() override;

    protected:
        void _set_type(Type *type) override;
        void _pre_type_inference() override;
    };
    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments);
}