#pragma once

#include "logia/ast/types/type.h"

namespace logia::AST
{

    /// @brief Defines a type declaration
    /// @details NOTE that TypeDecl can't be child of another TypeDecl
    struct TypeDecl : public Type
    {
        Primitives primitive = Primitives::VOID_TY;

        llvm::Type *ir_type = nullptr;
        llvm::DIType *di_type = nullptr;

        TypeDecl(location loc, Primitives prim);
        ~TypeDecl();

        virtual TypeDecl *get_effective_type_decl();

        std::string to_string() override;
        // TODO virtual ?
        TypeDecl *get_reference_to();

        /// @brief Checks that after this pass we have ir_type & di_type
        void pre_codegen(logia::Backend *backend) override;
    };

}