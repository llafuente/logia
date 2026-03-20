#include "ast.h"
#include "llvm/IR/TypedPointerType.h"
#include "utils.h"

namespace logia::AST
{
    LOGIA_API Body *createProgram(llvm::LLVMContext &C)
    {
        auto body = new Body(nullptr, nullptr, nullptr);
        body->type = (ast_types)(body->type | ast_types::PROGRAM); // override

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported at logia::AST::Type

        Type *i8 = new Type(nullptr, body, Primitives::i8);
        i8->Integer.bits = 8;
        i8->Integer.isSigned = true;
        i8->ir = llvm::Type::getInt8Ty(C);

        body->scope[(char *)"λi8"] = i8;

        Type *i16 = new Type(nullptr, body, Primitives::i16);
        i16->Integer.bits = 16;
        i16->Integer.isSigned = true;
        i16->ir = llvm::Type::getInt16Ty(C);

        body->scope[(char *)"λi16"] = i16;

        Type *i32 = new Type(nullptr, body, Primitives::i32);
        i32->Integer.bits = 32;
        i32->Integer.isSigned = true;
        i32->ir = llvm::Type::getInt32Ty(C);

        body->scope[(char *)"λi32"] = i32;

        Type *i64 = new Type(nullptr, body, Primitives::i64);
        i64->Integer.bits = 64;
        i64->Integer.isSigned = true;
        i64->ir = llvm::Type::getInt64Ty(C);

        body->scope[(char *)"λi64"] = i64;

        Type *u8 = new Type(nullptr, body, Primitives::u8);
        u8->Integer.bits = 8;
        u8->Integer.isSigned = true;
        u8->ir = llvm::Type::getInt8Ty(C);

        body->scope[(char *)"λu8"] = u8;

        Type *u16 = new Type(nullptr, body, Primitives::u16);
        u16->Integer.bits = 16;
        u16->Integer.isSigned = true;
        u16->ir = llvm::Type::getInt16Ty(C);

        body->scope[(char *)"λu16"] = u16;

        Type *u32 = new Type(nullptr, body, Primitives::u32);
        u32->Integer.bits = 32;
        u32->Integer.isSigned = true;
        u32->ir = llvm::Type::getInt32Ty(C);

        body->scope[(char *)"λu32"] = u32;

        Type *u64 = new Type(nullptr, body, Primitives::u64);
        u64->Integer.bits = 64;
        u64->Integer.isSigned = true;
        u64->ir = llvm::Type::getInt64Ty(C);

        body->scope[(char *)"λu64"] = u64;

        Type *f16 = new Type(nullptr, body, Primitives::f16);
        f16->Float.bits = 16;
        f16->ir = llvm::Type::getHalfTy(C);

        body->scope[(char *)"λf16"] = f16;

        Type *f32 = new Type(nullptr, body, Primitives::f32);
        f32->Float.bits = 32;
        f32->ir = llvm::Type::getFloatTy(C);

        body->scope[(char *)"λf32"] = f32;

        Type *f64 = new Type(nullptr, body, Primitives::f64);
        f64->Float.bits = 64;
        f64->ir = llvm::Type::getDoubleTy(C);

        body->scope[(char *)"λf64"] = f64;

        Type *f128 = new Type(nullptr, body, Primitives::f128);
        f128->Float.bits = 64;
        f128->ir = llvm::Type::getFP128Ty(C);

        body->scope[(char *)"λf128"] = f128;

        Type *lvoid = new Type(nullptr, body, Primitives::Void);
        lvoid->ir = llvm::Type::getVoidTy(C);

        body->scope[(char *)"λvoid"] = lvoid;

        // TODO study opaque pointers, while seem what we need
        Type *ptr = new Type(nullptr, body, Primitives::ptr);
        // opaque pointer, do not store information about pointee
        ptr->ir = llvm::PointerType::get(C, 0);
        body->scope[(char *)"λptr"] = ptr;

        return body;
    }

