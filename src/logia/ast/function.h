#pragma once

#include "logia/ast/node.h"
#include "logia/ast/type.h"

namespace llvm
{
    class AllocaInst;   // #include "llvm/AllocaInst.h"
    class FunctionType; // #include "llvm/IR/DerivedTypes.h"
    class PointerType;  // #include "llvm/IR/DerivedTypes.h"
    class Function;     // #include "llvm/IR/Function.h"
    class DISubprogram;
    class Type;
}

namespace logia::AST
{
    struct Expression;
    struct Identifier;
    struct Program;

    /// @brief Defines a function parameter
    struct FunctionParameter : Node
    {
        char *docstring = nullptr;
        /// @brief Alloca instruction for this parameter, populated at codegen and used for variable access
        llvm::AllocaInst *alloca_inst = nullptr;

        /// @brief Index of this parameter in the function parameter list
        /// @remarks Handled by Function
        size_t index = 0;

        /// @brief Constructs a function parameter
        /// @param name The name of the parameter
        /// @param type The type of the parameter
        /// @param defaultValue The default value of the parameter, if any
        FunctionParameter(
            Identifier *name,
            Type *type,
            Node *defaultValue = nullptr);

        /// @brief Retrieves function name
        Identifier *get_name();

        /// @brief Retrieves the default value of the function parameter
        /// @return The default value expression, if any
        Expression *get_default_value();

        /// @brief Checks if the function parameter has a default value
        /// @return True if the parameter has a default value, false otherwise
        bool has_default_value();

        std::string to_string() override;

        llvm::Value *post_codegen(logia::Backend *backend) override;

        void on_after_attach() override;

        void validate() override;

        /// @brief Retrieves function type (self)
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
        /// @brief marks function as method of a struct. First parameter will be a ref to the struct!
        bool is_method = false;
        /// @brief LLVM function parameter types
        std::vector<llvm::Type *> ir_parameters = {};
        /// @brief LLVM function type
        llvm::FunctionType *ir_functy = nullptr;
        /// @brief LLVM Function declaration
        llvm::Function *ir_func = nullptr;
        /// @brief LLVM Debug Information Subprogram
        llvm::DISubprogram *di_subprogram = nullptr;

        /// @brief Constructs a function
        /// @param rule The parser rule context
        /// @param name The name of the function
        /// @param return_type The return type of the function
        /// @param is_intrinsic Whether the function is intrinsic or not, intrinsic functions don't have body and are defined outside user program.
        Function(location loc, Identifier *name, Type *return_type = nullptr, bool is_intrinsic = false);
        ~Function();

        /// @brief Retrieves the function parameters
        /// @return A vector of function parameters
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

        /// @brief Retrieves a function parameter by index
        /// @param i The index of the parameter
        /// @return The function parameter at the specified index
        FunctionParameter *get_parameter(uint32_t i);

        /// @brief Retrieves the number of function parameters
        /// @return The number of function parameters
        int64_t get_parameter_count();

        /// @brief Retrieves the name of a function parameter by index
        /// @param i The index of the parameter
        /// @return The name of the function parameter at the specified index
        Identifier *get_parameter_name(uint32_t i);

        /// @brief Retrieves a function parameter by name
        /// @param name The name of the parameter
        /// @return The function parameter with the specified name
        FunctionParameter *get_parameter_by_name(const char *name);

        /// @brief Retrives the number of mandatory parameters
        /// @return
        uint32_t get_mandatory_parameters_size();

        /// @brief Retrives the number of optional parameters
        /// @return
        uint32_t get_optional_parameters_size();

        /// @brief Adds a parameter to a function
        void push_parameter(FunctionParameter *param);

        /// @brief Insert a parameter at given position
        void insert_parameter(size_t position, FunctionParameter *param);

        /// @brief Checks if given call is valid
        /// @details Checks if the number of arguments and their types are compatible with the function parameters
        /// Modify callee sorting named arguments and adding default values for optional parameters if needed
        /// @param callee The call expression to check
        void validate_and_fill_call(CallExpression *callee);

        std::string to_string() override;
        std::string get_repr() override;
        void on_after_attach() override;
        void validate() override;
        /// @brief generate parameters alloca. Used at FunctionBlock
        void codegen_parameters(logia::Backend *backend);
        void pre_codegen(logia::Backend *backend) override;
        llvm::Value *post_codegen(logia::Backend *backend) override;

    protected:
        void _pre_type_inference() override;
    };

    /// @brief Defines an operator, which is a special kind of function a name based on given operator
    struct LOGIA_EXPORT Operator : public Function
    {
        /// @brief The operator kind
        Operators op;

        Operator(location loc, Operators op, Type *return_type = nullptr);
        ~Operator();
    };

    /// @brief Defines an intrinsic function, which is a special kind of function that is defined outside user program and implemented directly in LLVM IR or exposed by the compiler
    struct LOGIA_EXPORT Intrinsic : public Function
    {
        /// @brief The real name of the intrinsic function, used to link with the correct LLVM IR function or to expose it to comptime/jit
        const char *real_name = nullptr;

        /// @brief Constructs an intrinsic function
        /// @param ir The LLVM IR Function
        /// @param real_name The real name of the intrinsic function
        /// @param scope_name The scope name of the intrinsic function
        /// @param return_type The return type of the intrinsic function
        /// @param arguments The argument types of the intrinsic function
        Intrinsic(llvm::Function *ir, const char *real_name, const char *scope_name, Type *return_type, std::vector<Type *> arguments);
        ~Intrinsic();

        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
    };
    /*
        /// @brief Defines an intrinsic function that will be resolved as operator overloading
        struct LOGIA_EXPORT OperatorIntrinsic : public Intrinsic
        {
            /// @brief The real name of the intrinsic function, used to link with the correct LLVM IR function or to expose it to comptime/jit
            const char *real_name = nullptr;

            /// @brief Constructs an intrinsic function
            /// @param ir The LLVM IR Function
            /// @param real_name The real name of the intrinsic function
            /// @param scope_name The scope name of the intrinsic function
            /// @param return_type The return type of the intrinsic function
            /// @param arguments The argument types of the intrinsic function
            OperatorIntrinsic(llvm::Function *ir, const char *real_name, const Operators op, Type *return_type, std::vector<Type *> arguments);
            ~OperatorIntrinsic();

            std::string to_string() override;
            llvm::Value *post_codegen(logia::Backend *backend) override;
        };
    */
}