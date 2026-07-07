#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{

    struct Identifier;
    /// @brief Defines a struct initializer, used for struct literals and struct field default values
    struct StructInitializer : Expression
    {
    public:
        uint32_t values = 0;
        Type *type = nullptr;

        StructInitializer(location loc);

        std::string to_string() override;
        /// @brief Adds a named property
        /// @param locator The identifier for the property
        /// @param value The expression representing the value of the property
        void add_named_property(Identifier *locator, Expression *value);
        /// @brief Adds a positional property
        /// @param value The expression representing the value of the property
        void add_positional_property(Expression *value);
        /// @brief Sets a named property at a specific index
        /// @remarks Property at index should already exist.
        /// @param name The identifier for the property
        /// @param value The expression representing the value of the property
        /// @param index The index at which to set the property
        void set_named_property(Identifier *name, Expression *value, uint32_t index);
        /// @brief Gets the value of a property by its index
        /// @param index The index of the property
        /// @return The expression representing the value of the property
        Expression *get_value_by_index(uint32_t index);
        /// @brief Gets the value of a property by its name
        /// @param name The name of the property
        /// @return The expression representing the value of the property
        Expression *get_value_by_name(const char *name);

        // Inherited via Expression
        llvm::Value *post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        Type *get_type() override;

    protected:
        void _on_set_type(TypeDecl *type) override;
    };

} // namespace logia::AST
