#include "ast.h"
namespace logia::AST
{
    Body *createProgram()
    {
        auto body = new Body(nullptr, nullptr);

        Type *i8 = new Type(nullptr, Primitives::i8);
        i8->Integer.bits = 8;
        i8->Integer.isSigned = true;

        body->scope[(char *)"λi8"] = i8;

        Type *i16 = new Type(nullptr, Primitives::i16);
        i16->Integer.bits = 16;
        i16->Integer.isSigned = true;

        body->scope[(char *)"λi16"] = i16;

        Type *i32 = new Type(nullptr, Primitives::i32);
        i32->Integer.bits = 32;
        i32->Integer.isSigned = true;

        body->scope[(char *)"λi32"] = i32;

        Type *i64 = new Type(nullptr, Primitives::i64);
        i64->Integer.bits = 64;
        i64->Integer.isSigned = true;

        body->scope[(char *)"λi64"] = i64;

        Type *u8 = new Type(nullptr, Primitives::u8);
        u8->Integer.bits = 8;
        u8->Integer.isSigned = true;

        body->scope[(char *)"λu8"] = u8;

        Type *u16 = new Type(nullptr, Primitives::u16);
        u16->Integer.bits = 16;
        u16->Integer.isSigned = true;

        body->scope[(char *)"λu16"] = u16;

        Type *u32 = new Type(nullptr, Primitives::u32);
        u32->Integer.bits = 32;
        u32->Integer.isSigned = true;

        body->scope[(char *)"λu32"] = u32;

        Type *u64 = new Type(nullptr, Primitives::u64);
        u64->Integer.bits = 64;
        u64->Integer.isSigned = true;

        body->scope[(char *)"λu64"] = u64;

        Type *f32 = new Type(nullptr, Primitives::f32);
        f32->Float.bits = 32;

        body->scope[(char *)"λf32"] = f32;

        Type *f64 = new Type(nullptr, Primitives::f64);
        f64->Float.bits = 64;

        body->scope[(char *)"λf64"] = f64;

        Type *ptr = new Type(nullptr, Primitives::ptr);
        body->scope[(char *)"λptr"] = f64;

        return body;
    }

    Body *createBody(Body *parent)
    {
        return new Body(nullptr, parent);
    }

    Type *getTypeByName(Body *body, char *name)
    {
        // TODO cast if possible or nullptr!
        return (Type *)body->lookup(name);
    }

    Type *createFunctionType(Body *body, char *name, Type *return_type)
    {
        Type *ast = new Type(nullptr, Primitives::PRIMITIVE_FUNCTION);
        ast->Function.name = name;
        ast->Function.return_type = return_type;
        ast->Function.body = new Body(nullptr, body);

        body->set(name, ast);

        return ast;
    }

    FloatLiteral *createFloatLiteral(Body *body, double value)
    {
        return new FloatLiteral(nullptr, (Type *)body->lookup(strdup("λf64")), value);
    }
    IntegerLiteral *createSignedIntegerLiteral(Body *body, int64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λi64")), value);
    }
    IntegerLiteral *createUnsignedIntegerLiteral(Body *body, uint64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λu64")), value);
    }

    CallExpression *createCallExpression(Expression *locator, std::vector<Expression *> arguments)
    {
        return new CallExpression(nullptr, locator, arguments);
    }

    StringLiteral *createStringLiteral(Body *program, char *text)
    {
        return new StringLiteral(nullptr, text);
    }

    ReturnStmt *createReturn(Expression *ret)
    {
        return new ReturnStmt(nullptr, ret);
    }
}