#include "ast/expr.h"
#include "ast/constexpr.h"
#include "ast/traverse.h"

namespace logia::AST
{
    //
    // Expression
    //

    Expression::Expression(antlr4::ParserRuleContext *rule, ast_types type) : Node(rule, (ast_types)(type | ast_types::EXPRESSION)) {}
    std::string Expression::to_string()
    {
        return std::format("Expression ({:p})", static_cast<void *>(this));
    }

    //
    // MemberAccessExpression
    //
    MemberAccessExpression::MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right) : Expression(rule, (ast_types)(ast_types::EXPRESSION | ast_types::MEMBER_ACCESS))
    {
        this->push_child(left);
        this->push_child(right);
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
        return this->resolve()->get_type();
    }
    Node *MemberAccessExpression::resolve()
    {
        auto left = this->get_left()->resolve();
        auto left_type = left->get_type();
        LOGIA_ASSERT(left_type->isStruct(), "only structs can be resolved atm.");
        auto left_as_struct = left_type->as<Struct>();
        return left_as_struct->get_field_type(this->get_right());
    }

    //
    // CallExpression
    //
    CallExpression::CallExpression() : Expression(nullptr, ast_types::CALL_EXPRESSION)
    {
    }
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments) : Expression(rule, ast_types::CALL_EXPRESSION)
    {
        LOGIA_ASSERT(locator && "locator is mantadory");
        node_assert<Identifier, MemberAccessExpression>(locator, __FUNCTION__ ":" TOSTRING(__LINE__));

        this->push_child(locator);
        for (int i = 0; i < positional_arguments.size(); ++i)
        {
            this->add_positional_argument(positional_arguments[i]);
        }
    }

    void CallExpression::add_named_argument(Identifier *id, Expression *expr)
    {
        LOGIA_ASSERT(id && "id is mantadory");
        LOGIA_ASSERT(expr && "expr is mantadory");
        node_assert<Identifier>(id, __FUNCTION__ ":" TOSTRING(__LINE__));
        node_assert<Expression>(expr, __FUNCTION__ ":" TOSTRING(__LINE__));

        this->push_child(id);
        this->push_child(expr);
    }
    void CallExpression::add_positional_argument(Expression *expr)
    {
        LOGIA_ASSERT(expr && "expr is mantadory");
        node_assert<Expression>(expr, __FUNCTION__ ":" TOSTRING(__LINE__));

        this->push_child(ast_create_identifier((char *)"")); // TODO maybe empty identifier ?!
        this->push_child(expr);
    }

    Expression *CallExpression::get_locator()
    {
        return this->get_child<Expression>(0);
    }
    Expression *CallExpression::get_argument(uint32_t pos)
    {
        return this->get_child<Expression>(1 + (pos * 2) + 1);
    }
    Identifier *CallExpression::get_argument_name(uint32_t pos)
    {
        return this->get_child<Identifier>(1 + (pos * 2) + 0);
    }
    std::vector<Node *> CallExpression::get_arguments()
    {
        auto v = std::vector<Node *>();
        v.reserve((this->children.size() - 1) / 2);
        // TODO this will sort and match the callee parameters
        // return std::vector<Node *>(this->children.begin() + 1, this->children.end());

        DEBUG() << v.size() << "/" << v.capacity() << "/" << this->children.size() << std::endl;

        for (int i = 1; i < this->children.size();)
        {
            // TODO handle position and named
            // solve locator because we will need it to check
            DEBUG() << "name [" << i << "]= " << this->children[i]->to_string() << std::endl;
            ++i;
            DEBUG() << "argument[" << i << "] = " << this->children[i]->to_string() << std::endl;
            v.push_back(children[i]);
            ++i;
        }

        return v;
    }
    Type *CallExpression::get_type()
    {
        // CallExpression should point to a function
        Function *f = this->get_locator()->get_type()->as<Function>();

        return f->get_return_type();
    }

    std::string CallExpression::to_string()
    {
        auto locator = this->get_locator();
        if (!locator)
        {
            return std::string("CallExpression: incomplete");
        }

        auto arguments = this->get_arguments();
        return std::format("CallExpression[{} arguments] ({:p})", arguments.size(), static_cast<void *>(this));
    }

    llvm::Value *CallExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

        if (this->children.size() == 0)
        {
            throw std::runtime_error("Incomplete CallExpression");
        }

        // Look up the name in the global module table.
        auto name = this->get_locator()->as<Identifier>();

        llvm::Function *CalleeF = codegen->module->getFunction(name->identifier);
        if (!CalleeF)
        {
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->identifier);
        }

        auto arguments = this->get_arguments();
        // If argument mismatch error.
        if (CalleeF->arg_size() != arguments.size())
        {
            throw std::runtime_error(std::format("Expected arguments {} arguments passed {}", CalleeF->arg_size(), arguments.size()));
        }
        auto arg_itr = CalleeF->arg_begin();

        std::vector<llvm::Value *> ArgsV;
        for (unsigned i = 0, e = arguments.size(); i != e; ++i)
        {
            auto callee_arg = CalleeF->getArg(i);
            DEBUG() << "argument[" << i << "]" << std::endl;
            auto caller_arg = arguments[i]->codegen(codegen, builder);

            // check arguments type are compatible one by one
            if (callee_arg->getType() != caller_arg->getType())
            {
                auto message = std::format("Invalid argument {} of type {} expected type {}", i, llvm_type_to_string(caller_arg->getType()), llvm_type_to_string(callee_arg->getType()));
                DEBUG() << message << std::endl
                        << arguments[i]->to_string_tree();
                throw std::runtime_error(message);
            }

            ArgsV.push_back(caller_arg);
            if (!ArgsV.back())
            {
                return nullptr;
            }
        }
        // NOTE name is not what i expect -> blank!
        return builder->CreateCall(CalleeF, ArgsV);
        // return builder->CreateCall(CalleeF, ArgsV, "call");
        //  return (llvm::Value*) llvm::CallInst::Create(CalleeF, ArgsV, "call");
    }

    LOGIA_API CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments)
    {
        auto callexpr = new CallExpression(nullptr, locator, arguments);

        return callexpr;
    }

    //
    // MemberExpression
    //
    std::string MemberAccessExpression::to_string()
    {
        return std::format("MemberAccessExpression[left {} / right {}] ({:p})", this->get_left()->to_string(), this->get_right()->to_string(), static_cast<void *>(this));
    }

    llvm::Value *MemberAccessExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // TODO handle left side to be a pointer to struct or struct itself, for now we assume it's always a pointer
        auto left = this->get_left();
        auto right = this->get_right();

        auto leftValue = left->codegen(codegen, builder);
        auto rightIdent = right->as<Identifier>();
        auto left_type = left->get_type();
        LOGIA_ASSERT(left_type->isStruct() && "left should be a struct");
        auto struct_ty = left_type->as<Struct>();

        int propertyIndex = struct_ty->get_field_index(rightIdent);
        if (propertyIndex == -1)
        {
            throw std::runtime_error(std::string("Unknown struct property: ") + rightIdent->identifier);
        }

        return builder->CreateStructGEP(struct_ty->llvm_type, leftValue, propertyIndex);
    }

    // TODO create

    //
    // BinaryExpression
    //

    std::string BinaryExpression::to_string()
    {
        auto id = this->get_locator()->as<Identifier>();
        return std::format("BinaryExpression[{}({}, {}) ({:p})", id->identifier, this->get_left()->to_string(), this->get_right()->to_string(), static_cast<void *>(this));
    }

    BinaryExpression::BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, BinaryOperator op, Expression *right) : CallExpression()
    {
        this->op = op;
        // NOTE start as null, because we may don't know the types yet
        // this->push_child(ast_create_identifier(strdup(ast_binary_operator_to_string(op, left->get_type(), right->get_type()))));
        this->push_child(ast_create_identifier(""));
        switch (op)
        {
        case BinaryOperator::ASSIGN:
        case BinaryOperator::ADD_ASSIGN:
        case BinaryOperator::SUB_ASSIGN:
        case BinaryOperator::MUL_ASSIGN:
        case BinaryOperator::DIV_ASSIGN:
            // 1 NoOp
            // 2 ref
            this->add_positional_argument(ast_create_ref(left));
            break;
        default:
            // 1 NoOp
            // 2 expr
            this->add_positional_argument(left);
            break;
        }
        // 3 NoOp
        // 4 expr
        this->add_positional_argument(right);
    }

    Expression *BinaryExpression::get_left()
    {
        return this->get_argument(0);
    }
    Expression *BinaryExpression::get_right()
    {
        return this->get_argument(1);
    }

    bool BinaryExpression::pre_type_inference()
    {
        return true;
    }
    void BinaryExpression::post_type_inference()
    {
        auto left = this->get_left()->get_type();
        auto right = this->get_right()->get_type();
        auto ident = this->get_locator()->as<Identifier>();
        ident->identifier = strdup(ast_binary_operator_to_string(op, left, right));
    }

    LOGIA_API LOGIA_LEND BinaryExpression *ast_create_binary_expr(Expression *left, BinaryOperator op, Expression *right)
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
        return std::format("PrefixUnaryExpression[{}({}) ({:p})", ast_prefix_unary_operator_to_string(this->op), this->get_operand()->to_string(), static_cast<void *>(this));
    }

    PrefixUnaryExpression::PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand) : CallExpression()
    {
        this->op = op;
        switch (this->op)
        {
        case PrefixUnaryOperator::DEREFERENCE:
            this->push_child(new NoOp());
            node_assert<Identifier>(operand, __FUNCTION__ ":" TOSTRING(__LINE__));
            break;
        default:
            this->push_child(ast_create_identifier(strdup(ast_prefix_unary_operator_to_string(op))));
        }
        this->add_positional_argument(operand);
    }

    Expression *PrefixUnaryExpression::get_operand()
    {
        return this->get_argument(0);
    }

    Type *PrefixUnaryExpression::get_type()
    {
        // TODO REVIEW impl totally false!
        return this->get_operand()->get_type();
    }

    llvm::Value *PrefixUnaryExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

        switch (this->op)
        {
        case PrefixUnaryOperator::DEREFERENCE:
        {
            auto operand = this->get_operand();

            // auto operandValue = operand->codegen(codegen, builder);
            auto operandValue = this->get_operand()->as<Identifier>()->get_var_decl()->ir;
            auto operandType = operandValue->getType();
            // return builder->CreateIntToPtr(operandValue, llvm::PointerType::get(codegen->context, 0));
            // return builder->CreateLoad(llvm::PointerType::get(codegen->context, 0), operandValue);
            // return builder->CreateLoad(operandType->getPointerTo(), operandValue, false);
            auto ptr = builder->CreateAlloca(operandType->getPointerTo(), nullptr, "deref");
            builder->CreateStore(operandValue, ptr);
            return builder->CreateLoad(operandType->getPointerTo(), ptr);
        }
        default:
            return CallExpression::codegen(codegen, builder);
        }
    }

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_ref(Expression *operand)
    {
        PrefixUnaryExpression *expr = new PrefixUnaryExpression(nullptr, PrefixUnaryOperator::DEREFERENCE, operand);
        return expr;
    }

    LOGIA_API LOGIA_LEND PrefixUnaryExpression *ast_create_preunary_expr(PrefixUnaryOperator op, Expression *operand)
    {
        return new PrefixUnaryExpression(nullptr, op, operand);
    }

    // TODO create

    //
    // PostfixUnaryExpression
    //

    std::string PostfixUnaryExpression::to_string()
    {
        return std::format("PostfixUnaryExpression[{}({})] ({:p})", ast_postfix_unary_operator_to_string(this->op), this->get_operand()->to_string(), static_cast<void *>(this));
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand) : CallExpression()
    {
        this->op = op;
        this->push_child(ast_create_identifier(strdup(ast_postfix_unary_operator_to_string(op))));
        this->add_positional_argument(operand);
    }

    Expression *PostfixUnaryExpression::get_operand()
    {
        return this->get_child<Expression>(1);
    }

    // TODO create

    //
    // Identifier
    //
    Identifier::Identifier(antlr4::ParserRuleContext *rule, const char *identifier) : Expression(rule, ast_types::IDENTIFIER)
    {
        LOGIA_ASSERT(type);
        this->identifier = identifier;
    }
    std::string Identifier::to_string()
    {
        return std::format("Identifier[{}] ({:p})", this->identifier, static_cast<void *>(this));
    }

    llvm::Value *Identifier::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

        auto decl = ast_get_vardecl_by_name(this, this->identifier);

        return builder->CreateLoad(decl->ir->getAllocatedType(), decl->ir, this->identifier);
    }

    VarDeclStmt *Identifier::get_var_decl()
    {
        return ast_get_vardecl_by_name(this, this->identifier);
    }
    Type *get_function_decl()
    {
        throw std::runtime_error("not implemented");
    }

    void Identifier::on_after_attach()
    {
        Expression::on_after_attach();
        // TODO
    }

    Type *Identifier::get_type()
    {
        return this->resolve()->get_type();
    }

    Node *Identifier::resolve()
    {
        if (this->identifier == nullptr || strlen(this->identifier) == 0)
        {
            throw std::runtime_error("Cannot retrieve type. Call type_inference first.");
        }
        auto block = ast_get_block(this);
        return block->lookup(this->identifier);
    }

    LOGIA_API Identifier *ast_create_identifier(LOGIA_CLONE const char *name)
    {
        LOGIA_ASSERT(name);

        return new Identifier(nullptr, strdup(name));
    }

    //
    // StructInitializer
    //

    StructInitializer::StructInitializer(antlr4::ParserRuleContext *rule) : Expression(rule, ast_types::NONE) {}
    
    void StructInitializer::set_type(Type* type) {
        this->is_typed = true;
        this->unshift_child(type);
    }

    void StructInitializer::add_named_property(TypeDef *locator, Expression *value)
    {
        this->push_child(locator);
        this->push_child(value);

        ++this->length;
    }
    void StructInitializer::add_positional_property(Expression *value)
    {
        this->push_child(new NoOp());
        this->push_child(value);

        ++this->length;
    }
    bool StructInitializer::pre_type_inference()
    {
        // if every value is constant -> we are constant!
        int count = 0;
        this->foreach_child<ConstExpression>([&count](auto x)
                                             { ++count; });
        this->is_constant = count == this->length;
        return true;
    }

    llvm::Value* StructInitializer::codegen(logia::Backend* codegen, llvm::IRBuilder<>* builder)
    {
        return nullptr;
    }

    Type* StructInitializer::get_type()
    {
        if (is_typed) {
            // too soon ?!
        }
        return this->get_child<Type>(0);
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
        case PrefixUnaryOperator::DEREFERENCE:
            return "logia_intrinsics_deref";
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

    char ast_binary_operator_to_string_buffer[256];
    const char *ast_binary_operator_to_string(BinaryOperator op, Type *left, Type *right)
    {
        switch (op)
        {
        case BinaryOperator::ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::ADD_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::SUB_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::DIV_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::MUL_ASSIGN:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_assign_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::ADD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_add_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::SUB:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_sub_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::MUL:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mul_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::DIV:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_div_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::MOD:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_mod_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_EQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_eq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_NEQ:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_neq_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_LT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_GT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gt_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_LTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_lte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGIAL_GTE:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_gte_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGICAL_AND:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_and_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::LOGICAL_OR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_or_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::BITWISE_XOR:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_xor_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::BITWISE_LEFT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_shl_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        case BinaryOperator::BITWISE_RIGHT_SHIFT:
            sprintf(ast_binary_operator_to_string_buffer, "logia_intrinsics_bin_shr_%s_%s", left->get_repr().c_str(), right->get_repr().c_str());
            return ast_binary_operator_to_string_buffer;
        default:
            throw std::runtime_error("Unknown binary operator");
        }
    }
}