#pragma once

#include "logia/ast/stmt.h"

namespace logia::AST
{
    struct Expression;
    /// @brief counter to generate unique if statement labels
    extern uint64_t if_stmt_count;

    // references: https://clang.llvm.org/doxygen/classclang_1_1IfStmt.html

    /// @brief if statement, with optional else
    struct IfStmt : Stmt
    {
        IfStmt(location loc, Expression *condition);
        /// @brief Retrieves the condition expression
        /// @return
        Expression *get_condition();

        /// @brief Retrieves the then block
        /// @return
        Block *get_then();
        /// @brief Retrieves the else block
        /// @return
        Block *get_else();
        /// @brief Retrieves the continue (exit) block
        /// @return
        Block *get_continue_block();

        std::string to_string() override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

    protected:
        void _post_type_inference() override;
        void _on_set_type(TypeDecl *t) override;
    };
}