#include "ast.h"
#include "llvm/IR/TypedPointerType.h"
#include "utils.h"

namespace logia::AST
{
    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C)
    {
        auto body = new Program(nullptr, nullptr, nullptr);

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

    LOGIA_API Body *ast_create_body(Node *parentNode)
    {
        LOGIA_ASSERT(parentNode);

        auto parentBody = (Body *)ast_find_closest_parent(parentNode, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        return new Body(nullptr, parentNode, parentBody);
    }

    FloatLiteral *ast_create_float_lit(Body *body, double value)
    {
        return new FloatLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λf64")), value);
    }
    IntegerLiteral *ast_create_int_lit(Body *body, int64_t value)
    {
        return new IntegerLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λi64")), value);
    }
    IntegerLiteral *ast_create_uint_lit(Body *body, uint64_t value)
    {
        return new IntegerLiteral(nullptr, nullptr, (Type *)body->lookup(strdup("λu64")), value);
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
    std::string Program::toString()
    {
        char buffer[36];
        return std::string("program[") + std::string(itoa(this->children.size(), buffer, 10)) + "] ";
    }
    std::string Body::toString()
    {
        char buffer[36];
        return std::string("body[") + std::string(itoa(this->children.size(), buffer, 10)) + "] ";
    }
    std::string Type::toString()
    {
        switch (this->type)
        {
        case Primitives::PRIMITIVE_FUNCTION:
            return std::string("Type[Function]") + this->Function.name;
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
        return std::string("CallExpression: ") + this->locator->toString() + "(" + std::string(itoa(this->arguments.size(), buffer, 10)) + " args)";
    }

    std::string IntegerLiteral::toString()
    {
        DEBUG() << this->type << std::endl;
        DEBUG() << this->type->toString() << std::endl;
        DEBUG() << this->ivalue << std::endl;
        DEBUG() << this->uvalue << std::endl;

        char buffer[36];
        return std::string("IntegerLiteral[") + this->type->toString() + "] = " + std::string(itoa(this->ivalue, buffer, 10));
    }

    std::string FloatLiteral::toString()
    {
        return "FloatLiteral";
    }

    std::string StringLiteral::toString()
    {
        auto t = ast_types_to_string(this->type);
        auto str = std::string("StringLiteral{text: ") + this->text + ", type: " + t + "}";
        free(t);
        return str;
    }

    std::string ReturnStmt::toString()
    {
        return "ReturnStmt";
    }

    std::string VarDeclStmt::toString()
    {
        return "VarDeclStmt";
    }
    std::string Identifier::toString()
    {
        return std::string("Identifier: ") + this->identifier;
    }

    ///
    /// codegen
    ///
    llvm::Value *Body::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;

        for (int i = 0; i < this->statements.size(); i++)
        {
            Node *n = this->statements[i];
            DEBUG() << "codegen.statement[" << i << "] " << n->toString() << std::endl;
            auto inst = n->codegen(codegen, builder);
        }

        return nullptr;
    }

    llvm::Value *Type::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
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
                this->Function.parametersIR.push_back((llvm::Type *)this->Function.parameters[i].type->codegen(codegen, builder));
            }
            this->Function.return_type->codegen(codegen, builder);
            this->ir = (llvm::Type *)llvm::FunctionType::get(this->Function.return_type->ir,
                                                             this->Function.parametersIR, // parameter list
                                                             false);                      // not variadic

            this->Function.functionIR = llvm::Function::Create((llvm::FunctionType *)this->ir, llvm::Function::ExternalLinkage, 0, this->Function.name, codegen->module.get());

            // Create a basic block and insert a return

            if (!this->Function.intrinsic)
            {
                LOGIA_ASSERT(this->Function.body);

                llvm::BasicBlock *BB = llvm::BasicBlock::Create(codegen->context, "entry", nullptr);
                BB->insertInto(this->Function.functionIR);

                llvm::IRBuilder<> *builderInto = new llvm::IRBuilder<>(codegen->context);
                builderInto->SetInsertPoint(BB);

                this->Function.body->codegen(codegen, builderInto);
            }

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
                    elements.push_back((llvm::Type *)p->type->codegen(codegen, builder));
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

    llvm::Value *CallExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;

        // Look up the name in the global module table.
        auto name = (StringLiteral *)this->locator;
        llvm::Function *CalleeF = codegen->module->getFunction(name->text);
        // auto x = codegen->module->getValueSymbolTable();
        // x->
        if (!CalleeF)
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->text);

        // If argument mismatch error.
        if (CalleeF->arg_size() != this->arguments.size())
            throw std::exception("Incorrect # arguments passed");

        std::vector<llvm::Value *> ArgsV;
        for (unsigned i = 0, e = this->arguments.size(); i != e; ++i)
        {
            DEBUG() << "argument[" << i << "]" << std::endl;
            ArgsV.push_back(this->arguments[i]->codegen(codegen, builder));
            if (!ArgsV.back())
                return nullptr;
        }
        // NOTE name is not what i expect -> blank!
        return builder->CreateCall(CalleeF, ArgsV);
        // return builder->CreateCall(CalleeF, ArgsV, "call");
        //  return (llvm::Value*) llvm::CallInst::Create(CalleeF, ArgsV, "call");
    }

    llvm::Value *IntegerLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        return llvm::ConstantInt::get((llvm::Type *)this->type->codegen(codegen, builder), llvm::APInt(this->type->Integer.bits, this->ivalue, this->type->Integer.isSigned));
    }

    llvm::Value *FloatLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        throw std::exception("todo");
    }

    llvm::Value *StringLiteral::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        // NOTE module is required or 0xc0000005
        // !getType()->isVoidTy() && "Cannot assign a name to void values!"??
        return builder->CreateGlobalString(this->text, ".str", 0, codegen->module.get(), true);
        /*
                llvm::Constant *strConst = llvm::ConstantDataArray::getString(codegen->context, this->text, true);

                // Create a global variable to hold the string
                llvm::GlobalVariable *gvar = new llvm::GlobalVariable(
                    codegen->module,
                    strConst->getType(),
                    true, // isConstant
                    llvm::GlobalValue::PrivateLinkage,
                    strConst,
                    ".str");

                gvar->setAlignment(llvm::Align(1));

                return gvar;
        */
    }

    llvm::Value *ReturnStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        if (!this->expr)
        {
            return builder->CreateRetVoid();
            // return llvm::ReturnInst::Create(codegen->context);
        }
        // return llvm::ReturnInst::Create(codegen->context, this->expr->codegen(codegen, builder));
        return builder->CreateRet(this->expr->codegen(codegen, builder));
    }

    llvm::Value *VarDeclStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        if (this->ir != nullptr)
        {
            return this->ir;
        }

        DEBUG() << this->toString() << " into " << builder->GetInsertBlock()->getNumber() << std::endl;
        auto value = (llvm::Value *)this->expr->codegen(codegen, builder);

        value->getType()->print(llvm::outs());

        // TODO Type should be handled before ?
        // this->ir = builder->CreateAlloca((llvm::Type*) this->type->codegen(codegen, builder), 0, value);
        // this->ir = builder->CreateAlloca(value->getType(), 0, value);
        this->ir = builder->CreateAlloca(value->getType(), 0, nullptr);
        builder->CreateStore(value, this->ir);

        return this->ir;
    }

    llvm::Value *Identifier::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;

        auto decl = ast_get_vardecl_by_name(this, this->identifier);

        return builder->CreateLoad(decl->ir->getAllocatedType(), decl->ir, this->identifier);
    }

    //
    // ast creation
    //
    LOGIA_API CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments)
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
    LOGIA_API StringLiteral *ast_create_string_literal(char *text)
    {
        // TODO review remove parentNode from constructor, is a leaf right?
        return new StringLiteral(nullptr, nullptr, text);
    }

    LOGIA_API ReturnStmt *ast_create_return(Expression *ret)
    {
        auto stmt = new ReturnStmt(nullptr, nullptr, ret);
        ret->parentNode = stmt;
        return stmt;
    }

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

    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, char *name, Type *return_type)
    {
        LOGIA_ASSERT(program);
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, program, Primitives::PRIMITIVE_FUNCTION);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.intrinsic = true;
        t->Function.body = nullptr;

        program->set(name, t);
        program->unshift_statement(t);

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

    LOGIA_API VarDeclStmt *ast_create_var_decl(Node *current, char *name, Type *type, Expression *expr)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        VarDeclStmt *variable = new VarDeclStmt(nullptr, nullptr, strdup(name), type, expr);
        expr->parentNode = variable;

        parentBody->set(strdup(name), variable);

        return variable;
    }

    LOGIA_API Identifier *ast_create_identifier(Node *current, char *name)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        return new Identifier(nullptr, nullptr, strdup(name));
    }

    //
    // ast fill
    //
    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, char *param_name, Expression *param_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isFunction());
        // LOGIA_ASSERT(*prop_name);

        auto param = s->Function.parameters.emplace_back(param_name, param_type, param_default_value);
        if (param_default_value)
        {
            param_default_value->parentNode = s;
        }
    }
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, char *prop_name, Expression *prop_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isStruct());
        // LOGIA_ASSERT(*prop_name);

        auto prop = s->Struct.properties.emplace_back(prop_name, nullptr, StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD, prop_type, prop_default_value);
        if (prop_default_value)
        {
            prop_default_value->parentNode = s;
        }
    }

    //
    // ast query
    //

    LOGIA_API LOGIA_LEND char *ast_types_to_string(ast_types type)
    {
        const char *a;
        const char *b;

        if ((ast_types::EXPRESSION & type) != 0)
        {
            a = "EXPRESSION.";
        }
        if ((ast_types::STMT & type) != 0)
        {
            a = "STMT.";
        }
        if ((ast_types::TYPE & type) != 0)
        {
            a = "TYPE.";
        }
        if ((ast_types::BODY & type) != 0)
        {
            a = "BODY.";
        }

        // remove all flags
        type = (ast_types)(type & ~ALL_FLAGS);

        switch (type)
        {
        case ast_types::PROGRAM:
            b = "PROGRAM";
            break;
        case ast_types::FUNCTION:
            b = "FUNCTION";
            break;
        case ast_types::CALL_EXPRESSION:
            b = "CALL_EXPRESSION";
            break;
        case ast_types::STRING_LITERAL:
            b = "STRING_LITERAL";
            break;
        case ast_types::FLOAT_LITERAL:
            b = "FLOAT_LITERAL";
            break;
        case ast_types::INTEGER_LITERAL:
            b = "INTEGER_LITERAL";
            break;
        case ast_types::RETURN_STMT:
            b = "RETURN_STMT";
            break;
        }
        char *dst = (char *)malloc(strlen(a) + strlen(b) + 1);
        strcpy(dst, a);
        strcpy(dst, b);

        return dst;
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
            current = current->parentNode;
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

    LOGIA_API VarDeclStmt *ast_get_vardecl_by_name(Node *current, char *name)
    {
        auto body = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(body); // this shall exists!
        // TODO cast if possible or nullptr!
        Node *n = body->lookup(name);
        if (n != nullptr)
        {
            LOGIA_ASSERT((n->type & ast_types::VAR_DECL_STMT) != 0);
            return (VarDeclStmt *)n;
        }
        return nullptr;
    }

}
