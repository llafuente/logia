#pragma once

#include "logia/ast/types/type.h"

namespace logia::AST
{
    // #include "logia/ast/types/templatedef.h"
    struct TemplateDef;

    /// @brief Node to resolve a type by name(s)
    /// @remarks Due to some implementation collisions, TypeDef need to be resolved asap, this means
    /// that the real "type_inference" is done at on_after_attach
    struct LOGIA_EXPORT TypeDef : public Type
    {
        /// @brief type modification not allowed
        unsigned char is_readonly : 1 = false;
        /// @brief lend memory, creates memory, lhs is the owner now.
        // TODO should be used only at return type ? a function can return inside parameters, needed?
        unsigned char lend_memory : 1 = false;
        /// @brief own memory, transfer ownership or delete the memory
        unsigned char own_memory : 1 = false;
        /// @brief uninitialized memory, marks memory as need to be initialized -> call constructor on it before use
        unsigned char uninitialized_memory : 1 = false;
        /// @brief Type is optional / null / undefined is a valid value.
        unsigned char is_optional : 1 = false;

        TypeDef(location loc);
        ~TypeDef();

        void add_identifier(Identifier *name);

        void add_type_def(TypeDef *td);

        void add_template_def(TemplateDef *tdef);

        Type *get_type() override;

        std::string to_string() override;

        std::string get_repr() override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        /// @brief searches in the scope for given ident
        TypeDecl *resolve_identifier(Identifier *ident);

        /// @brief searches in thegiven type a property by name
        TypeDecl *resolve_property(TypeDecl *tyd, Identifier *ident);

        bool type_inference(size_t pass_id) override;
    };

}