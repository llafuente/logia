#include "logia/ast/expr.h"
#include "logia/ast/operators.h"

namespace logia::AST
{
    struct CallExpression;

    /// @brief counter to generate unique block labels
    extern uint64_t expr_logical_count;

    /// @brief Binary expression, used for binary operators
    /// @details
    /// Binary/Unary expressions in logia are implemented the following way
    /// assignament -> check lhs and rhs are compatible (casting rhs if possible) -> use store instruction (TODO structs)
    /// logical and -> check lhs and rhs are booleans -> create the jumps/blocks and phi
    /// logical or -> check lhs and rhs are booleans -> create the jumps/blocks and phi
    /// the reset are calls to functions defined in intrinsics.ll
    /// @remarks Implemented as a call expression to be able to resolve operator overloads and use intrinsics for codegen
    struct BinaryExpression : Expression
    {
        /// @brief The operator of the binary expression
        Operators op = Operators::DEFAULT;
        /// @brief Replacement call expression for this binary expression, optional.
        CallExpression *call_expr = nullptr;
        /// @brief Cached type of the binary expression (call expression return type)
        Type *type = nullptr;

        BinaryExpression(location loc, Expression *left, Operators op, Expression *right);

        /// @brief Gets the left-hand side expression of the binary expression
        /// @return The left-hand side expression
        Expression *get_left();
        /// @brief Gets the right-hand side expression of the binary expression
        /// @return The right-hand side expression
        Expression *get_right();

        /// @brief A binary expression is constant if left and right are constants
        /// @return true if constant, false otherwise
        maybe_semantic_error can_execute() override;
        /// @brief Executes the binary expression if it is constant at compile time
        /// @return Returns the result of the expression execution
        ConstExpression *execute() override;

        std::string to_string() override;

        Type *get_type() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void post_attach() override;

        void validate() override;

    protected:
        void _set_type(Type *type) override;
        void _pre_type_inference() override;
        void _post_type_inference() override;

    private:
        void __enforce_assignament_type(Type *left_ty, Type *right_ty);
    };

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, Operators op, Expression *right);
}