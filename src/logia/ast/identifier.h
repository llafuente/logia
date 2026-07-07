#include "logia/ast/expr.h"

namespace logia::AST
{
    struct VarDeclStmt;
    struct Function;

    /// @brief Defines an identifier, used for variable names, function names, struct field names, etc.
    struct Identifier : Expression
    {
        /// @brief Whether this identifier should be resolved in type inference phase
        bool resolve = true;
        /// @brief Whether this identifier should be resolved to a unique declaration, if false it can be used for member access and function call where multiple declaration can be valid
        bool resolve_unique = true;
        /// @brief The name of the identifier
        const char *identifier;
        /// @brief List of candidate declarations for this identifier
        std::vector<Node *> decl_candidates = {};
        /// @brief Pointed declaration
        Node *decl = nullptr;

        Identifier(location loc, const char *identifier);
        Identifier::Identifier(const Identifier &other);
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

        void on_after_attach() override;

        void validate() override;

        void set_declaration(Node *node);

        bool is_empty();

    protected:
        void _on_set_type(TypeDecl *t) override;
        void _pre_type_inference() override;
    };

} // namespace logia::AST
