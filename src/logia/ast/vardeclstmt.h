#pragma once

#include "logia/ast/stmt.h"

namespace llvm
{
    class AllocaInst; // #include "llvm/IR/Instructions.h"
}

namespace logia::AST
{
    /// @brief Variable declaration/initialization statement
    struct VarDeclStmt : Stmt
    {
        /// @brief AllocaInst for this variable, populated at pre_codegen and cached for later use
        llvm::AllocaInst *alloca_inst;
        /// @brief Cached type
        TypeDef *type;
        /// @brief Variable declaration without type, expr is mandatory
        VarDeclStmt(location loc, Identifier *id, Expression *expr);
        /// @brief Variable declaration with type, expr is optional
        VarDeclStmt(location loc, Identifier *id, TypeDef *type, Expression *expr);

        /// @brief Returns initializer expression
        /// @return
        Expression *get_expr();

        /// @brief  Returns variable name
        /// @return
        const char *get_name();

        /// @brief Returns the identifier of the variable
        /// @return
        Identifier *get_identifier();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        /// @brief Returns the type of the variable
        /// @remarks While not necessary (logia do not support chained vardecl) could be usefull when referencing the variable
        /// @return Type of the variable
        Type *get_type() override;

    protected:
        /// @brief Sets type based on declaration or expression type
        void _pre_type_inference() override;
        /// @brief Sets type
        void _on_set_type(TypeDecl *ty) override;
    };
} // namespace logia::AST
