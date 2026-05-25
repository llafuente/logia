#pragma once

#include "logia/ast/node.h"
#include "logia/ast/expr.h"
#include "logia/ast/block.h"
#include "llvm/IR/Type.h"
#include "logia/ast/program.h"
#include "logia/ast/operators.h"

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
    };

    /**
     * Converts Primitives enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim);

    /// @brief Base class for all types
    struct LOGIA_EXPORT Type : public Node
    {
    public:
        Primitives primitive = Primitives::VOID_TY;
        // modifiers
        bool readonly = false;

        llvm::Type *ir_type = nullptr;
        llvm::DIType *di_type = nullptr;

        Type(antlr4::ParserRuleContext *rule, Primitives prim);
        ~Type();

        std::string to_string() override;
        virtual std::string get_repr(); // TODO do it pure virtual asap :)

        // TODO
        /// @brief Checks if this type is layout equivalent to another type, this is used for struct field access and function parameters matching
        bool is_layout_equivalent(Type *other);
        // TODO
        /// @brief Checks if this type is type equivalent to another type
        bool is_type_equivalent(Type *other);

        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
        void post_attach() override;

    protected:
        void _set_type(Type *type);
        /// @brief registers this type into block
        /// @param name
        void __register_type(const char *name);
    };

    /// @brief Represents an integer
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Integer : public Type
    {
    public:
        int bits;
        bool is_signed;
        // NOTE Integer is a primitive, won't have rule
        Integer(bool is_signed, int bits);
        ~Integer();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    /// @brief Represents an floating point number
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Float : public Type
    {
    public:
        int bits;
        // NOTE Integer is a primitive, won't have rule
        Float(int bits);
        ~Float();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    /// @brief Represents void aka no-type
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Void : public Type
    {
    public:
        // NOTE Integer is a primitive, won't have rule
        Void();
        ~Void();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    /// @brief Represents an opaque pointer
    /// @remarks llvm is created at pre_codegen, so it will be available anytime!
    struct Pointer : public Type
    {
    public:
        // NOTE Integer is a primitive, won't have rule
        Pointer();
        ~Pointer();

        std::string to_string() override;

        std::string get_repr() override;

        void pre_codegen(logia::Backend *backend) override;

        void post_attach() override;
    };

    // TODO implement templates
    // TODO implement static values
    // TODO aggregates ?
    /// @brief Node to resolve a type by name
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
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Node *resolve() override;
    };

    struct InferType : Type
    {
        InferType();
        ~InferType();
        std::string get_repr() override;
        std::string to_string() override;
        void pre_codegen(logia::Backend *backend) override;
        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _set_type(Type *t) override;
    };
}
