#include "logia/ast/expr.h"

namespace logia::AST
{
    struct VarDeclStmt;
    struct Function;

    /// @brief Defines an identifier, used for variable names, function names, struct field names, etc.
    struct Identifier : Expression
    {
        /// @brief The name of the identifier
        const char *identifier;
        /// @brief Pointed type
        Type *type = nullptr;
        /// @brief Pointed declaration
        Node *decl = nullptr;

        Identifier(location loc, const char *identifier);
        bool operator==(const char *id);
        bool operator==(const Identifier *id);

        /// @brief Gets the variable declaration associated with this identifier
        /// @return
        VarDeclStmt *get_var_decl();
        /// @brief Gets the function declaration associated with this identifier
        /// @return
        Function *get_function_decl();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        Type *get_type() override;

        void post_attach() override;

        Node *resolve() override;

        void set_declaration(Node *node);

        bool is_empty();

    protected:
        void _set_type(Type *t) override;
        void _pre_type_inference() override;
    };

    /// @brief Creates an identifier
    /// @param name The name of the identifier
    /// @return The created identifier
    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name);

} // namespace logia::AST
