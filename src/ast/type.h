#pragma once

#include "ast/node.h"
#include "ast/expr.h"
#include "ast/block.h"
#include "llvm/IR/Type.h"
#include "ast/program.h"

namespace logia::AST
{
    struct Identifier;
    struct Type;
    struct Expression;

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

    struct FunctionParameters
    {
        Identifier *name;
        Type *type;
        Node *defaultValue;
        FunctionParameters(
            Identifier *_name,
            Type *_type,
            Node *_defaultValue) : name(_name), type(_type), defaultValue(_defaultValue)
        {
        }
    };

    struct FloatProperties
    {
    public:
        int bits;
    };

    struct LOGIA_EXPORT Type : public Node
    {
    public:
        Primitives primitive = Primitives::VOID_TY;
        // modifiers
        bool readonly = false;

        llvm::Type *llvm_type = nullptr;

        // type properties
        union
        {
            FloatProperties Float;
        };

        Type(antlr4::ParserRuleContext *rule, Primitives prim);
        ~Type();

        bool isFunction();
        bool isStruct();

        std::string to_string() override;
        virtual std::string get_repr(); // TODO do it pure virtual asap :)

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
        void on_after_attach() override;
    };

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

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        void on_after_attach() override;
    };

    struct StructAlias
    {
    public:
        Identifier *from;
        Identifier *to;
        char *docstring;
        StructAlias(Identifier *_from, Identifier *_to, char *_docstring = nullptr) : from(_from), to(_to), docstring(_docstring) {}
    };

    struct StructField
    {
        Identifier *name;
        Type *type;
        Expression *default_value;
        char *docstring;

        StructField(
            Identifier *_name,
            Type *_type,
            Expression *_default_value, char *_docstring = nullptr) : docstring(_docstring), name(_name), type(_type), default_value(_default_value) {}
    };

    /*
     * `fields`: Named values that are stored in memory inside the structure.
     * `setters` and `getters`: Named values that aren't stored in memory (Syntactic sugar)
     * `alias`: Named values that points to a field, setter or getter and has the same type (Syntactic sugar)
     * `methods`: named function that manipulates the struct, there is an implicit first argument `this`.
     * `properties`: Set of `fields` + `aliases` + `getters` + `setters`
     * `members`: Set of `fields` + `aliases` + `getters` + `setters` + `methods` + `types`.
     */

    struct LOGIA_EXPORT Struct : public Type
    {
        char *docstring;
        std::vector<StructField> fields;
        std::vector<StructAlias> aliases;
        std::vector<Type *> methods;

        Struct(antlr4::ParserRuleContext *rule, Identifier *id);

        char *get_name();
        Identifier *get_identifier();

        /**
         * Adds a field to struct
         */
        void add_field(Type *prop_type, Identifier *prop_name, Expression *prop_default_value = nullptr);
        void add_alias(Identifier *from, Identifier *to);
        Identifier *get_alias_to(Identifier *from);
        uint32_t get_field_index(Identifier *id);

        std::string to_string() override;
        void on_after_attach() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct LOGIA_EXPORT Function : public Type
    {
    public:
        char *docstring;
        std::vector<FunctionParameters> parameters;
        std::vector<llvm::Type *> parametersIR;

        /**
         * intrinsic are defined in the compiler process or in the module bootstrap
         */
        bool intrinsic;
        llvm::Function *functionIR;

        Function(antlr4::ParserRuleContext *rule, Identifier *id, Type *return_type = nullptr, bool is_intrinsic = false);
        ~Function();

        char *get_name(); // shortcut
        Identifier *get_identifier();
        Type *get_return_type();
        Block *get_body();
        uint32_t get_mandatory_parameters_size();
        uint32_t get_optional_parameters_size();

        /// @brief Adds a parameter to a function
        /// @param param_type
        /// @param param_name
        /// @param param_default_value
        void add_param(Type *param_type, Identifier *param_name, Expression *param_default_value);

        std::string to_string() override;
        void on_after_attach() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    /**
     * Creates a function
     */
    LOGIA_API LOGIA_LEND Function *ast_create_function_type(Identifier *name, Type *return_type);
    /**
     * Creates a intrinsic function
     *
     * REVIEW TODO this may also need to call backkend::add_intrinsic to keep everything sync.
     */
    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, Identifier *id, Type *return_type);
    /**
     * Creates a struct type
     */
    LOGIA_API LOGIA_LEND Struct *ast_create_struct_type(Identifier *id);
}
