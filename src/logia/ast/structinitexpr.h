#pragma once

#include "logia/ast/expr.h"

namespace logia::AST
{
    /// @brief Defines a struct initializer, used for struct literals and struct field default values
    struct StructInitializer : Expression
    {
    public:
        uint32_t values = 0;
        Type *type = nullptr;

        StructInitializer(antlr4::ParserRuleContext *rule);

        std::string to_string() override;

        void add_named_property(Identifier *locator, Expression *value);
        void add_positional_property(Expression *value);
        void set_named_property(Identifier *name, Expression *value, uint32_t index);

        Expression *get_value_by_index(uint32_t index);
        Expression *get_value_by_name(const char *name);

        // Inherited via Expression
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;

    protected:
        void _set_type(Type *type) override;
    };

} // namespace logia::AST