    LOGIA_API Body *createBody(Node *parentNode)
    {
        LOGIA_ASSERT(parentNode);

        auto parentBody = (Body *)ast_find_closest_parent(parentNode, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        return new Body(nullptr, parentNode, parentBody);
    }

    FloatLiteral *createFloatLiteral(Body *body, double value)
    {
        return new FloatLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λf64")), value);
    }
    IntegerLiteral *createSignedIntegerLiteral(Body *body, int64_t value)
    {
        return new IntegerLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λi64")), value);
    }
    IntegerLiteral *createUnsignedIntegerLiteral(Body *body, uint64_t value)
    {
        return new IntegerLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λu64")), value);
    }

    LOGIA_API CallExpression *createCallExpression(Expression *locator, std::vector<Expression *> arguments)
    {
        LOGIA_ASSERT((locator->type & ast_types::EXPRESSION) != 0);
        // TODO LOGIA_ASSERT_ALL(arguments, .type & ast_types::EXPRESSION != 0);

        auto callexpr = new CallExpression(nullptr, nullptr, locator, arguments);

        locator->parentNode = callexpr;
        for (int i = 0; i < arguments.size(); ++i)
        {
            arguments[i]->parentNode = callexpr;
        }

        return callexpr;
    }
    LOGIA_API StringLiteral *createStringLiteral(char *text)
    {
        // TODO review remove parentNode from constructor, is a leaf right?
        return new StringLiteral(nullptr, nullptr, text);
    }

    LOGIA_API ReturnStmt *createReturn(Expression *ret)
    {
        auto stmt = new ReturnStmt(nullptr, nullptr, ret);
        ret->parentNode = stmt;
        return stmt;
    }

    ///
    /// toString
    ///

    // utils
    std::string __itoa(int i)
    {
        char buffer[36];
        return std::string(itoa(i, buffer, 10));
    }

    std::string Body::toString()
    {
        char buffer[36];
        return std::string(this->parent ? "body[" : "program[") + std::string(itoa(this->children.size(), buffer, 10)) + "] ";
    }
    std::string Type::toString()
    {
        switch (this->type)
        {
        case Primitives::PRIMITIVE_FUNCTION:
            return "Type[Function]";
        case Primitives::Void:
            return "Type[Void]";
        case Primitives::i8:
            return "Type[i8]";
        case Primitives::i64:
            return "Type[i64]";
        case Primitives::Struct:
            return "Type[struct]";
        }

        return "Type[?]";
    }

    std::string CallExpression::toString()
    {
        char buffer[36];
        auto name = (StringLiteral *)this->locator;
        return std::string("CallExpression: ") + name->text + "(" + std::string(itoa(this->arguments.size(), buffer, 10)) + " args)";
    }

    std::string IntegerLiteral::toString()
    {
        std::cout << this->type << std::endl;
        std::cout << this->type->toString() << std::endl;
        std::cout << this->ivalue << std::endl;
        std::cout << this->uvalue << std::endl;

        char buffer[36];
        return std::string("IntegerLiteral[") + this->type->toString() + "] = " + std::string(itoa(this->ivalue, buffer, 10));
    }

    std::string FloatLiteral::toString()
    {
        return "FloatLiteral";
    }

    std::string StringLiteral::toString()
    {
        return std::string("StringLiteral{name: ") + this->text + ", type: " + ast_types_to_string(this->type) + "}";
    }

    std::string ReturnStmt::toString()
    {
        return "ReturnStmt";
    }

    ///
    /// codegen
    ///

    llvm::Value *Body::codegen(logia::Backend *codegen)
    {
        llvm::BasicBlock *BB = nullptr;
        if (this->parent)
        {
            BB = llvm::BasicBlock::Create(codegen->context, "entry", nullptr);
        }

        std::cout << this->toString() << std::endl;

        for (int i = 0; i < this->children.size(); i++)
        {
            // update insert point on everystatement as visitor may change it!
            // REVIEW builder should be a param for codegen, this will be a problem soon.
            if (BB)
            {
                codegen->builder->SetInsertPoint(BB);
            }
            // update insert point on everystatement as visitor may change it!
            Node *n = this->children[i];
            std::cout << "codegen.statement[" << i << "] " << n->toString() << std::endl;
            auto inst = n->codegen(codegen);
        }
        return BB;
    }

    llvm::Value *Type::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir)
        {
            return (llvm::Value *)this->ir;
        }

        switch (this->type)
        {
        case Primitives::PRIMITIVE_FUNCTION:
        {
            int pcount = this->Function.parameters.size();
            this->Function.parametersIR.reserve(pcount);
            for (int i = 0; i < pcount; ++i)
            {
                this->Function.parametersIR.push_back((llvm::Type *)this->Function.parameters[i].type->codegen(codegen));
            }
            this->Function.return_type->codegen(codegen);
            this->ir = (llvm::Type *)llvm::FunctionType::get(this->Function.return_type->ir,
                                                             this->Function.parametersIR, // parameter list
                                                             false);                      // not variadic

            this->Function.functionIR = llvm::Function::Create((llvm::FunctionType *)this->ir, llvm::Function::ExternalLinkage, 0, this->Function.name, codegen->module.get());

            // Create a basic block and insert a return

            llvm::BasicBlock *BB = (llvm::BasicBlock *)this->Function.body->codegen(codegen);
            BB->insertInto(this->Function.functionIR);

            return (llvm::Value *)this->ir;
        }
        break;
        case Primitives::Struct:
        {
            auto max = this->Struct.properties.size();
            std::vector<llvm::Type *> elements;
            elements.reserve(max);
            for (int i = 0; i < max; ++i)
            {
                auto p = &this->Struct.properties[i];
                if (p->isField())
                {
                    elements.push_back((llvm::Type *)p->type->codegen(codegen));
                }
            }
            auto st = llvm::StructType::create(codegen->context, this->Struct.name);
            st->setBody(elements);

            this->ir = st;
            return (llvm::Value *)this->ir;
        }
        break;
        }

