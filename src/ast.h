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
        // flags
        EXPRESSION = 0b1000000000000000,
        STMT = 0b0100000000000000,
        TYPE = 0b0010000000000000,
        BODY = 0b0001000000000000,

        ALL_FLAGS = 0b1111000000000000,

        // types
        PROGRAM = 0x01,
        FUNCTION = 0x02,
        BLOCK = 0x04,
        CALL_EXPRESSION = 0x10,
        STRING_LITERAL = 0x100,
        FLOAT_LITERAL = 0x200,
        INTEGER_LITERAL = 0x400,
        IDENTIFIER = 0x0800,

        RETURN_STMT = 0x1000,
        VAR_DECL_STMT = 0x2000
        // NOTE: if modified -> ast_types_to_string
    };

    // Homogenous AST node type
    struct LOGIA_EXPORT Node
    {
    public:
        antlr4::ParserRuleContext *rule = nullptr;
        ast_types type;
        Node *parentNode = nullptr;

        std::vector<Node *> children; // normalized list of children

        Node(antlr4::ParserRuleContext *rule, ast_types type, Node *parentNode)
        {
            this->rule = rule;
            this->type = type;
            this->parentNode = parentNode;
        }
        ~Node() {}

        // std::string getText() { return this->rule->getText(); }

        void addChild(Node *t)
        {
            children.push_back(t);
        }

        std::vector<Node *> getChildren() { return children; }

        virtual std::string toString() = 0;
        /*
                {
                    char buffer[36];
                    itoa(this->children.size(), buffer, 10);

                    return std::string("Node(").append(buffer).append(")");
                    // TODO
                    // return this->getText();
                }
        */
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

        virtual llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) = 0;
    };

    enum class Primitives
    {
        Void,

        i8,
        i16,
        i32,
        i64,
        u8,
        u16,
        u32,
        u64,
        f16,
        f32,
        f64,
        f128,

        /*
        Method	Bit Width	Description
        *Type::getHalfTy(context)	16	IEEE 754 half precision
        Type::getBFloatTy(context)	16	Brain floating point
        *Type::getFloatTy(context)	32	IEEE 754 single precision
        *Type::getDoubleTy(context)	64	IEEE 754 double precision
        *Type::getFP128Ty(context)	128	IEEE 754 quadruple precision
        Type::getX86_FP80Ty(context)	80	x87 extended precision
        Type::getPPC_FP128Ty(context)	128	PowerPC double-double
        */

        // aliases but very special...
        Int,
        Bool,
        size,
        ptrdiff,
        address,
        Typeid,

        ptr,
        Enum,
        Struct,
        interface,
        PRIMITIVE_FUNCTION,
        // this is a pointer to function, but we may need to declare at this level
        callable,
    };

    enum class StructPropertyType
    {
        STRUCT_PROPERTY_TYPE_FIELD,
        STRUCT_PROPERTY_TYPE_ALIAS,
        STRUCT_PROPERTY_TYPE_GETTER,
        STRUCT_PROPERTY_TYPE_SETTER,
    };

    struct FunctionParameters
    {
        std::string name;
        Type *type;
        Node *defaultValue;
        FunctionParameters(
            std::string _name,
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
        std::string name;
        std::string docstring;
        enum StructPropertyType kind;
        Type *type;
        Expression *default_value;

        StructProperty(
            std::string _name,
            std::string _docstring,
            enum StructPropertyType _kind,
            Type *_type,
            Expression *_default_value) : name(_name),
                                          docstring(_docstring),
                                          kind(_kind),
                                          type(_type),
                                          default_value(_default_value)
        {
        }

        bool isField() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD; };
        bool isAlias() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_ALIAS; };
        bool isGetter() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_GETTER; };
        bool isSetter() { return this->kind == StructPropertyType::STRUCT_PROPERTY_TYPE_SETTER; };
    };

    struct IntegerProperties
    {
        int bits;
        bool isSigned;
    };

    struct FloatProperties
    {
        int bits;
    };

    struct Body : public Node
    {
    public:
        std::vector<Node *> statements;
        // wrong char* is not the expected type, no "=="
        // std::map<char*, Node*> scope;
        // std::unordered_map<char*, Node*> scope;
        // string_view works with char*
        std::unordered_map<std::string_view, Node *> scope;
        Body *parent;
        // TODO remove body as we can reverse the tree and search it!
        Body(antlr4::ParserRuleContext *rule, Node *parentNode, Body *parent) : Node(rule, ast_types::BODY, parentNode)
        {
            this->parent = parent;
        }
        void add_statement(Node *node)
        {
            node->parentNode = this;
            statements.push_back(node);
            addChild(node);
        }
        void set(char *name, Node *node)
        {
            this->scope[name] = node;
        }
        Node *lookup(char *name)
        {
            Body *p = this;
            Node *f;
            do
            {
                std::cout << p->toString() << ".lookup(" << name << ")" << std::endl;
                f = p->scope[name];
                if (f != nullptr)
                {
                    return f;
                }
                p = p->parent;
            } while (p != nullptr);

            return nullptr;
        }

        std::string toString() override;
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
        std::string docstring;
        std::vector<FunctionParameters> parameters;
        std::vector<llvm::Type *> parametersIR;
        Type *return_type;
        Body *body;
        llvm::Function *functionIR;
    };

    struct StructType
    {

        std::string name;
        std::string docstring;
        std::vector<StructProperty> properties;
        std::vector<Type *> methods;
    };

    struct LOGIA_EXPORT Type : public Node
    {
    public:
        Primitives type = Primitives::Void;
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

        Type(antlr4::ParserRuleContext *rule, Node *parentNode, Primitives type) : Node(rule, ast_types::TYPE, parentNode)
        {
            this->type = type;
        }
        ~Type() {}

        bool isFunction() { return this->type == Primitives::PRIMITIVE_FUNCTION; };
        bool isStruct() { return this->type == Primitives::Struct; };

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
        Expression *locator;
        std::vector<Expression *> arguments;
        CallExpression(antlr4::ParserRuleContext *rule, Node *parentNode, Expression *locator, std::vector<Expression *> arguments) : Expression(rule, ast_types::CALL_EXPRESSION, parentNode)
        {
            this->locator = locator;
            this->arguments = arguments;
        }
        std::string toString() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
    };
    struct MemberAccessExpression : Expression
    {
        Node *left;
        Node *right;
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
            this->ivalue = value;
            this->type = type;
        }
        IntegerLiteral(antlr4::ParserRuleContext *rule, Node *parentNode, Type *type, uint64_t value) : Expression(rule, ast_types::INTEGER_LITERAL, parentNode)
        {
            LOGIA_ASSERT(type);
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

    // shortcuts api
    LOGIA_API Body *createBody(Node *parentNode);

    FloatLiteral *createFloatLiteral(Body *body, double value);
    IntegerLiteral *createSignedIntegerLiteral(Body *body, int64_t value);
    IntegerLiteral *createUnsignedIntegerLiteral(Body *body, uint64_t value);

    LOGIA_API CallExpression *createCallExpression(Expression *locator, std::vector<Expression *> arguments);

    LOGIA_API StringLiteral *createStringLiteral(char *text);

    LOGIA_API ReturnStmt *createReturn(Expression *ret);

    //
    // ast creation
    //
    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C);
    LOGIA_API Type *ast_create_function_type(Body *body, char *name, Type *return_type);
    LOGIA_API Type *ast_create_struct_type(Body *body, char *name);
    LOGIA_API VarDeclStmt *ast_create_var_decl(Node *current, char *name, Type *type, Expression *expr);
    LOGIA_API Identifier *ast_create_identifier(Node *current, char *name);

    //
    // ast fill
    //
    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, std::string &&param_name, Expression *param_default_value);
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, std::string &&prop_name, Expression *prop_default_value);

    //
    // ast query
    //
    LOGIA_API std::string ast_types_to_string(ast_types type);

    //
    // ast-traverese/search
    //
    /**
     * reverse the tree a returns the first node that match the given mask
     */
    LOGIA_API Node *ast_find_closest_parent(Node *current, ast_types mask_type);
    /**
     * reverse the tree searching for given name
     */
    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name);
    LOGIA_API VarDeclStmt *ast_get_vardecl_by_name(Node *current, char *name);

}
