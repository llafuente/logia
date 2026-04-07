#pragma once

#include "ast/node.h"
#include "ast/expr.h"
#include "ast/block.h"
#include "llvm/IR/Type.h"
#include "ast/program.h"

namespace logia::AST
{
    enum class Primitives
    {
        VOID_TY,

        BOOL_TY,

        I8_TY,
        I16_TY,
        I32_TY,
        I64_TY,
        U8_TY,
        U16_TY,
        U32_TY,
        U64_TY,
        F16_TY,
        F32_TY,
        F64_TY,
        F128_TY,

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

    enum class StructPropertyType
    {
        STRUCT_PROPERTY_TYPE_FIELD,
        STRUCT_PROPERTY_TYPE_ALIAS,
        STRUCT_PROPERTY_TYPE_GETTER,
        STRUCT_PROPERTY_TYPE_SETTER,
        STRUCT_PROPERTY_TYPE_METHOD,
    };

    struct Type;
    struct Expression;

    struct FunctionParameters
    {
        char *name;
        Type *type;
        Node *defaultValue;
        FunctionParameters(
            char *_name,
            Type *_type,
            Node *_defaultValue) : name(_name), type(_type), defaultValue(_defaultValue)
        {
        }
    };

    /*
     * `fields`: Named values that are stored in memory inside the structure.
     * `setters` and `getters`: Named values that aren't stored in memory (Syntactic sugar)
     * `alias`: Named values that points to a field, setter or getter and has the same type (Syntactic sugar)
     * `methods`: named function that manipulates the struct, there is an implicit first argument `this`.
     * `properties`: Set of `fields` + `aliases` + `getters` + `setters`
     * `members`: Set of `fields` + `aliases` + `getters` + `setters` + `methods` + `types`.
     */

    struct StructProperty
    {
        char *name;
        char *docstring;
        enum StructPropertyType kind;
        Type *type;
        Expression *default_value;

        bool isField() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD; };
        bool isAlias() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_ALIAS; };
        bool isGetter() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_GETTER; };
        bool isSetter() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_SETTER; };
        bool isMethod() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_METHOD; };
    };

    struct IntegerProperties
    {
    public:
        int bits;
        bool is_signed;
    };

    struct FloatProperties
    {
    public:
        int bits;
    };

    struct FunctionType
    {
    public:
        char *name;
        char *docstring;
        std::vector<FunctionParameters> parameters;
        std::vector<llvm::Type *> parametersIR;
        Type *return_type;

        /**
         * intrinsic are defined in the compiler process or in the module bootstrap
         */
        bool intrinsic;
        Block *body;
        llvm::Function *functionIR;
    };

    struct StructType
    {
    public:
        char *name;
        char *docstring;
        std::vector<StructProperty> properties;
        std::vector<Type *> methods;
    };

    struct LOGIA_EXPORT Type : public Node
    {
    public:
        Primitives primitive = Primitives::VOID_TY;
        // modifiers
        bool readonly = false;

        llvm::Type *ir = nullptr;

        // type properties
        union
        {
            FunctionType Function;
            StructType Struct;
            IntegerProperties Integer;
            FloatProperties Float;
        };

        Type(antlr4::ParserRuleContext *rule, Primitives prim);

        bool isFunction();
        bool isStruct();

        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
        void on_after_attach() override;
    };

    /**
     * Creates a function
     */
    LOGIA_API LOGIA_LEND Type *ast_create_function_type(char *name, Type *return_type);
    /**
     * Creates a intrinsic function
     *
     * REVIEW TODO this may also need to call backkend::add_intrinsic to keep everything sync.
     */
    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, char *name, Type *return_type);
    /**
     * Creates a struct type
     */
    LOGIA_API LOGIA_LEND Type *ast_create_struct_type(char *name);

    /**
     * Adds a parameter to a function
     */
    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, char *param_name, Expression *param_default_value);

    /**
     * Adds a field to struct
     */
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, char *prop_name, Expression *prop_default_value);
}
