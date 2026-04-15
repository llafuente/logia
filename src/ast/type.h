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
        NONE,

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

    /// @brief Defines a function parameter
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
    /// @brief Base class for all types
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

        // TODO
        /// @brief Checks if this type is layout equivalent to another type, this is used for struct field access and function parameters matching
        bool is_layout_equivalent(Type *other);
        // TODO
        /// @brief Checks if this type is type equivalent to another type
        bool is_type_equivalent(Type *other);

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
        void post_attach() override;

    protected:
        /// @brief registers this type into block
        /// @param name
        void __register_type(const char *name);
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

        void post_attach() override;
    };

    // REVIEW method aliasing ?
    /// @brief Defines a type alias, used for struct field alias
    struct StructAlias : Type
    {
    public:
        const char *docstring;
        StructAlias(antlr4::ParserRuleContext *rule, Identifier *from, Identifier *to, const char *_docstring = nullptr);

        std::string to_string() override;
        Type *get_type() override;
        /// @brief Returns the source identifier of the alias
        /// @return
        Identifier *get_from();
        /// @brief Returns the target identifier of the alias
        /// @return
        Identifier *get_to();
    };

    /// @brief Defines a field (something that is stored in memory) within a struct
    struct StructField : Type
    {
        const char *docstring;

        StructField(
            antlr4::ParserRuleContext *rule,
            Identifier *name,
            Type *type,
            Expression *default_value,
            const char *docstring = nullptr);

        std::string to_string() override;
        Type *get_type() override;
        Identifier *get_name();
        Expression *get_default_value();
    };

    // TODO setter
    // TODO getter
    // TODO method, because we need to modify the function definition before codegen to add "this"
    // REVIEW type decl inside a struct ?

    /// @brief Defines a struct type, which is a collection of fields, alias, setters, getters and methods
    struct LOGIA_EXPORT Struct : public Type
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

        Struct(antlr4::ParserRuleContext *rule, Identifier *id);

        /// @brief Retrives struct identifier name as C string
        /// @return
        const char *get_name();

        /// @brief Retrives struct identifier name
        /// @return
        Identifier *get_identifier();

        /// @brief Sets the struct identifier, call this once or throws!
        void set_identifier(Identifier *id);

        /// @brief Adds a field to the struct
        void add_field(
            antlr4::ParserRuleContext *rule,
            Identifier *name,
            Type *type,
            Expression *default_value,
            const char *docstring);

        /// @brief Adds an alias to the struct
        void add_alias(antlr4::ParserRuleContext *rule, Identifier *from, Identifier *to, const char *docstring);

        /// @brief Retrieves the target identifier of an alias
        /// @param from The source identifier of the alias
        /// @return The target identifier of the alias
        Identifier *get_alias_to(Identifier *from);

        /// @brief Retrieves a field by its identifier
        /// @param id The identifier of the field
        /// @return The field corresponding to the identifier
        StructField *get_field(Identifier *id);

        /// @brief Retrieves the index of a field by its identifier, used for struct field access codegen
        /// @param id The identifier of the field
        /// @return The index of the field
        uint32_t get_field_index(Identifier *id);

        /// @brief Retrieves the type of a field by its identifier
        /// @param id The identifier of the field
        /// @return The type of the field
        Type *get_field_type(Identifier *id);

        std::string to_string() override;

        void post_attach() override;

        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
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

        Type *get_type() override;
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Node *resolve() override;
    };

    /// @brief Defines a function type, which is a collection of parameters, a body and a return type
    struct LOGIA_EXPORT Function : public Type
    {
    public:
        char *docstring;
        std::vector<FunctionParameters> parameters;
        std::vector<llvm::Type *> parametersIR;

        /// @brief is an intrinsic function, intrinsics don't have body and are defined outside user program.
        bool is_intrinsic;
        llvm::Function *functionIR;

        Function(antlr4::ParserRuleContext *rule, Identifier *id, Type *return_type = nullptr, bool is_intrinsic = false);
        ~Function();

        /// @brief Retrives function identifier name as C string
        /// @return
        const char *get_name();

        /// @brief Retrives function identifier name
        Identifier *get_identifier();

        /// @brief Retrives function return type
        /// @return
        Type *get_return_type();

        /// @brief Retrives function body
        /// @return
        Block *get_body();

        /// @brief Retrives the number of mandatory parameters
        /// @return
        uint32_t get_mandatory_parameters_size();

        /// @brief Retrives the number of optional parameters
        /// @return
        uint32_t get_optional_parameters_size();

        /// @brief Adds a parameter to a function
        /// @param param_type The type of the parameter
        /// @param param_name
        /// @param param_default_value
        void add_param(Type *param_type, Identifier *param_name, Expression *param_default_value);

        /// @brief Checks if given call is valid
        /// @details Checks if the number of arguments and their types are compatible with the function parameters
        /// Modify callee sorting named arguments and adding default values for optional parameters if needed
        /// @param callee The call expression to check
        void check_call(CallExpression *callee);

        std::string to_string() override;
        void post_attach() override;
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

    /// @brief Retrieves node real type
    /// @param node
    /// @return
    LOGIA_API LOGIA_LEND Type *ast_resolve_type(Node *node);
}
