#pragma once

#include "antlr4-runtime.h"
#include "backend.h"
#include <cstdlib> // for itoa
#include "utils.h"

namespace logia::AST
{
    // Homogenous AST node type
    struct Node
    {
    public:
        antlr4::ParserRuleContext *rule = nullptr;
        std::vector<Node *> children; // normalized list of children

        Node(antlr4::ParserRuleContext *rule) { this->rule = rule; }
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

        virtual llvm::Value* codegen(logia::Backend *codegen) = 0;
    };

    struct Type;

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

    enum StructPropertyType
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
    };

    struct StructProperty
    {
        std::string name;
        std::string docstring;
        enum StructPropertyType type;

        bool isField() { return this->type == STRUCT_PROPERTY_TYPE_FIELD; };
        bool isAlias() { return this->type == STRUCT_PROPERTY_TYPE_ALIAS; };
        bool isGetter() { return this->type == STRUCT_PROPERTY_TYPE_GETTER; };
        bool isSetter() { return this->type == STRUCT_PROPERTY_TYPE_SETTER; };
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
        Body *parent;
        std::vector<Node *> statements;
        // wrong char* is not the expected type, no "=="
        // std::map<char*, Node*> scope;
        // std::unordered_map<char*, Node*> scope;
        // string_view works with char*
        std::unordered_map<std::string_view, Node *> scope;

        Body(antlr4::ParserRuleContext *rule, Body *parent) : Node(rule)
        {
            this->parent = parent;
        }
        void add_statement(Node *node)
        {
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
        llvm::Value* codegen(logia::Backend *codegen) override;
    };

    struct FunctionType
    {
        char *name;
        std::string docstring;
        std::vector<FunctionParameters *> parameters;
        std::vector<llvm::Type *> parametersIR;
        Type *return_type;
        Body *body;
        llvm::Function *functionIR;
    };

    struct StructType
    {

        std::string name;
        std::string docstring;
        std::vector<StructProperty *> properties;
        std::vector<Type *> methods;
    };

    struct Type : public Node
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

        Type(antlr4::ParserRuleContext *rule, Primitives type) : Node(rule)
        {
            this->type = type;
        }
        ~Type() {}

        bool isFunction() { return this->type == Primitives::PRIMITIVE_FUNCTION; };

        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };

    struct Expression : Node
    {
        // REVIEW strange  why do i need to declare this ?
        Expression(antlr4::ParserRuleContext *rule) : Node(rule) {}
        std::string toString() override
        {
            return "Expression";
        }
    };

    struct CallExpression : Expression
    {
        Expression *locator;
        std::vector<Expression *> arguments;
        CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> arguments) : Expression(rule)
        {
            this->locator = locator;
            this->arguments = arguments;
        }
        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };
    struct MemberAccessExpression : Expression
    {
        Node *left;
        Node *right;
    };

    struct StringLiteral : Expression
    {
        char *text;
        StringLiteral(antlr4::ParserRuleContext *rule, char *text) : Expression(rule)
        {
            this->text = text;
        }
        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };

    struct FloatLiteral : Expression
    {
    public:
        double value;
        Type *type;

        FloatLiteral(antlr4::ParserRuleContext *rule, Type *type, double value) : Expression(rule)
        {
            this->value = value;
            this->type = type;
        }
        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };
    struct IntegerLiteral : Expression
    {
        uint64_t uvalue;
        int64_t ivalue;
        Type *type;

        IntegerLiteral(antlr4::ParserRuleContext *rule, Type *type, int64_t value) : Expression(rule)
        {
            LOGIA_ASSERT(type);
            this->ivalue = value;
            this->type = type;
        }
        IntegerLiteral(antlr4::ParserRuleContext *rule, Type *type, uint64_t value) : Expression(rule)
        {
            LOGIA_ASSERT(type);
            this->uvalue = value;
            this->type = type;
        }
        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };

    struct ReturnStmt : Node
    {
        Expression *expr;
        ReturnStmt(antlr4::ParserRuleContext *rule, Expression *expr) : Node(rule)
        {
            this->expr = expr;
        }
        std::string toString() override;
        llvm::Value* codegen(logia::Backend *codegen) override;
    };

    // shortcuts api

    Body *createProgram(llvm::LLVMContext &C);

    Body *createBody(Body *parent);

    Type *getTypeByName(Body *body, char *name);

    Type *createFunctionType(Body *body, char *name, Type *return_type);

    FloatLiteral *createFloatLiteral(Body *body, double value);
    IntegerLiteral *createSignedIntegerLiteral(Body *body, int64_t value);
    IntegerLiteral *createUnsignedIntegerLiteral(Body *body, uint64_t value);

    CallExpression *createCallExpression(Expression *locator, std::vector<Expression *> arguments);

    StringLiteral *createStringLiteral(Body *program, char *text);

    ReturnStmt *createReturn(Expression *ret);

}