#pragma once

#include "logia/ast/node.h"
#include "logia/ast/semantic_error.h"

namespace logia
{
    struct Backend;
}

namespace logia::AST
{
    struct ConstExpression;
    /// @brief Base expression
    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(location loc);
        Expression(const Expression &other);

        std::string to_string() override;

        /// @brief Determines if the expression is constant at compile time
        /// @return true if constant, false otherwise
        virtual maybe_semantic_error can_execute();

        /// @brief Executes the expression if it is constant at compile time
        /// @return Returns the result of the expression execution
        virtual ConstExpression *execute();

        void post_codegen(logia::Backend *backend) override;

        /// @brief Retrieves codegen value
        /// @details
        /// Should be called inside post_codegen stage, before it's expected to fail.
        llvm::Value *get_codegen_value(logia::Backend *backend);
        /// @brief Sets codegen value and attach debug information to the instruction
        /// @details
        /// Disallow reentry
        /// @param(backend) send nullptr if you don't want debug information (required for constants!)
        void set_codegen_value(logia::Backend *backend, llvm::Value *value);

    private:
        /// @brief codegen result
        llvm::Value *cg_value = nullptr;
    };
}