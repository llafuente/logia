#pragma once

#include "logia/ast/node.h"
#include "logia/ast/type.h"

namespace logia::AST
{
    struct Expression;
    struct Identifier;

    /// @brief Defines a function parameter
    struct FunctionParameter : Node
    {
        llvm::AllocaInst *alloca_inst = nullptr;
        size_t index = 0;

        FunctionParameter(
            Identifier *name,
            Type *type,
            Node *defaultValue);

        Identifier *get_name();
        Expression *get_default_value();
        bool has_default_value();

        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;

    protected:
        void _set_type(Type *type) override;
    };

    /// @brief Defines a function type, which is a collection of parameters, a body and a return type
    struct LOGIA_EXPORT Function : public Type
    {
    public:
        char *docstring = nullptr;
        /// @brief is an intrinsic function, intrinsics don't have body and are defined outside user program.
        bool is_intrinsic = false;

        std::vector<llvm::Type *> ir_parameters = {};
        llvm::FunctionType *ir_functy = nullptr;
        llvm::Function *ir_func = nullptr;
        llvm::DISubprogram *di_subprogram = nullptr;

        Function(antlr4::ParserRuleContext *rule, Identifier *name, Type *return_type = nullptr, bool is_intrinsic = false);
        ~Function();

        std::vector<FunctionParameter *> get_parameters();

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

        FunctionParameter *get_parameter(uint32_t i);

        int64_t get_parameter_count();

        Identifier *get_parameter_name(uint32_t i);

        FunctionParameter *get_parameter_by_name(const char *name);

        /// @brief Retrives the number of mandatory parameters
        /// @return
        uint32_t get_mandatory_parameters_size();

        /// @brief Retrives the number of optional parameters
        /// @return
        uint32_t get_optional_parameters_size();

        /// @brief Adds a parameter to a function
        void push_parameter(FunctionParameter *param);

        /// @brief Checks if given call is valid
        /// @details Checks if the number of arguments and their types are compatible with the function parameters
        /// Modify callee sorting named arguments and adding default values for optional parameters if needed
        /// @param callee The call expression to check
        void validate_and_fill_call(CallExpression *callee);

        std::string to_string() override;
        std::string get_repr() override;
        void post_attach() override;
        /// @brief generate parameters alloca. Used at FunctionBlock
        void codegen_parameters(logia::Backend *backend);
        void pre_codegen(logia::Backend *backend) override;
        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _pre_type_inference() override;
    };

    struct LOGIA_EXPORT Operator : public Function
    {
        Operator(antlr4::ParserRuleContext *rule, Operators op, Type *return_type = nullptr);
        ~Operator();
    };

    struct LOGIA_EXPORT Intrinsic : public Function
    {
        const char *real_name = nullptr;

        Intrinsic(llvm::Function *ir, const char *real_name, const char *scope_name, Type *return_type, std::vector<Type *> arguments);
        ~Intrinsic();

        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
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
}