#pragma once

#include "antlr4-runtime.h"
#include "backend.h"
#include <cstdlib> // for itoa
#include "utils.h"

namespace logia::AST
{
    // foward declaration
    struct Body;
    struct Type;
    struct Expression;

    enum ast_types : uint32_t
    {
        // expression flag
        EXPRESSION = (1 << 31),
        // statement flag
        STMT = (1 << 30),
        // type flag
        TYPE = (1 << 29),
        // body flag
        BODY = (1 << 28),

        // all flag bits, to remove them if necessary
        ALL_FLAGS = EXPRESSION | STMT | TYPE | BODY,

        // types
        PROGRAM = (1 << 1),
        FUNCTION = (1 << 2),
        BLOCK = (1 << 3),
        CALL_EXPRESSION = (1 << 4),
        STRING_LITERAL = (1 << 5),
        FLOAT_LITERAL = (1 << 6),
        INTEGER_LITERAL = (1 << 7),
        IDENTIFIER = (1 << 8),

        RETURN_STMT = (1 << 9),
        VAR_DECL_STMT = (1 << 10),
        IF_STMT = (1 << 11),
        GOTO_STMT = (1 << 12),
        // NOTE: if modified -> ast_types_to_string
    };

    // Homogenous AST node type
    struct LOGIA_EXPORT Node
    {
    public:
        /**
         * TODO
         */
        antlr4::ParserRuleContext *rule = nullptr;
        /**
         * Node type to be able to cast from node back to the real type
         */
        ast_types type = (ast_types)0;
        /**
         * parent node to traverse to root
         */
        Node *parentNode = nullptr;
        /**
         * Avoid children modification (from API uknow :)
         */
        bool freezed = false;
        /**
         * My beatiful children, and some not so beatiful.
         */
        std::vector<Node *> children = {}; // normalized list of children

        Node(antlr4::ParserRuleContext *rule, ast_types type, Node *parentNode)
        {
            this->rule = rule;
            this->type = type;
            this->parentNode = parentNode;
        }
        ~Node() {}

        // TODO
        // std::string getText() { return this->rule->getText(); }
        /**
         * Adds child at the end
         */
        void push_child(Node *child)
        {
            if (freezed)
            {
                throw std::exception("Node is freezed");
            }
            children.push_back(child);
            child->parentNode = this;
        }
        /**
         * Adds a child at start
         */
        void unshift_child(Node *child)
        {
            if (freezed)
            {
                throw std::exception("Node is freezed");
            }
            children.insert(children.begin(), child);
            child->parentNode = this;
        }

