#pragma once

#include "logia/ast/node.h"

namespace logia::AST
{
    // forward declaration
    struct Block;
    struct Type;
    struct TypeDef;
    struct Expression;
    struct Identifier;

    /// @brief Base class for all statements
    struct Stmt : Node
    {
        Stmt(location loc);

        std::string to_string() override;

        Type *get_type() override;
    };

    /// @brief Goto statement
    struct GotoStmt : Stmt
    {
        GotoStmt(location loc, Identifier *name);

        /// @brief Returns the identifier of the target label
        /// @return Identifier of the target label
        Identifier *get_identifier();

        /// @brief Returns the name of the target label
        /// @return Name of the target label
        const char *get_name();

        std::string to_string() override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

    protected:
        /// @brief Do nothing
        void _on_set_type(TypeDecl *ty) override;
    };

}
