#pragma once

#include "logia/ast/type.h"

namespace llvm
{
    class StructType;
}

namespace logia::AST
{
    struct Function;

    // REVIEW method aliasing ?
    /// @brief Defines a type alias, used for struct field alias
    struct StructAlias : Node
    {
    public:
        const char *docstring;

        Node *target;

        StructAlias(location loc, Identifier *from, Identifier *to, const char *_docstring = nullptr);

        std::string to_string() override;
        Type *get_type() override;
        /// @brief Returns the source identifier of the alias
        /// @return
        Identifier *get_from();
        /// @brief Returns the target identifier of the alias
        /// @return
        Identifier *get_to();

        void on_after_attach() override;

        void validate() override;

    protected:
        void _on_set_type(TypeDecl *ty) override;
    };

    /// @brief Defines a field (something that is stored in memory) within a struct
    struct StructField : Node
    {
        const char *docstring;

        uint32_t index;

        StructField(
            location loc,
            uint32_t index,
            Identifier *name,
            Type *type,
            Expression *default_value = nullptr,
            const char *docstring = nullptr);

        std::string to_string() override;
        Type *get_type() override;
        Identifier *get_name();
        Expression *get_default_value();

        void on_after_attach() override;

        void validate() override;

    protected:
        void _pre_type_inference() override;
        void _on_set_type(TypeDecl *ty) override;
    };

    // TODO setter
    // TODO getter
    // TODO method, because we need to modify the function definition before codegen to add "this"
    // REVIEW type decl inside a struct ?

    /// @brief Defines a struct type, which is a collection of fields, alias, setters, getters and methods
    struct LOGIA_EXPORT Struct : public TypeDecl
    {
        char *docstring;
        // TODO remove!
        std::vector<Type *> methods;
        /// @brief number of fields
        uint32_t field_count = 0;
        /// @brief number of aliases
        uint32_t alias_count = 0;
        /// @brief number of methods
        uint32_t method_count = 0;
        /// @brief number of methods
        uint32_t getter_count = 0;
        /// @brief number of methods
        uint32_t setter_count = 0;

        llvm::StructType *struct_type = nullptr;

        Struct(location loc, Identifier *name);

        /// @brief Retrives struct identifier name as C string
        /// @return
        const char *get_name();

        /// @brief Retrives struct identifier name
        /// @return
        Identifier *get_identifier();

        /// @brief Adds a field to the struct
        void add_field(
            location loc,
            Identifier *name,
            Type *type,
            Expression *default_value = nullptr,
            const char *docstring = nullptr);

        /// @brief Adds an alias to the struct
        void add_alias(location loc, Identifier *from, Identifier *to, const char *docstring);

        /// @brief Adds given function to the method
        /// @remarks It will add "this" as first parameter
        void add_method(Function *fn);

        /// @brief Retrieves the target identifier of an alias
        /// @param from The source identifier of the alias
        /// @return The target identifier of the alias
        Identifier *get_alias_to(const char *from);

        /// @brief Retrieves a field by its identifier or throws a semantic_error if not found
        /// @param id The identifier of the field
        /// @return The field corresponding to the identifier
        StructField *get_field(Identifier *id);

        /// @brief Retrieves a field by its identifier
        /// @param id The identifier of the field
        /// @return The field corresponding to the identifier
        StructField *get_field(const char *id);

        Node *get_property(const char *id);

        /// @brief Retrieves the type of a field by its identifier
        /// @param id The identifier of the field
        /// @return The type of the field
        Type *get_field_type(Identifier *id);

        StructField *get_field_by_index(uint32_t index);

        std::string to_string() override;

        std::string get_repr() override;

        void on_after_attach() override;

        void validate() override;
        void pre_codegen(logia::Backend *backend) override;
        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _pre_type_inference() override;
    };
} // namespace logia::AST