        std::string toStringTree(std::string padding = "")
        {
            std::cout << this->children.size() << std::endl;

            if (this->children.size() == 0)
                return padding + this->toString();

            std::string out;
            out += this->toString();

            padding += "  ";

            for (int i = 0; i < this->children.size(); i++)
            {
                out += this->children[i]->toStringTree(padding);
            }
            return out;
        }
        /**
         * print essential information nto debug
         */
        virtual std::string toString() = 0;
        /**
         * AST -> LLVM
         */
        virtual llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) = 0;
    };

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

    enum class StructPropertyType
    {
        STRUCT_PROPERTY_TYPE_FIELD,
        STRUCT_PROPERTY_TYPE_ALIAS,
        STRUCT_PROPERTY_TYPE_GETTER,
        STRUCT_PROPERTY_TYPE_SETTER,
        STRUCT_PROPERTY_TYPE_METHOD,
    };

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
        int bits;
        bool is_signed;
    };

    struct FloatProperties
    {
        int bits;
    };

    struct Body : public Node
    {
    public:
        // NOTE: about cpp
        // std::unordered_map<char*, Node*> scope; --> wrong char* is not the expected type, no "=="
        // std::unordered_map<string, Node*> scope; --> misc errors
        std::unordered_map<std::string_view, Node *> scope;
        // back pointer to fast access
        Body *parent = nullptr;
        // BasicBlock will be populated at create_llvm_block and cached
        // NOTE BasicBlock needs to be attached before codegen into them
        llvm::BasicBlock *llvm_basicblock = nullptr;
        // TODO remove body as we can reverse the tree and search it!
        Body(antlr4::ParserRuleContext *rule, Node *parentNode, Body *parent) : Node(rule, ast_types::BODY, parentNode)
        {
            this->parent = parent;
        }
        /**
         * Register a name in the scope
         */
        void set(char *name, Node *node)
        {
            // TODO check for valid ast_types
            this->scope[name] = node;
        }

        Node *lookup(char *name)
        {
            std::string_view name_view(name);
            Body *p = this;
            Node *f;
            do
            {
                std::cout << p->toString() << ".lookup(" << name << ")" << std::endl;
                // f = p->scope[name_view];
                auto it = p->scope.find(name_view);
                if (it != p->scope.end())
                {
                    return it->second;
                }
                p = p->parent;
            } while (p != nullptr);

            return nullptr;
        }

        std::string toString() override;
        /**
         * Creates named or returns cached LLVM BasicBlock
         * 
         * NOTE: BasicBlocks needs to be created and attached before codegen inside them or raise SEH / parenting issues
         */
        llvm::BasicBlock *create_llvm_block(logia::Backend *codegen, char *name);
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    class Program : public Body
    {
    public:
        Program(antlr4::ParserRuleContext *rule, Node *parentNode, Body *parent) : Body(rule, parentNode, parent)
        {
            this->type = (ast_types)(ast_types::PROGRAM | ast_types::BODY);
        }

        std::string toString() override;
    };

    struct FunctionType
    {
        char *name;
        char *docstring;
        std::vector<FunctionParameters> parameters;
        std::vector<llvm::Type *> parametersIR;
        Type *return_type;

        /**
         * intrinsic are defined in the compiler process or in the module bootstrap
         */
        bool intrinsic;
        Body *body;
        llvm::Function *functionIR;
    };

    struct StructType
    {
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

        Type(antlr4::ParserRuleContext *rule, Node *parentNode, Primitives prim) : Node(rule, ast_types::TYPE, parentNode)
        {
            this->primitive = prim;
        }
        ~Type() {}

        bool isFunction() { return this->primitive == Primitives::FUNCTION_TY; };
        bool isStruct() { return this->primitive == Primitives::STRUCT_TY; };

        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule, ast_types type, Node *parentNode) : Node(rule, (ast_types)(type | ast_types::EXPRESSION), parentNode) {}
        std::string toString() override
        {
            return "Expression";
        }
    };

    struct CallExpression : Expression
    {
        // TODO remove locator!
        Expression *locator = nullptr;
        // TODO remove arguments!
        std::vector<Expression *> arguments;
        CallExpression(antlr4::ParserRuleContext *rule, Node *parentNode, Expression *locator, std::vector<Expression *> arguments) : Expression(rule, ast_types::CALL_EXPRESSION, parentNode)
        {
            // TODO remove 2
            this->locator = locator;
            this->arguments = arguments;

            this->push_child(locator);
            for (int i = 0; i < arguments.size(); ++i)
            {
                this->push_child(arguments[i]);
            }
        }
        Expression* get_locator() {
            return this->children[0];
        }
        std::vector<Expression*> get_arguments() {
            return std::vector<Expression*>(this->children.begin() + 1, this->children.end());
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct StringLiteral : Expression
    {
        char *text;
        StringLiteral(antlr4::ParserRuleContext *rule, Node *parentNode, char *text) : Expression(rule, ast_types::STRING_LITERAL, parentNode)
        {
            this->text = text;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct FloatLiteral : Expression
    {
    public:
        double value;
        Type *type;

        FloatLiteral(antlr4::ParserRuleContext *rule, Node *parentNode, Type *type, double value) : Expression(rule, ast_types::FLOAT_LITERAL, parentNode)
        {
            this->value = value;
            this->type = type;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };
    struct IntegerLiteral : Expression
    {
        uint64_t uvalue;
        int64_t ivalue;
        Type *type;

        IntegerLiteral(antlr4::ParserRuleContext *rule, Node *parentNode, Type *type, int64_t value) : Expression(rule, ast_types::INTEGER_LITERAL, parentNode)
        {
            LOGIA_ASSERT(type);
            this->uvalue = 0;

            this->ivalue = value;
            this->type = type;
        }
        IntegerLiteral(antlr4::ParserRuleContext *rule, Node *parentNode, Type *type, uint64_t value) : Expression(rule, ast_types::INTEGER_LITERAL, parentNode)
        {
            LOGIA_ASSERT(type);
            this->ivalue = 0;

            this->uvalue = value;
            this->type = type;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct Identifier : Expression
    {
        char *identifier;
        Identifier(antlr4::ParserRuleContext *rule, Node *parentNode, char *identifier) : Expression(rule, ast_types::IDENTIFIER, parentNode)
        {
            LOGIA_ASSERT(type);
            this->identifier = identifier;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct MemberAccessExpression : Expression
    {
        MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *parentNode, Node *left, Node *right) : Expression(rule, ast_types::EXPRESSION, parentNode)
        {
            this->push_child(left);
            this->push_child(right);
        }
        Expression* get_left() {
            return (Expression *)this->children[0];
        }
        Expression* get_right() {
            return (Expression *)this->children[1];
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder
    };

    enum class BinaryOperator {
        ADD,
        SUB,
        MUL,
        DIV,
        EQ,
        NEQ,
        LT,
        GT,
        LTE,
        GTE,
        AND,
        OR,
        NOT,
        ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
    };

    struct BinaryExpression : CallExpression
    {
        BinaryOperator op;
        BinaryExpression(antlr4::ParserRuleContext *rule, Node *parentNode, BinaryOperator op, Expression *left, Expression *right) : Expression(rule, ast_types::EXPRESSION, parentNode)
        {
            this->op = op;
            this->add_child(ast_create_identifier(nullptr, this, ast_binary_operator_to_string(op)));
            this->add_child(left);
            this->add_child(right);
        }
        Expression* get_left() {
            return (Expression *)this->children[0];
        }
        Expression* get_right() {
            return (Expression *)this->children[1];
        }
        std::string toString() override;
    };

    enum class PrefixUnaryOperator {
        NEG, // -
        NOT, // !
        INC, // ++
        DEC, // --
    };

    struct PrefixUnaryExpression : CallExpression
    {
        PrefixUnaryOperator op;
        PrefixUnaryExpression(antlr4::ParserRuleContext *rule, Node *parentNode, PrefixUnaryOperator op, Expression *operand) : Expression(rule, ast_types::EXPRESSION, parentNode)
        {
            this->op = op;
            this->add_child(ast_create_identifier(nullptr, this, ast_prefix_unary_operator_to_string(op)));
            this->add_child(operand);
        }
        Expression* get_operand() {
            return (Expression *)this->children[0];
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    enum class PostfixUnaryOperator {
        INC, // ++
        DEC, // --
    };

    struct PostfixUnaryExpression : CallExpression
    {
        PostfixUnaryOperator op;
        PostfixUnaryExpression(antlr4::ParserRuleContext *rule, Node *parentNode, PostfixUnaryOperator op, Expression *operand) : Expression(rule, ast_types::EXPRESSION, parentNode)
        {
            this->op = op;
            this->add_child(ast_create_identifier(nullptr, this, ast_postfix_unary_operator_to_string(op)));
            this->add_child(operand);
        }
        Expression* get_operand() {
            return (Expression *)this->children[0];
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct Stmt : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Stmt(antlr4::ParserRuleContext *rule, ast_types type, Node *parentNode) : Node(rule, (ast_types)(type | ast_types::STMT), parentNode) {}
        std::string toString() override
        {
            return "Statement";
        }
    };

    struct ReturnStmt : Stmt
    {
        Expression *expr;
        ReturnStmt(antlr4::ParserRuleContext *rule, Node *parentNode, Expression *expr) : Stmt(rule, ast_types::RETURN_STMT, parentNode)
        {
            this->expr = expr;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct VarDeclStmt : Stmt
    {
        char *name;
        Type *type;
        Expression *expr;
        llvm::AllocaInst *ir;

        VarDeclStmt(antlr4::ParserRuleContext *rule, Node *parentNode, char *name, Type *type, Expression *expr) : Stmt(rule, ast_types::VAR_DECL_STMT, parentNode), ir(nullptr)
        {
            this->name = name;
            this->type = type;
            this->expr = expr;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct IfStmt : Stmt
    {
        char *name;
        Type *type;
        Expression *expr;
        llvm::AllocaInst *ir;

        IfStmt(antlr4::ParserRuleContext *rule, Node *parentNode, Expression *condition) : Stmt(rule, ast_types::IF_STMT, parentNode), ir(nullptr)
        {
            this->push_child(condition);
            this->push_child(new Body(nullptr, this, nullptr)); // thenblock
            this->push_child(new Body(nullptr, this, nullptr)); // elseblock
            this->freezed = true;
        }
        Expression *get_condition()
        {
            return (Expression *)this->children[0];
        }
        Body *get_then()
        {
            return (Body *)this->children[1];
        }
        Body *get_else()
        {
            return (Body *)this->children[2];
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };

    struct GotoStmt : Stmt
    {
        char *name;

        GotoStmt(antlr4::ParserRuleContext *rule, Node *parentNode, char *name) : Stmt(rule, ast_types::GOTO_STMT, parentNode)
        {
            this->name = name;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };
    
    //
    // logia AST c api
    // this can be used in comptime execution
    //

    //
    // ast creation
    //
    /**
     * Creates a body (function body/block scope)
     */
    LOGIA_API LOGIA_LEND Body *ast_create_body(Node *parentNode);
    /**
     * Creates a string literal
     */
    LOGIA_API LOGIA_LEND StringLiteral *ast_create_string_lit(char *text);
    /**
     * Creates a floating point literal
     */
    LOGIA_API LOGIA_LEND FloatLiteral *ast_create_float_lit(Body *body, double value);
    /**
     * Creates a signed integer literal
     */
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_int_lit(Body *body, int64_t value);
    /**
     * Creates an unsigned integer literal
     */
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_uint_lit(Body *body, uint64_t value);
    /**
     * Creates a call expression
     */
    LOGIA_API LOGIA_LEND CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments);
    /**
     * Creates a return statement
     */
    LOGIA_API LOGIA_LEND ReturnStmt *ast_create_return(Expression *ret);
    /**
     * Creates a program
     */
    LOGIA_API LOGIA_LEND Program *ast_create_program(llvm::LLVMContext &C);
    /**
     * Creates a function
     */
    LOGIA_API LOGIA_LEND Type *ast_create_function_type(Body *body, char *name, Type *return_type);
    /**
     * Creates a intrinsic function
     *
     * REVIEW TODO this may also need to call backkend::add_intrinsic to keep everything sync.
     */
    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, char *name, Type *return_type);
    /**
     * Creates a struct type
     */
    LOGIA_API LOGIA_LEND Type *ast_create_struct_type(Body *body, char *name);
    /**
     * Creates a variable declaration
     * Note, to create a constant Type should be readonly.
     */
    LOGIA_API LOGIA_LEND VarDeclStmt *ast_create_var_decl(Node *current, char *name, Type *type, Expression *expr);
    /**
     * Creates an identifier
     */
    LOGIA_API LOGIA_LEND Identifier *ast_create_identifier(Node *current, char *name);
    /**
     * Creates an identifier
     */
    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition);
    //
    // ast fill
    //
    /**
     * Adds a parameter to a function
     */
    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, char *param_name, Expression *param_default_value);
    /**
     * Adds a field to struct
     */
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, char *prop_name, Expression *prop_default_value);

    //
    // ast query
    //
    /**
     * Converts Primitives enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim);
    /**
     * Converts ast_types enum to string
     */
    LOGIA_API LOGIA_LEND char *ast_types_to_string(ast_types type);
    /**
     * callback type for traverse function
     */
    typedef bool (*ast_traverse_callback_t)(Node *);
    /**
     * traverse subnodes.
     */
    void ast_traverse(Node *current, ast_traverse_callback_t cb);

    //
    // ast-traverese/search
    //
    /**
     * reverse the tree a returns the first node that match the given mask
     */
    LOGIA_API Node *ast_find_closest_parent(Node *current, ast_types mask_type);
    /**
     * reverse the tree searching for given name that match with a Type
     */
    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name);
    /**
     * reverse the tree searching for given name that match with a vardecl
     */
    LOGIA_API VarDeclStmt *ast_get_vardecl_by_name(Node *current, char *name);

    //
    // utils
    //
    LOGIA_API const char* ast_postfix_unary_operator_to_string(PostfixUnaryOperator op);
    LOGIA_API const char* ast_prefix_unary_operator_to_string(PrefixUnaryOperator op);
    LOGIA_API const char* ast_binary_operator_to_string(ast_binary_operator op);
}
