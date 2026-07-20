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

        std::string to_code(size_t ident = 0) override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        bool type_inference(size_t pass_id) override;

    protected:
        void _on_set_type(TypeDecl *type) override;

    private:
        void __enforce_assignament_type(TypeDecl *left_ty, TypeDecl *right_ty);
    };
}