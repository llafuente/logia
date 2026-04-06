#include "ast.h"
#include "llvm/IR/TypedPointerType.h"
#include "utils.h"

namespace logia::AST
{
    //
    // constructors
    //
    BinaryExpression::BinaryExpression(antlr4::ParserRuleContext *rule, BinaryOperator op, Expression *left, Expression *right) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_binary_operator_to_string(op))));
        this->push_child(left);
        this->push_child(right);
    }

    PrefixUnaryExpression::PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_prefix_unary_operator_to_string(op))));
        this->push_child(operand);
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_postfix_unary_operator_to_string(op))));
        this->push_child(operand);
    }

    LOGIA_API Program *ast_create_program(llvm::LLVMContext &C)
    {
        auto body = new Program(nullptr, nullptr);

        // we know declare all primitives
        // any type in the language should use those
        // it's prohibited to create type using llvm
        // everything shall be supported directly

        Type *i8 = new Type(nullptr, Primitives::I8_TY);
        i8->Integer.bits = 8;
        i8->Integer.is_signed = true;
        i8->ir = llvm::Type::getInt8Ty(C);
        i8->parent_node = body;

        body->scope[(char *)"λi8"] = i8;

        Type *i16 = new Type(nullptr, Primitives::I16_TY);
        i16->Integer.bits = 16;
        i16->Integer.is_signed = true;
        i16->ir = llvm::Type::getInt16Ty(C);
        i16->parent_node = body;

        body->scope[(char *)"λi16"] = i16;

        Type *i32 = new Type(nullptr, Primitives::I32_TY);
        i32->Integer.bits = 32;
        i32->Integer.is_signed = true;
        i32->ir = llvm::Type::getInt32Ty(C);
        i32->parent_node = body;

        body->scope[(char *)"λi32"] = i32;

        Type *i64 = new Type(nullptr, Primitives::I64_TY);
        i64->Integer.bits = 64;
        i64->Integer.is_signed = true;
        i64->ir = llvm::Type::getInt64Ty(C);
        i64->parent_node = body;

        body->scope[(char *)"λi64"] = i64;

        Type *u8 = new Type(nullptr, Primitives::U8_TY);
        u8->Integer.bits = 8;
        u8->Integer.is_signed = true;
        u8->ir = llvm::Type::getInt8Ty(C);
        u8->parent_node = body;

        body->scope[(char *)"λu8"] = u8;

        Type *u16 = new Type(nullptr, Primitives::U16_TY);
        u16->Integer.bits = 16;
        u16->Integer.is_signed = true;
        u16->ir = llvm::Type::getInt16Ty(C);
        u16->parent_node = body;

        body->scope[(char *)"λu16"] = u16;

        Type *u32 = new Type(nullptr, Primitives::U32_TY);
        u32->Integer.bits = 32;
        u32->Integer.is_signed = true;
        u32->ir = llvm::Type::getInt32Ty(C);
        u32->parent_node = body;

        body->scope[(char *)"λu32"] = u32;

        Type *u64 = new Type(nullptr, Primitives::U64_TY);
        u64->Integer.bits = 64;
        u64->Integer.is_signed = true;
        u64->ir = llvm::Type::getInt64Ty(C);
        u64->parent_node = body;

        body->scope[(char *)"λu64"] = u64;

        Type *f16 = new Type(nullptr, Primitives::F16_TY);
        f16->Float.bits = 16;
        f16->ir = llvm::Type::getHalfTy(C);
        f16->parent_node = body;

        body->scope[(char *)"λf16"] = f16;

        Type *f32 = new Type(nullptr, Primitives::F32_TY);
        f32->Float.bits = 32;
        f32->ir = llvm::Type::getFloatTy(C);
        f32->parent_node = body;

        body->scope[(char *)"λf32"] = f32;

        Type *f64 = new Type(nullptr, Primitives::F64_TY);
        f64->Float.bits = 64;
        f64->ir = llvm::Type::getDoubleTy(C);
        f64->parent_node = body;

        body->scope[(char *)"λf64"] = f64;

        Type *f128 = new Type(nullptr, Primitives::F128_TY);
        f128->Float.bits = 64;
        f128->ir = llvm::Type::getFP128Ty(C);
        f128->parent_node = body;

        body->scope[(char *)"λf128"] = f128;

        Type *lvoid = new Type(nullptr, Primitives::VOID_TY);
        lvoid->ir = llvm::Type::getVoidTy(C);
        lvoid->parent_node = body;

        body->scope[(char *)"λvoid"] = lvoid;

        // TODO study opaque pointers, while seem what we need
        Type *ptr = new Type(nullptr, Primitives::PTR_TY);
        // opaque pointer, do not store information about pointee
        ptr->ir = llvm::PointerType::get(C, 0);
        body->scope[(char *)"λptr"] = ptr;
        ptr->parent_node = body;

        return body;
    }

    LOGIA_API Body *ast_create_body(Node *parentNode)
    {
        LOGIA_ASSERT(parentNode);

        auto parentBody = (Body *)ast_find_closest_parent(parentNode, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        return new Body(nullptr, parentBody);
    }
    LOGIA_API LOGIA_LEND StringLiteral *ast_create_string_lit(char *text)
    {
        // TODO review remove parentNode from constructor, is a leaf right?
        return new StringLiteral(nullptr, text);
    }
    LOGIA_API LOGIA_LEND FloatLiteral *ast_create_float_lit(Body *body, double value)
    {
        return new FloatLiteral(nullptr, (Type *)body->lookup(strdup("λf64")), value);
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_int_lit(Body *body, int64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λi64")), value);
    }
    LOGIA_API LOGIA_LEND IntegerLiteral *ast_create_uint_lit(Body *body, uint64_t value)
    {
        return new IntegerLiteral(nullptr, (Type *)body->lookup(strdup("λu64")), value);
    }
    LOGIA_API LOGIA_LEND GotoStmt *ast_create_goto_stmt(Body *body, char *name)
    {
        return new GotoStmt(nullptr, name);
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
        std::string scope;
        for (const auto &pair : this->scope)
        {
            scope += scope.empty() ? "" : ", ";
            scope += pair.first;
        }

        return std::string("body[") + std::string(itoa(this->children.size(), buffer, 10)) + "] scope: " + scope;
    }
    std::string Type::toString()
    {
        std::string list;
        switch (this->primitive)
        {
        case Primitives::FUNCTION_TY:
            // concat each parameter type
            for (auto &param : this->Function.parameters)
            {
                if (!list.empty())
                {
                    list += ", ";
                }
                list += param.type->toString();
            }
            return std::string("Type[") + this->Function.return_type->toString() + " function " + this->Function.name + "(" + list + ")" + "]";
        case Primitives::STRUCT_TY:
            // concat all properties with their type
            for (auto &prop : this->Struct.properties)
            {
                if (prop.isField())
                {
                    if (!list.empty())
                    {
                        list += ", ";
                    }
                    list += prop.type->toString();
                }
            }
            return std::string("Type[struct ") + this->Struct.name + " {" + list + "}";
        }

        return std::string("Type[") + ast_primitives_to_string(this->primitive) + "]";
    }

    std::string CallExpression::toString()
    {
        char buffer[36];
        auto arguments = this->get_arguments();
        return std::string("CallExpression: ") + this->get_locator()->toString() + "(" + std::string(itoa(arguments.size(), buffer, 10)) + " args)";
    }

    std::string IntegerLiteral::toString()
    {
        DEBUG() << this->get_type() << std::endl;
        DEBUG() << this->get_type()->toString() << std::endl;
        DEBUG() << this->ivalue << std::endl;
        DEBUG() << this->uvalue << std::endl;

        char buffer[36];
        return std::string("IntegerLiteral[") + this->get_type()->toString() + "] = " + std::string(itoa(this->ivalue, buffer, 10));
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

    std::string MemberAccessExpression::toString()
    {
        return std::string("MemberAccessExpression: ") + this->get_left()->toString() + "." + this->get_right()->toString();
    }
    std::string BinaryExpression::toString()
    {
        return std::string("BinaryExpression: ") + ast_binary_operator_to_string(this->op) + "(" + this->get_left()->toString() + ", " + this->get_right()->toString() + ")";
    }
    std::string PrefixUnaryExpression::toString()
    {
        return std::string("PrefixUnaryExpression: ") + ast_prefix_unary_operator_to_string(this->op) + "(" + this->get_operand()->toString() + ")";
    }
    std::string PostfixUnaryExpression::toString()
    {
        return std::string("PostfixUnaryExpression: ") + ast_postfix_unary_operator_to_string(this->op) + "(" + this->get_operand()->toString() + ")";
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
    std::string IfStmt::toString()
    {
        return std::string("IfStmt: ");
    }
    std::string GotoStmt::toString()
    {
        return std::string("GotoStmt: ") + this->name;
    }

    llvm::BasicBlock *Body::create_llvm_block(logia::Backend *codegen, char *name)
    {
        if (this->llvm_basicblock)
        {
            return this->llvm_basicblock;
        }

        DEBUG() << this->toString() << std::endl;

        this->llvm_basicblock = llvm::BasicBlock::Create(codegen->context, name, nullptr);
        return this->llvm_basicblock;
    }

    ///
    /// codegen
    ///
    llvm::Value *Body::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;

        // not the main program -> override block!
        if ((type & ast_types::PROGRAM) == 0)
        {
            this->create_llvm_block(codegen, nullptr);
            builder = new llvm::IRBuilder<>(codegen->context);
            builder->SetInsertPoint(this->llvm_basicblock);
            /*
            auto p = this->parentNode;
            if ((p->type & ast_types::BLOCK) != 0) {
                auto p = (Body*) p;
                BB->insertInto(nullptr, p->llvm_basicblock);
            } else if ((p->type & ast_types::TYPE) != 0) {
                auto pfunc = (Type*)(p);
                BB->insertInto(pfunc->Function.functionIR);
            }
            else if ((p->type & ast_types::STMT) != 0) {
                p->parentNode = p;
                if ((p->type & ast_types::BLOCK) != 0) {
                    throw std::runtime_error(std::string("invalid stmt, has no parent body: ") + p->toString());
                }

                auto pfunc = (Type*)(p);

            } else {
                throw std::runtime_error(std::string("invalid parent for body: ") + p->toString() );
            }
            */
        }

        for (int i = 0; i < this->children.size(); i++)
        {
            Node *n = this->children[i];
            DEBUG() << "codegen.statement[" << i << "] " << n->toString() << std::endl;
            auto inst = n->codegen(codegen, builder);
        }

        return this->llvm_basicblock;
    }

    llvm::Value *Type::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir)
        {
            return (llvm::Value *)this->ir;
        }

        switch (this->primitive)
        {
        case Primitives::FUNCTION_TY:
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

                auto BB = (llvm::BasicBlock *)this->Function.body->create_llvm_block(codegen, (char *)"entry");
                BB->insertInto(this->Function.functionIR);
                this->Function.body->codegen(codegen, builder);
            }

            return (llvm::Value *)this->ir;
        }
        break;
        case Primitives::STRUCT_TY:
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
        auto name = (StringLiteral *)this->get_locator();
        llvm::Function *CalleeF = codegen->module->getFunction(name->text);

        auto arguments = this->get_arguments();
        // auto x = codegen->module->getValueSymbolTable();
        // x->
        if (!CalleeF)
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->text);

        // If argument mismatch error.
        if (CalleeF->arg_size() != arguments.size())
            throw std::exception("Incorrect # arguments passed");

        std::vector<llvm::Value *> ArgsV;
        for (unsigned i = 0, e = arguments.size(); i != e; ++i)
        {
            DEBUG() << "argument[" << i << "]" << std::endl;
            ArgsV.push_back(arguments[i]->codegen(codegen, builder));
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
        return llvm::ConstantInt::get((llvm::Type *)this->get_type()->codegen(codegen, builder), llvm::APInt(this->get_type()->Integer.bits, this->ivalue, this->get_type()->Integer.is_signed));
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
        auto expr = this->get_expr();
        if (!expr)
        {
            return builder->CreateRetVoid();
            // return llvm::ReturnInst::Create(codegen->context);
        }
        // return llvm::ReturnInst::Create(codegen->context, this->expr->codegen(codegen, builder));
        return builder->CreateRet(expr->codegen(codegen, builder));
    }

    llvm::Value *VarDeclStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        if (this->ir != nullptr)
        {
            return this->ir;
        }

        DEBUG() << this->toString() << " into ??" << /*builder->GetInsertBlock()->getNumber() <<*/ std::endl;
        auto value = (llvm::Value *)this->get_expr()->codegen(codegen, builder);

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

    llvm::Value *MemberAccessExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        // TODO handle left side to be a pointer to struct or struct itself, for now we assume it's always a pointer
        auto left = this->get_left();
        auto right = this->get_right();

        auto leftValue = left->codegen(codegen, builder);
        auto rightIdent = (Identifier *)right;
        auto structType = left->get_type();

        // find property index
        int propertyIndex = -1;
        for (int i = 0; i < structType->Struct.properties.size(); ++i)
        {
            if (strcmp(structType->Struct.properties[i].name, rightIdent->identifier) == 0)
            {
                propertyIndex = i;
                break;
            }
        }
        if (propertyIndex == -1)
        {
            throw std::runtime_error(std::string("Unknown struct property: ") + rightIdent->identifier);
        }

        return builder->CreateStructGEP(structType->ir, leftValue, propertyIndex);
    }

    llvm::Value *IfStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;

        auto condition = this->get_condition()->codegen(codegen, builder);
        auto then_body = this->get_then();
        auto else_body = this->get_else();
        auto then_block = then_body->create_llvm_block(codegen, (char *)"thenblock");
        auto else_block = else_body->create_llvm_block(codegen, (char *)"elseblock");

        // NOTE create before codegen each block so the blocks are attached to function before codegen
        auto v = builder->CreateCondBr(condition, then_block, else_block);
        auto func = builder->GetInsertBlock()->getParent();
        func->insert(func->end(), then_block);
        func->insert(func->end(), else_block);

        then_body->codegen(codegen, builder);
        else_body->codegen(codegen, builder);

        return v;
    }

    llvm::Value *GotoStmt::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->toString() << std::endl;
        // find label and jump to it
        // function shall be inside the closest function
        auto label = (Body *)ast_find_closest_parent(this, (ast_types)(ast_types::FUNCTION | ast_types::BODY));
        if (!label)
        {
            throw std::runtime_error(std::string("goto statement has no parent function or body: ") + this->toString());
        }
        // TODO generate before or wait until generated to continue ?
        return builder->CreateBr(label->llvm_basicblock);
    }

    //
    // ast creation
    //
    LOGIA_API CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments)
    {
        LOGIA_ASSERT((locator->type & ast_types::EXPRESSION) != 0);
        // TODO LOGIA_ASSERT_ALL(arguments, .type & ast_types::EXPRESSION != 0);

        auto callexpr = new CallExpression(nullptr, locator, arguments);

        return callexpr;
    }

    LOGIA_API ReturnStmt *ast_create_return(Expression *ret)
    {
        auto stmt = new ReturnStmt(nullptr, ret);
        return stmt;
    }

    Type *ast_create_function_type(Body *parentBody, char *name, Type *return_type)
    {
        LOGIA_ASSERT(parentBody);
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, Primitives::FUNCTION_TY);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.body = new Body(nullptr, parentBody);
        t->Function.body->type = (ast_types)(ast_types::FUNCTION | ast_types::BODY);
        t->push_child(t->Function.body);
        parentBody->set(name, t);

        return t;
    }

    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, char *name, Type *return_type)
    {
        LOGIA_ASSERT(program);
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, Primitives::FUNCTION_TY);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.intrinsic = true;
        t->Function.body = nullptr;

        program->set(name, t);
        program->unshift_child(t);

        return t;
    }

    LOGIA_API Type *ast_create_struct_type(Body *current, char *name)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        Type *t = new Type(nullptr, Primitives::STRUCT_TY);
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

        VarDeclStmt *variable = new VarDeclStmt(nullptr, strdup(name), type, expr);

        parentBody->set(strdup(name), variable);

        return variable;
    }

    LOGIA_API Identifier *ast_create_identifier(Node *current, char *name)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Body *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        return new Identifier(nullptr, strdup(name));
    }

    LOGIA_API LOGIA_LEND IfStmt *ast_create_if(Expression *condition)
    {
        LOGIA_ASSERT(condition);

        return new IfStmt(nullptr, condition);
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
        // TODO REVIEW default values are not push ?
        if (param_default_value)
        {
            param_default_value->parent_node = s;
        }
        s->Function.body->set(param_name, param_type);
    }
    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, char *prop_name, Expression *prop_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isStruct());
        // LOGIA_ASSERT(*prop_name);

        auto prop = s->Struct.properties.emplace_back(prop_name, nullptr, StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD, prop_type, prop_default_value);
        // TODO REVIEW default values are not push ?
        if (prop_default_value)
        {
            prop_default_value->parent_node = s;
        }
    }

    //
    // ast query
    //

    void ast_traverse(Node *current, ast_traverse_callback_t cb)
    {
        auto t = current->type;
        if ((t & ast_types::PROGRAM) != 0)
        {
        }
    }

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
        type = (ast_types)(type & ~ast_types::ALL_FLAGS);

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

    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim)
    {
        switch (prim)
        {
        case Primitives::VOID_TY:
            return strdup("void");
        case Primitives::I8_TY:
            return strdup("i8");
        case Primitives::I16_TY:
            return strdup("i16");
        case Primitives::I32_TY:
            return strdup("i32");
        case Primitives::I64_TY:
            return strdup("i64");
        case Primitives::U8_TY:
            return strdup("u8");
        case Primitives::U16_TY:
            return strdup("u16");
        case Primitives::U32_TY:
            return strdup("u32");
        case Primitives::U64_TY:
            return strdup("u64");
        case Primitives::F16_TY:
            return strdup("f16");
        case Primitives::F32_TY:
            return strdup("f32");
        case Primitives::F64_TY:
            return strdup("f64");
        case Primitives::F128_TY:
            return strdup("f128");
        case Primitives::BOOL_TY:
            return strdup("bool");
        case Primitives::PTR_TY:
            return strdup("ptr");
        case Primitives::STRUCT_TY:
            return strdup("struct");
        case Primitives::FUNCTION_TY:
            return strdup("function");
        default:
            throw std::exception("unreachable");
        }
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
            current = current->parent_node;
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

    //
    // utils
    //
    const char *ast_postfix_unary_operator_to_string(PostfixUnaryOperator op)
    {
        switch (op)
        {
        case PostfixUnaryOperator::INCREMENT:
            return "logia_intrinsics_postfix_inc";
        case PostfixUnaryOperator::DECREMENT:
            return "logia_intrinsics_postfix_dec";
        default:
            throw std::runtime_error("Unknown postfix unary operator");
        }
    }
    const char *ast_prefix_unary_operator_to_string(PrefixUnaryOperator op)
    {
        switch (op)
        {
        case PrefixUnaryOperator::INCREMENT:
            return "logia_intrinsics_prefix_inc";
        case PrefixUnaryOperator::DECREMENT:
            return "logia_intrinsics_prefix_dec";
        case PrefixUnaryOperator::NEGATION:
            return "logia_intrinsics_prefix_neg";
        case PrefixUnaryOperator::BITWISE_NOT:
            return "logia_intrinsics_prefix_bitwise_not";
        case PrefixUnaryOperator::LOGICAL_NOT:
            return "logia_intrinsics_prefix_logical_not";
        default:
            throw std::runtime_error("Unknown prefix unary operator");
        }
    }

    const char *ast_binary_operator_to_string(BinaryOperator op)
    {
        switch (op)
        {
        case BinaryOperator::ADD:
            return "logia_intrinsics_bin_add";
        case BinaryOperator::SUB:
            return "logia_intrinsics_bin_sub";
        case BinaryOperator::MUL:
            return "logia_intrinsics_bin_mul";
        case BinaryOperator::DIV:
            return "logia_intrinsics_bin_div";
        case BinaryOperator::MOD:
            return "logia_intrinsics_bin_mod";
        case BinaryOperator::EQ:
            return "logia_intrinsics_bin_eq";
        case BinaryOperator::NEQ:
            return "logia_intrinsics_bin_neq";
        case BinaryOperator::LT:
            return "logia_intrinsics_bin_lt";
        case BinaryOperator::GT:
            return "logia_intrinsics_bin_gt";
        case BinaryOperator::LTE:
            return "logia_intrinsics_bin_lte";
        case BinaryOperator::GTE:
            return "logia_intrinsics_bin_gte";
        case BinaryOperator::AND:
            return "logia_intrinsics_bin_and";
        case BinaryOperator::OR:
            return "logia_intrinsics_bin_or";
        case BinaryOperator::XOR:
            return "logia_intrinsics_bin_xor";
        case BinaryOperator::SHL:
            return "logia_intrinsics_bin_shl";
        case BinaryOperator::SHR:
            return "logia_intrinsics_bin_shr";
        default:
            throw std::runtime_error("Unknown binary operator");
        }
    }
}
