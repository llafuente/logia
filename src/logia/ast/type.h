#pragma once

#include "logia/ast/node.h"
#include "logia/ast/operators.h"

namespace llvm
{
    class PointerType;
    class DIType; // #include "llvm/DIType.h"
    class Type;
}

namespace logia::AST
{
    struct Identifier;
    struct Type;
    struct Expression;
    struct CallExpression;
    struct ConstExpression;

    enum class Primitives
    {
        NONE,

        VOID_TY,

        INTEGER_TY,

        FLOATING_POINT_TY,

        // aliases but very special...
        // Int,
        // size,
        // ptrdiff,
        // address,
        // Typeid,

        PTR_TY,
        // Enum,
        STRUCT_TY,
        // interface,
        FUNCTION_TY,
        // this is a pointer to function, but we may need to declare at this level
        // callable,
        ANY_TY,
    };

    /**
     * Converts Primitives enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim);

    /// @brief Base class for all types
    struct LOGIA_EXPORT Type : public Node
    {
    public:
        // modifiers
        bool readonly = false;

        Type(location loc);
        ~Type();

        virtual std::string get_repr() = 0;

    protected:
        void _on_set_type(TypeDecl *type) override;
        /// @brief registers this type into block
        /// @param name
        void __register_type(const char *name);
    };

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

    /// @brief Represents an integer
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Integer : public TypeDecl
    {
    public:
        uint32_t bits;
        bool is_signed;

        Integer(bool is_signed, int bits);
        ~Integer();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };

    /// @brief Represents an floating point number
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Float : public TypeDecl
    {
    public:
        int bits;

        Float(int bits);
        ~Float();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };

    /// @brief Represents void aka no-type
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Void : public TypeDecl
    {
    public:
        Void();
        ~Void();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;
        void validate() override;
    };

    /// @brief Represents an opaque pointer
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Pointer : public TypeDecl
    {
    public:
        llvm::PointerType *ir_ptype;

        Pointer();
        ~Pointer();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;
        void validate() override;
    };

    /// @brief Represents a referente to a single object (it's a pointer that chan't move!!)
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Ref : public Pointer
    {
    public:
        TypeDecl *pointee;

        Ref(TypeDecl *pointee);
        ~Ref();

        TypeDecl *get_pointee();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };

    struct Vec : public Ref
    {
    public:
        Type *pointee;
        size_t size = 0;

        Vec(Type *pointee);
        ~Vec();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;
    };

    // TODO implement templates
    // TODO implement static values
    // TODO aggregates ?
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

        TypeDef();
        ~TypeDef();

        void add_locator(Identifier *name);

        Type *get_type() override;

        std::string to_string() override;

        std::string get_repr() override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

    protected:
        void _early_type_inference() override;
        void _pre_type_inference() override;
    };

    struct InferType : Type
    {
        InferType();
        ~InferType();

        std::string get_repr() override;

        std::string to_string() override;

        void pre_codegen(logia::Backend *backend) override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

    protected:
        void _on_set_type(TypeDecl *t) override;
    };
}
