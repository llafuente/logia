#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/llvm.h"
#include "llvm/IR/Constant.h"
#include "logia/type_system.h"

namespace logia::AST
{
    //
    // Expression
    //

    Expression::Expression(antlr4::ParserRuleContext *rule) : Node(rule) {}
    std::string Expression::to_string()
    {
        return std::format("Expression{}", Node::to_string());
    }

    llvm::Value *Expression::post_codegen(logia::Backend *backend)
    {
        if (this->cg_value != nullptr)
        {
            DEBUG() << this->to_string() << std::endl;
            backend->set_debug_loc((llvm::Instruction *)this->cg_value, this->rule);
        }
        return Node::post_codegen(backend);
    }

    //
    // MemberAccessExpression
    //
    MemberAccessExpression::MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right) : Expression(rule)
    {
        this->push_child(left);
        this->push_child(right);
        right->skip_type_inference = true;
    }
    Expression *MemberAccessExpression::get_left()
    {
        return this->get_child<Expression>(0);
    }
    Identifier *MemberAccessExpression::get_right()
    {
        return this->get_child<Identifier>(1);
    }
    Type *MemberAccessExpression::get_type()
    {
        return this->type == nullptr ? new InferType() : this->type;
    }
    void MemberAccessExpression::set_type(Type *type)
    {
        this->is_typed = true;
        this->type = type;
    }
    Node *MemberAccessExpression::resolve()
    {
        this->pre_type_inference();
        return this->get_type();
    }

    void MemberAccessExpression::_pre_type_inference()
    {
        auto left = this->get_left();
        left->pre_type_inference();
        auto left_ty = left->get_type();

        if (!left_ty->is<Struct>())
        {
            throw_compiler_error("TODO! Only structs can be resolved atm.");
        }

        auto left_ty_stuct = left_ty->as<Struct>();
        auto right = this->get_right();
        auto ty = left_ty_stuct->get_field_type(right)->get_final_type();
        right->set_type(ty);
        this->set_type(ty);
        Node::_pre_type_inference();
    }

    std::string MemberAccessExpression::to_string()
    {
        return std::format("MemberAccessExpression {}", Node::to_string());
    }

    llvm::Value *MemberAccessExpression::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string_tree() << std::endl;
        // TODO handle left side to be a pointer to struct or struct itself, for now we assume it's always a pointer
        auto left = this->get_left();
        auto left_type = left->get_type();
        auto left_value = left->codegen(backend);

        if (!left_type->is<Struct>())
        {
            LERROR() << left->to_string_tree() << std::endl;
            throw_semantic_error(left, "Expected left to be a struct");
        }
        auto struct_ty = left_type->as<Struct>();

        auto right = this->get_right();
        if (!right->is<Identifier>())
        {
            LERROR() << left->to_string_tree() << std::endl;
            throw_semantic_error(left, "Expected right to be an identifier");
        }
        auto right_ident = right->as<Identifier>();

        auto field = struct_ty->get_field(right_ident->identifier);
        if (field == nullptr)
        {
            throw_semantic_error(left, std::format("struct '{}' do not contains a property with name '{}'", struct_ty->get_name(), right_ident->identifier));
        }

        // auto property_ty = (llvm::Type *)field->get_final_type()->codegen(backend);
        // left_value = llvm_load_if_required(left_value, backend);
        auto gep = backend->builder->CreateStructGEP(struct_ty->ir_type, left_value, field->index);
        this->cg_value = gep;

        return Expression::post_codegen(backend);
    }

    //
    // CallExpressionArgument
    //

    CallExpressionArgument::CallExpressionArgument(
        size_t index,
        Identifier *name,
        Expression *value) : Node(value->rule)
    {
        this->has_type = false;
        this->skip_type_inference = true;

        this->push_child(name);
        name->skip_codegen = true;
        name->skip_type_inference = true;
        name->has_type = false;
        this->push_child(value);
    }

    bool CallExpressionArgument::is_named()
    {
        return strlen(this->get_name()->identifier) > 0;
    }

    Identifier *CallExpressionArgument::get_name()
    {
        return this->get_child<Identifier>(0);
    }

    Expression *CallExpressionArgument::get_value()
    {
        return this->get_child<Expression>(1);
    }

    std::string CallExpressionArgument::to_string()
    {
        return std::format("CallExpressionArgument");
    }

    Type *CallExpressionArgument::get_type()
    {
        return this->get_value()->get_type();
    }

    void CallExpressionArgument::set_type(Type *type)
    {
        return this->get_value()->set_type(type);
        this->is_typed = true;
    }

    void CallExpressionArgument::_post_type_inference()
    {
        this->is_typed = this->get_value()->is_typed;
        Node::_post_type_inference();
    }

    //
    // CallExpression
    //
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule) : Expression(rule)
    {
    }
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments) : Expression(rule)
    {
        LOGIA_ASSERT(locator && "locator is mantadory");

        // these two rules are couple atm, but we should handle identifiers in other ways in the future...
        node_assert<Identifier, MemberAccessExpression>(locator, __FUNCTION__ ":" TOSTRING(__LINE__));
        locator->skip_type_inference = true;
        locator->skip_codegen = true;

        this->push_child(locator);
        for (size_t i = 0; i < positional_arguments.size(); ++i)
        {
            this->push_positional_argument(positional_arguments[i]);
        }
    }

    void CallExpression::push_named_argument(Identifier *name, Expression *expr)
    {
        LOGIA_ASSERT(name && "name is mantadory");
        LOGIA_ASSERT(expr && "expr is mantadory");

        node_assert<Identifier>(name, __FUNCTION__ ":" TOSTRING(__LINE__));
        node_assert<Expression>(expr, __FUNCTION__ ":" TOSTRING(__LINE__));

        this->push_child(new CallExpressionArgument(argument_count++, name, expr));
    }
    void CallExpression::push_positional_argument(Expression *expr)
    {
        return this->push_named_argument(new Identifier(expr->rule, ""), expr);
    }

    void CallExpression::insert_named_argument(size_t position, Identifier *name, Expression *expr)
    {
        this->push_named_argument(name, expr);
        // now we swap position and length-1 to put the new argument in the right place
        for (size_t i = this->argument_count - 1; i > position; --i)
        {
            std::swap(this->children[i], this->children[i - 1]);
            this->children[i]->as<CallExpressionArgument>()->index = i;
        }
    }

    void CallExpression::insert_positional_argument(size_t position, Expression *expr)
    {
        return this->insert_named_argument(position, ast_create_identifier((char *)""), expr);
    }

    void CallExpression::remove_argument_at(size_t position)
    {
        if (position >= this->argument_count)
        {
            throw_compiler_error("Argument position out of range");
        }
        this->children.erase(this->children.begin() + position + 1); // +1 to skip locator
        this->argument_count--;
        // update indices
        for (size_t i = position; i < this->argument_count; ++i)
        {
            this->children[i + 1]->as<CallExpressionArgument>()->index = i; // +1 to skip locator
        }
    }

    CallExpressionArgument *CallExpression::get_argument_by_name(const char *name)
    {
        for (size_t i = 1; i < this->children.size(); ++i)
        {
            auto arg = this->get_child<CallExpressionArgument>(i);
            auto arg_name = arg->get_name();
            if (arg_name->operator==(name))
            {
                return arg;
            }
        }
        return nullptr;
    }

    CallExpressionArgument *CallExpression::get_argument_by_index(uint32_t index)
    {
        if (index >= this->argument_count)
        {
            return nullptr;
        }
        return this->get_child<CallExpressionArgument>(index + 1);
    }

    Expression *CallExpression::get_locator()
    {
        return this->get_child<Expression>(0);
    }

    Expression *CallExpression::get_argument_expr(uint32_t pos)
    {

        return this->get_argument_by_index(pos)->get_value();
    }

    Identifier *CallExpression::get_argument_name(uint32_t pos)
    {
        return this->get_argument_by_index(pos)->get_name();
    }

    std::vector<Expression *> CallExpression::get_arguments()
    {
        auto v = std::vector<Expression *>();
        v.reserve(this->argument_count);

        DEBUG() << v.size() << "/" << v.capacity() << "/" << this->children.size() << std::endl;

        for (size_t i = 1; i < this->children.size(); ++i)
        {
            v.push_back(this->children[i]->as<CallExpressionArgument>()->get_value());
        }

        return v;
    }
    void CallExpression::_pre_type_inference()
    {
        // it's possible to solve at this point if the locator where final
        // maybe it's a problem with BinaryExpression ??
        // auto locator = this->get_locator();
        // locator->pre_type_inference();

        auto locator = this->get_locator();
        locator->skip_type_inference = false;
        locator->pre_type_inference();
        locator->post_type_inference();

        auto locator_ty = locator->get_type();
        locator->set_type(locator_ty);

        Function *f = nullptr;
        if (!locator_ty->try_cast<Function>(&f))
        {
            LERROR() << this->to_string_tree() << std::endl;
            LERROR() << locator_ty->to_string_tree() << std::endl;
            throw_semantic_error(this, std::format("LGERR033 This expression is not callable is: '{}'", locator_ty->get_repr()));
            // cannot be used as a function
        }

        this->is_typed = true;
        this->type = f->get_return_type()->get_final_type();

        Expression::_pre_type_inference();
    }

    Type *CallExpression::get_type()
    {
        return this->type;
    }

    std::string CallExpression::to_string()
    {
        auto locator = this->get_locator();
        if (!locator)
        {
            return std::string("CallExpression: incomplete");
        }

        auto arguments = this->get_arguments();
        return std::format("CallExpression[{} arguments]{}", arguments.size(), Node::to_string());
    }

    llvm::Value *CallExpression::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;

        if (this->children.size() == 0)
        {
            throw std::runtime_error("Incomplete CallExpression");
        }

        // Look up the name in the global module table.
        auto name = this->get_locator()->as<Identifier>();

        llvm::Function *CalleeF = backend->getFunction(name->identifier);
        if (!CalleeF)
        {
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->identifier);
        }

        auto arguments = this->get_arguments();
        // If argument mismatch error.
        if (CalleeF->arg_size() != arguments.size())
        {
            throw std::runtime_error(std::format("Expected arguments {} arguments passed {} calling {}", CalleeF->arg_size(), arguments.size(), name->identifier));
        }
        auto arg_itr = CalleeF->arg_begin();

        std::vector<llvm::Value *> ArgsV;
        for (size_t i = 0, e = arguments.size(); i != e; ++i)
        {
            DEBUG() << "argument[" << i << "]" << std::endl;

            auto argument = arguments[i];
            auto ir_argument = llvm_load_if_required(argument->codegen(backend), backend);
            auto ir_argument_ty = ir_argument->getType();

            auto ir_parameter = CalleeF->getArg(i);
            auto ir_parameter_ty = ir_parameter->getType();

            // check arguments type are compatible one by one
            if (ir_parameter_ty != ir_argument_ty)
            {
                LERROR() << this->to_string_tree();
                throw_semantic_error(argument, std::format("Invalid argument {} '{}' of type '{}' expected type '{}'", i + 1, name->identifier, llvm_type_to_string(ir_argument_ty), llvm_type_to_string(ir_parameter_ty)));
            }

            ArgsV.push_back(ir_argument);
            if (!ArgsV.back())
            {
                return nullptr;
            }
        }

        // @llafuente remove name or we got duplications (same if strategy ?)
        auto call = backend->builder->CreateCall(CalleeF, ArgsV);

        this->cg_value = (llvm::Value *)call;
        return Expression::post_codegen(backend);
    }

    LOGIA_API CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments)
    {
        auto callexpr = new CallExpression(nullptr, locator, arguments);

        return callexpr;
    }

    // TODO create

    //
    // BinaryExpression
    //

    std::string BinaryExpression::to_string()
    {
        return std::format("BinaryExpression [{}]", ast_operator_to_function_name(this->op), Expression::to_string());
    }

    BinaryExpression::BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, Operators op, Expression *right) : Expression(rule)
    {
        this->op = op;

        switch (op)
        {
        case Operators::BINARY_ASSIGN:
            this->push_child(left);
            break;
        case Operators::BINARY_ADD_ASSIGN:
        case Operators::BINARY_SUB_ASSIGN:
        case Operators::BINARY_MUL_ASSIGN:
        case Operators::BINARY_DIV_ASSIGN:
            this->push_child(new PrefixUnaryExpression(this->rule, Operators::PREFIX_DEREFERENCE, left));
            break;
        default:
            this->push_child(left);
            break;
        }
        this->push_child(right);
    }

    bool BinaryExpression::is_assignament()
    {
        switch (this->op)
        {
        case Operators::BINARY_ASSIGN:
        case Operators::BINARY_ADD_ASSIGN:
        case Operators::BINARY_SUB_ASSIGN:
        case Operators::BINARY_MUL_ASSIGN:
        case Operators::BINARY_DIV_ASSIGN:
            return true;
        }
        return false;
    }

    Type *BinaryExpression::get_type()
    {
        return this->type;
    }
    void BinaryExpression::set_type(Type *type)
    {
        this->type = type;
    }

    Expression *BinaryExpression::get_left()
    {
        return this->get_child<Expression>(0);
    }
    Expression *BinaryExpression::get_right()
    {
        return this->get_child<Expression>(1);
    }

    void BinaryExpression::_pre_type_inference()
    {
        auto left = this->get_left();
        left->pre_type_inference();
        auto left_ty = left->get_final_type();
        if (this->is_assignament())
        {
            if (left->is<ConstExpression>())
            {
                throw_semantic_error(this, std::format("LGER032 lhs cannot be a constant expression"));
            }
            if (left_ty == nullptr || left_ty->is<InferType>())
            {
                return; // TODO we cannot determine type atm! what we do ?
            }
            auto right = this->get_right();
            auto right_ty = right->get_final_type();

            auto err = type_system::type_is_compatible(left_ty, right_ty);
            if (err.is_error())
            {
                err.throw_semantic(this);
            }
            // rhs should have the same type!
            right->set_type(left_ty);
        }

        Expression::_pre_type_inference();
    }

    void BinaryExpression::_post_type_inference()
    {
        auto left = this->get_left();
        auto left_ty = left->get_final_type();
        if (left_ty->is<InferType>())
        {
            LERROR() << this->to_string_tree() << std::endl;
            throw_compiler_error("Unexpected left side infer type");
        }
        auto right = this->get_right();
        auto right_ty = right->get_final_type();
        if (right_ty->is<InferType>())
        {
            LERROR() << this->to_string_tree() << std::endl;
            throw_compiler_error("Unexpected right side infer type");
        }
        switch (op)
        {
        case Operators::BINARY_ASSIGN:
            this->set_type(left_ty);
            break;
        default:
            auto locator = new Identifier(this->rule, ast_binary_operator_to_string(op, left_ty->get_type(), right_ty->get_type()));
            this->call_expr = new CallExpression(this->rule, locator, {left, right});
            // makes no sense but need to keep this node attached
            this->push_child(this->call_expr);

            this->call_expr->pre_type_inference();
            this->call_expr->post_type_inference();
            this->set_type(this->call_expr->get_type());
        }

        this->is_typed = true;
        Expression::_post_type_inference();
    }
    llvm::Value *BinaryExpression::post_codegen(logia::Backend *backend)
    {
        auto left = this->get_left();
        auto left_value = left->codegen(backend);
        auto right = this->get_right();
        auto right_value = right->codegen(backend);
        switch (op)
        {
        case Operators::BINARY_ASSIGN:
            right_value = llvm_load_if_required(right_value, backend);
            auto store = backend->builder->CreateStore(right_value, left_value, false);
            backend->set_debug_loc((llvm::Instruction *)store, this->rule);
            this->cg_value = left_value;

            return left_value;
        }

        this->cg_value = this->call_expr->codegen(backend);

        return Expression::post_codegen(backend);
    }

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, Operators op, Expression *right)
    {
        BinaryExpression *expr = new BinaryExpression(nullptr, left, op, right);
        return expr;
    }

    // TODO create

    //
    // PrefixUnaryExpression
    //
    std::string PrefixUnaryExpression::to_string()
    {
        auto id = this->get_locator()->as<Identifier>();
        return std::format("PrefixUnaryExpression [{}] {}", id->identifier, Node::to_string());
    }

    PrefixUnaryExpression::PrefixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand) : CallExpression(rule)
    {
        this->op = op;

        // NOTE start as null, because we may don't know the types yet
        auto ident = ast_create_identifier("");
        ident->skip_codegen = true;
        this->push_child(ident);

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
            // node_assert<Identifier>(operand, __FUNCTION__ ":" TOSTRING(__LINE__));
            break;
        default:
        {
        }
        }

        this->push_positional_argument(operand);
    }

    Expression *PrefixUnaryExpression::get_operand()
    {
        return this->get_argument_expr(0);
    }

    Type *PrefixUnaryExpression::get_type()
    {
        // TODO this should be ptr when deferenced, but we use this type atm to generate callexpr identifier
        return this->get_operand()->get_final_type();
        /*
            switch (this->op)
            {
            case PrefixUnaryOperator::PREFIX_DEREFERENCE:
                return this->first_parent<Scope>()->lookup<Type>("ptr");
                break;
            default:
            }
        */
    }

    void PrefixUnaryExpression::_post_type_inference()
    {
        auto operand = this->get_operand()->get_final_type();
        auto ident = this->get_locator()->as<Identifier>();
        ident->identifier = strdup(ast_prefix_unary_operator_to_string(op, operand));

        CallExpression::_post_type_inference();
    }

    llvm::Value *PrefixUnaryExpression::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        {
            auto operand = this->get_operand();

            // auto operandValue = operand->codegen(codegen, builder);
            // auto operandValue = this->get_operand()->as<Identifier>()->get_var_decl()->alloca_inst;
            auto operandValue = this->get_operand()->codegen(backend);
            auto operandType = operandValue->getType();
            // return builder->CreateIntToPtr(operandValue, llvm::PointerType::get(codegen->context, 0));
            // return builder->CreateLoad(llvm::PointerType::get(codegen->context, 0), operandValue);
            // return builder->CreateLoad(operandType->getPointerTo(), operandValue, false);
            auto ptr = this->cg_value = backend->builder->CreateAlloca(operandType->getPointerTo(), nullptr, "deref");
            backend->set_debug_loc((llvm::Instruction *)ptr, this->rule);

            auto store = backend->builder->CreateStore(operandValue, ptr);
            backend->set_debug_loc((llvm::Instruction *)store, this->rule);

            // this->cg_value = backend->builder->CreateLoad(operandType->getPointerTo(), ptr);
            return Expression::post_codegen(backend);
        }
        default:
            return CallExpression::post_codegen(backend);
        }
    }

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_ref(Expression *operand)
    {
        PrefixUnaryExpression *expr = new PrefixUnaryExpression(nullptr, Operators::PREFIX_DEREFERENCE, operand);
        return expr;
    }

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_preunary_expr(Operators op, Expression *operand)
    {
        return new PrefixUnaryExpression(nullptr, op, operand);
    }

    // TODO create

    //
    // PostfixUnaryExpression
    //

    std::string PostfixUnaryExpression::to_string()
    {
        auto id = this->get_locator()->as<Identifier>();
        return std::format("PostfixUnaryExpression [{}]", id->identifier, Node::to_string());
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand) : CallExpression(rule)
    {
        this->op = op;

        // NOTE start as null, because we may don't know the types yet
        auto ident = ast_create_identifier("");
        ident->skip_codegen = true;
        this->push_child(ident);

        this->push_positional_argument(operand);
    }

    Expression *PostfixUnaryExpression::get_operand()
    {
        return this->get_argument_expr(0);
    }

    void PostfixUnaryExpression::_post_type_inference()
    {
        auto operand = this->get_operand()->get_final_type();
        auto ident = this->get_locator()->as<Identifier>();
        ident->identifier = strdup(ast_postfix_unary_operator_to_string(this->op, operand));

        CallExpression::_post_type_inference();
    }

    // TODO create

    //
    // Identifier
    //
    Identifier::Identifier(antlr4::ParserRuleContext *rule, LOGIA_CLONE const char *identifier) : Expression(rule)
    {
        this->identifier = strdup(identifier);
    }
    std::string Identifier::to_string()
    {
        return std::format("Identifier[{}]{}", this->identifier, Node::to_string());
    }

    llvm::Value *Identifier::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;

        auto decl = this->first_parent<Scope>()->lookup<Node>(this->identifier);
        if (decl->is<VarDeclStmt>())
        {
            this->cg_value = decl->as<VarDeclStmt>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        if (decl->is<FunctionParameter>())
        {
            this->cg_value = decl->as<FunctionParameter>()->alloca_inst;
            return Expression::post_codegen(backend);
        }
        // TODO function? -> function pointer
        throw_compiler_error(std::format("{}{}", "Identifier found but type not handled yet {}!", decl->to_string(), typeid(decl).name()));
    }
    bool Identifier::operator==(const char *id)
    {
        return strcmp(this->identifier, id) == 0;
    }
    bool Identifier::operator==(const Identifier *id)
    {
        return strcmp(this->identifier, id->identifier) == 0;
    }

    VarDeclStmt *Identifier::get_var_decl()
    {
        return this->first_parent<Block>()->lookup<VarDeclStmt>(this->identifier);
    }

    Function *Identifier::get_function_decl()
    {
        throw_compiler_error("todo");
    }

    void Identifier::post_attach()
    {
        Expression::post_attach();
        // TODO
    }

    Type *Identifier::get_type()
    {
        return this->type == nullptr ? new InferType() : this->type;
    }

    void Identifier::set_type(Type *t)
    {
        this->type = t;
        this->is_typed = true;
    }

    Node *Identifier::resolve()
    {
        if (this->identifier == nullptr || strlen(this->identifier) == 0)
        {
            return new InferType(); // TODO new ?
        }
        auto scope = this->first_parent<Scope>();
        return scope->lookup<Node>(this->identifier);
    }

    void Identifier::_pre_type_inference()
    {
        // this means my parent will type_inference this node!
        if (this->skip_type_inference)
        {
            return;
        }
        if (!this->is_typed)
        {
            this->set_type(this->resolve()->get_final_type());
        }
        Node::_pre_type_inference();
    }

    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name)
    {
        LOGIA_ASSERT(name);

        return new Identifier(nullptr, strdup(name));
    }

    //
    // StructInitializer
    //

    StructInitializer::StructInitializer(antlr4::ParserRuleContext *rule) : Expression(rule) {}
    std::string StructInitializer::to_string()
    {
        return std::format("{}{}", "StructInitializer", Expression::to_string());
    }

    void StructInitializer::set_type(Type *type)
    {
        if (this->is_typed)
        {
            throw_compiler_error("type was already set");
        }

        Struct *struct_ty;
        if (!type->try_cast<Struct>(&struct_ty))
        {
            throw_semantic_error(this, std::format("LGER030 incompatible type '{}', expected a struct", type->get_repr()));
        }

        this->is_typed = true;
        this->type = type;

        // TODO defaults!
        // search struct by name, if not found use the next position that should match the type, continue until defaults
        auto named_values = this->children;
        bool *used = new bool[struct_ty->field_count];

        // reset
        this->values = 0;
        this->children.clear(); // we will reorder children to match struct fields order, so we need to clear them first
        this->children.reserve(struct_ty->field_count * 2);

        for (auto i = 0; i < struct_ty->field_count; ++i)
        {
            used[i] = false;
            this->children.push_back(nullptr);
            this->children.push_back(nullptr);
        }

#if _DEBUG
        {
            std::string d = "";
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                d += used[i] ? "X" : "0";
            }
            DEBUG() << "fields: " << d << std::endl;
        }