        // TODO
        throw std::exception("TODO!");
    }

    llvm::Value *CallExpression::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;

        // Look up the name in the global module table.
        auto name = (StringLiteral *)this->locator;
        llvm::Function *CalleeF = codegen->module->getFunction(name->text);
        if (!CalleeF)
            throw std::exception("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != this->arguments.size())
            throw std::exception("Incorrect # arguments passed");

        std::vector<llvm::Value *> ArgsV;
        for (unsigned i = 0, e = this->arguments.size(); i != e; ++i)
        {
            std::cout << "argument[" << i << "]" << std::endl;
            ArgsV.push_back(this->arguments[i]->codegen(codegen));
            if (!ArgsV.back())
                return nullptr;
        }

        return codegen->builder->CreateCall(CalleeF, ArgsV, "call");
        // return (llvm::Value*) llvm::CallInst::Create(CalleeF, ArgsV, "call");
    }

    llvm::Value *IntegerLiteral::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;
        return llvm::ConstantInt::get((llvm::Type *)this->type->codegen(codegen), llvm::APInt(this->type->Integer.bits, this->ivalue, this->type->Integer.isSigned));
    }

    llvm::Value *FloatLiteral::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;
        throw std::exception("todo");
    }

    llvm::Value *StringLiteral::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;
        throw std::exception("todo");
    }

    llvm::Value *ReturnStmt::codegen(logia::Backend *codegen)
    {
        std::cout << this->toString() << std::endl;
        if (!this->expr)
        {
            return codegen->builder->CreateRetVoid();
            // return llvm::ReturnInst::Create(codegen->context);
        }
        // return llvm::ReturnInst::Create(codegen->context, this->expr->codegen(codegen));
        return codegen->builder->CreateRet(this->expr->codegen(codegen));
    }

    //
    // ast creation
    //
    Type *ast_create_function_type(Body *parentBody, char *name, Type *return_type)
    {
        LOGIA_ASSERT(parentBody);
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, parentBody, Primitives::PRIMITIVE_FUNCTION);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.body = new Body(nullptr, t, parentBody);

        parentBody->set(name, t);

        return t;
    }

    LOGIA_API Type *ast_create_struct_type(Body *current, char *name)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        Type *t = new Type(nullptr, nullptr, Primitives::Struct);
        new (&t->Struct) StructType();
        t->Struct.name = name;

        parentBody->set(name, t);

        return t;
    }

    //
    // ast fill
    //
    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, std::string &&param_name, Expression *param_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isFunction());
        // LOGIA_ASSERT(*prop_name);

        auto param = s->Function.parameters.emplace_back(param_name, param_type, param_default_value);
    }
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, std::string &&prop_name, Expression *prop_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isStruct());
        // LOGIA_ASSERT(*prop_name);

        auto prop = s->Struct.properties.emplace_back(prop_name, "", StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD, prop_type, prop_default_value);
    }

    //
    // ast query
    //

    std::string ast_types_to_string(ast_types type)
    {
        std::string out = "";
        if ((ast_types::EXPRESSION & type) != 0)
        {
            out += "EXPRESSION.";
        }
        if ((ast_types::STMT & type) != 0)
        {
            out += "STMT.";
        }
        if ((ast_types::TYPE & type) != 0)
        {
            out += "TYPE.";
        }
        if ((ast_types::BODY & type) != 0)
        {
            out += "BODY.";
        }

        // remove all flags
        type = (ast_types)(type & ~ALL_FLAGS);

        switch (type)
        {
        case ast_types::PROGRAM:
            out += PROGRAM;
            break;
        case ast_types::FUNCTION:
            out += FUNCTION;
            break;
        case ast_types::CALL_EXPRESSION:
            out += CALL_EXPRESSION;
            break;
        case ast_types::STRING_LITERAL:
            out += STRING_LITERAL;
            break;
        case ast_types::FLOAT_LITERAL:
            out += FLOAT_LITERAL;
            break;
        case ast_types::INTEGER_LITERAL:
            out += INTEGER_LITERAL;
            break;
        case ast_types::RETURN_STMT:
            out += RETURN_STMT;
            break;
        }
        return out;
    }

    //
    // ast-traverese/search
    //
    LOGIA_API Node *ast_find_closest_parent(Node *current, ast_types mask_type)
    {
        LOGIA_ASSERT(current);
        // REVIEW Start at the current node? or 1 level up?
        // LOGIA_ASSERT(current->parentNode);
        // current = current->parentNode;
        do
        {
            if (((int)current->type & (int)mask_type) != 0)
            {
                return current;
            }
        } while (current != nullptr);

        return nullptr;
    }

    LOGIA_API Type *ast_get_type_by_name(Node *current, char *name)
    {
        auto body = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(body); // this shall exists!
        // TODO cast if possible or nullptr!
        return (Type *)body->lookup(name);
    }

}
