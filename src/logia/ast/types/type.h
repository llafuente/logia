#pragma once

#include "logia/ast/node.h"
#include "logia/ast/operators.h"
#include "logia/ast/types/primitives.h"

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

    struct InferType : Type
    {
        InferType();
        ~InferType();

        std::string get_repr() override;

        std::string to_string() override;
        std::string to_code(size_t ident = 0) override { return ""; }

        void pre_codegen(logia::Backend *backend) override;

        void post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

    protected:
        void _on_set_type(TypeDecl *t) override;
    };
}