#endif

        // map first all named values
        for (auto j = 0; j < named_values.size(); j += 2)
        {
            auto name = named_values[j];
            auto value_node = named_values[j + 1]->as<Expression>();
            if (name->is<NoOp>())
            {
                continue; // positional, will be handled later
            }
            auto value_name = name->as<Identifier>();

            auto field = struct_ty->get_field(value_name);
            used[field->index] = true;

            this->set_named_property(field->get_name(), value_node, field->index);
            // remove from named_values as it's used
            named_values.erase(named_values.begin() + j, named_values.begin() + j + 2);
            j -= 2;
        }
#if _DEBUG
        {
            std::string d = "";
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                d += used[i] ? "X" : "0";
            }
            DEBUG() << "fields: " << d << std::endl;
        }
#endif

        // second, map by position!
        for (auto j = 0; j < named_values.size(); j += 2)
        {
            auto value_node = named_values[j + 1]->as<Expression>();
            auto found = false;
            // search first field "not-used" and use it
            for (auto i = 0; i < struct_ty->field_count; ++i)
            {
                if (!used[i])
                {
                    used[i] = true;

                    auto field = struct_ty->get_field_by_index(i);
                    auto field_name = field->get_name();
                    this->set_named_property(field_name, value_node, field->index);
                    named_values.erase(named_values.begin() + j, named_values.begin() + j + 2);
                    j -= 2;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw_semantic_error(value_node, std::format("Too many initializers. Expected {}", struct_ty->field_count));
            }
        }
        assert(named_values.size() == 0);

        // third fill defaults!
        for (auto i = 0; i < struct_ty->field_count; ++i)
        {
            if (!used[i])
            {
                auto field = struct_ty->get_field_by_index(i);
                auto field_name = field->get_name();
                auto field_ty = field->get_type();
                auto field_default_value = field->get_default_value();
                if (field_default_value == nullptr)
                {
                    throw_semantic_error(this, std::format("LGER031 Missing initializer for field '{}' at position '{}' of type '{}'", field_name->identifier, i + 1, struct_ty->get_repr()));
                }
                this->set_named_property(field_name, field_default_value, field->index);
            }
        }

        StructInitializer *si = nullptr;
        int constant_count = 0;
        for (auto i = 0; i < struct_ty->field_count; ++i)
        {
            auto field_ty = struct_ty->get_field_by_index(i)->get_final_type();
            auto value = this->get_value_by_index(i);
            value->set_type(field_ty); // TODO check type compatibility ?

            if (value->try_cast<StructInitializer>(&si))
            {
                if (si->is_constant)
                {
                    ++constant_count;
                }
            }
            else if (value->is<ConstExpression>())
            {
                ++constant_count;
            }
        }
        this->is_constant = constant_count == struct_ty->field_count;
        delete used;
    }

    void StructInitializer::add_named_property(Identifier *name, Expression *value)
    {
        this->push_child(name);
        name->skip_codegen = true;
        name->skip_type_inference = true;
        this->push_child(value);

        ++this->values;
    }

    void StructInitializer::set_named_property(Identifier *name, Expression *value, uint32_t index)
    {
        this->children[index * 2] = name;
        this->children[index * 2 + 1] = value;
    }

    void StructInitializer::add_positional_property(Expression *value)
    {
        this->push_child(new NoOp());
        this->push_child(value);

        ++this->values;
    }

    Expression *StructInitializer::get_value_by_index(uint32_t index)
    {
        return this->get_child<Expression>((index * 2) + 1);
    }

    Expression *StructInitializer::get_value_by_name(const char *name)
    {
        throw_compiler_error("TODO!");
    }

    llvm::Value *StructInitializer::post_codegen(logia::Backend *backend)
    {
        if (!this->is_constant)
        {
            throw_semantic_error(this, "non-constant initialization not supported atm.");
        }

        auto &ctx = backend->context;
        const llvm::DataLayout &dl = backend->module->getDataLayout();

        auto struct_ty = this->get_type()->as<Struct>();

        auto v = std::vector<llvm::Constant *>();
        v.reserve(this->values);

        // skip first, it's the type

        for (auto field_index = 0, i = 0; i < this->children.size(); i += 2, ++field_index)
        {
            auto field_ty = struct_ty->get_field_by_index(field_index)->get_final_type();
            field_ty->codegen(backend);

            auto item = this->get_child<Expression>(i + 1);
            auto item_ty = item->get_final_type();
            item_ty->codegen(backend);

            auto ir_item_value = item->codegen(backend);
            auto cir_item_value = (llvm::Constant *)(ir_item_value);

            if (field_ty->ir_type != item_ty->ir_type)
            {
                throw_semantic_error(item, std::format("Expected type {} found type {}", field_ty->get_repr(), item_ty->get_repr()));
            }

            v.push_back(cir_item_value);
        }

        auto ir_struct_ty = (llvm::StructType *)struct_ty->codegen(backend);

        // 1) Constant initializer (replace with your child constants)
        llvm::Constant *init = llvm::ConstantStruct::get(ir_struct_ty, v);

        // 2a) If parent is a StructInitializer, means is a nested initialization
        // do not create the global variable
        auto p = this->parent_node;
        if (p->is<StructInitializer>())
        {
            this->cg_value = init;
            return Node::post_codegen(backend);
        }
        // if (p->is<Stmt>() || p->is<Block>())

        // 2b) Materialize constant in read-only global memory (memcpy source must be an address)
        auto *srcGlobal = new llvm::GlobalVariable(
            *backend->module,
            ir_struct_ty,
            true, // isConstant
            llvm::GlobalValue::PrivateLinkage,
            init,
            ".struct.init");

        auto abiAlign = llvm::Align(dl.getABITypeAlign(ir_struct_ty).value());
        // srcGlobal->setAlignment(llvm::Align(8));
        srcGlobal->setAlignment(abiAlign);

        this->cg_value = srcGlobal;
        // skip to Node -> LLVM crashes
        return Node::post_codegen(backend);
    }

    Type *StructInitializer::get_type()
    {
        return this->type; // TODO InferType
    }

    //
    // utils
    //
    char ast_binary_operator_to_string_buffer[256];
    const char *ast_postfix_unary_operator_to_string(Operators op, Type *operand)
    {
        switch (op)
        {
        case Operators::PREFIX_INCREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_postfix_inc_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::POSTFIX_DECREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_postfix_dec_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown postfix unary operator");
        }
    }
    const char *ast_prefix_unary_operator_to_string(Operators op, Type *operand)
    {
        switch (op)
        {
        case Operators::PREFIX_DEREFERENCE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_deref_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_INCREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_inc_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_DECREMENT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_dec_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_NEGATION:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_neg_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_BITWISE_NOT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_bitwise_not_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::PREFIX_LOGICAL_NOT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_prefix_logical_not_%s", operand->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown prefix unary operator");
        }
    }

    const char *ast_binary_operator_to_string(Operators op, Type *left, Type *right)
    {
        switch (op)
        {
        case Operators::BINARY_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_ADD_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_SUB_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_DIV_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MUL_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_ADD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_SUB:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MUL:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_DIV:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_MOD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mod_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;

        case Operators::BINARY_LOGIAL_EQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_eq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_NEQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_neq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_LT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_GT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_LTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGIAL_GTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGICAL_AND:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_logical_and_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_LOGICAL_OR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_logical_or_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;

        case Operators::BINARY_BITWISE_OR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_or_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_AND:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_and_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_XOR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_xor_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_LEFT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_shl_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case Operators::BINARY_BITWISE_RIGHT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_bitwise_shr_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown binary operator");
        }
    }
}