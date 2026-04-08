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
        return "Expression";
    }

    //
    // MemberAccessExpression
    //
    MemberAccessExpression::MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Node *right) : Expression(rule, ast_types::EXPRESSION)
    {
        this->push_child(left);
        this->push_child(right);
    }
    Expression *MemberAccessExpression::get_left()
    {
        return (Expression *)this->children[0];
    }
    Expression *MemberAccessExpression::get_right()
    {
        return (Expression *)this->children[1];
    }
    Type *MemberAccessExpression::get_type()
    {
        // TODO resolve!
        return nullptr;
    }

    //
    // CallExpression
    //
    CallExpression::CallExpression() : Expression(nullptr, ast_types::CALL_EXPRESSION)
    {
    }
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> arguments) : Expression(rule, ast_types::CALL_EXPRESSION)
    {
        this->push_child(locator);
        for (int i = 0; i < arguments.size(); ++i)
        {
            this->push_child(arguments[i]);
        }
    }
    Expression *CallExpression::get_locator()
    {
        return (Expression *)this->children[0];
    }
    std::vector<Node *> CallExpression::get_arguments()
    {
        return std::vector<Node *>(this->children.begin() + 1, this->children.end());
    }
    Type *CallExpression::get_type()
    {
        return this->get_locator()->get_type()->Function.return_type;
    }

    std::string CallExpression::to_string()
    {
        char buffer[36];
        auto locator = this->get_locator();
        if (!locator)
        {
            return std::string("CallExpression: incomplete");
        }

        auto arguments = this->get_arguments();
        return std::string("CallExpression: ") + locator->to_string() + "(" + std::string(itoa(arguments.size(), buffer, 10)) + " args)";
    }

    llvm::Value *CallExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

        if (this->children.size() == 0)
        {
            throw std::runtime_error("Incomplete CallExpression");
        }

        // Look up the name in the global module table.
        auto name = (StringLiteral *)this->get_locator();

        llvm::Function *CalleeF = codegen->module->getFunction(name->text);
        if (!CalleeF)
        {
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->text);
        }

        auto arguments = this->get_arguments();
        // If argument mismatch error.
        if (CalleeF->arg_size() != arguments.size())
        {
            throw std::exception("Incorrect # arguments passed");
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
        LOGIA_ASSERT((locator->type & ast_types::EXPRESSION) != 0);
        // TODO LOGIA_ASSERT_ALL(arguments, .type & ast_types::EXPRESSION != 0);

        auto callexpr = new CallExpression(nullptr, locator, arguments);

        return callexpr;
    }

    //
    // MemberExpression
    //
    std::string MemberAccessExpression::to_string()
    {
        return std::string("MemberAccessExpression: ") + this->get_left()->to_string() + "." + this->get_right()->to_string();
    }

    llvm::Value *MemberAccessExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
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

    // TODO create

    //
    // BinaryExpression
    //

    std::string BinaryExpression::to_string()
    {
        return std::string("BinaryExpression: ") + ast_binary_operator_to_string(this->op) + "(" + this->get_left()->to_string() + ", " + this->get_right()->to_string() + ")";
    }

    BinaryExpression::BinaryExpression(antlr4::ParserRuleContext *rule, Expression *left, BinaryOperator op, Expression *right) : CallExpression()
    {
        this->op = op;
        this->push_child(ast_create_identifier(strdup(ast_binary_operator_to_string(op))));
        switch (op)
        {
        case BinaryOperator::ASSIGN:
        case BinaryOperator::ADD_ASSIGN:
        case BinaryOperator::SUB_ASSIGN:
        case BinaryOperator::MUL_ASSIGN:
        case BinaryOperator::DIV_ASSIGN:
            this->push_child(ast_create_ref(left));
            break;
        default:
            this->push_child(left);
            break;
        }
        this->push_child(right);
    }

    Expression *BinaryExpression::get_left()
    {
        return (Expression *)this->children[0];
    }
    Expression *BinaryExpression::get_right()
    {
        return (Expression *)this->children[1];
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
        return std::string("PrefixUnaryExpression: ") + this->get_locator()->to_string() + "(" + this->get_operand()->to_string() + ")";
    }

    PrefixUnaryExpression::PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand) : CallExpression()
    {
        this->op = op;
        switch (this->op)
        {
        case PrefixUnaryOperator::DEREFERENCE:
            this->push_child(new NoOp());
            NODE_TYPE_ASSERT(operand, ast_types::IDENTIFIER);
            this->push_child(operand);
            break;
        default:
            this->push_child(ast_create_identifier(strdup(ast_prefix_unary_operator_to_string(op))));
            this->push_child(operand);
        }
    }

    Expression *PrefixUnaryExpression::get_operand()
    {
        return (Expression *)this->children[1];
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
            auto operandValue = ((Identifier *)this->get_operand())->get_var_decl()->ir;
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

    // TODO create

    //
    // PostfixUnaryExpression
    //

    std::string PostfixUnaryExpression::to_string()
    {
        return std::string("PostfixUnaryExpression: ") + ast_postfix_unary_operator_to_string(this->op) + "(" + this->get_operand()->to_string() + ")";
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand) : CallExpression()
    {
        this->op = op;
        this->push_child(ast_create_identifier(strdup(ast_postfix_unary_operator_to_string(op))));
        this->push_child(operand);
    }

    Expression *PostfixUnaryExpression::get_operand()
    {
        return (Expression *)this->children[1];
    }

    // TODO create

    //
    // Identifier
    //
    Identifier::Identifier(antlr4::ParserRuleContext *rule, char *identifier) : Expression(rule, ast_types::IDENTIFIER)
    {
        LOGIA_ASSERT(type);
        this->identifier = identifier;
    }
    std::string Identifier::to_string()
    {
        return std::string("Identifier: ") + this->identifier;
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
        // TODO
    }

    Type *Identifier::get_type()
    {
        // TODO resolve!
        return nullptr;
    }

    LOGIA_API Identifier *ast_create_identifier(char *name)
    {
        LOGIA_ASSERT(name);

        return new Identifier(nullptr, strdup(name));
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
        // case PrefixUnaryOperator::DEREFERENCE:
        //     return "logia_intrinsics_deref";
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
        case BinaryOperator::ASSIGN:
            return "logia_intrinsics_bin_assign";
        case BinaryOperator::ADD_ASSIGN:
            return "logia_intrinsics_bin_add_assign";
        case BinaryOperator::SUB_ASSIGN:
            return "logia_intrinsics_bin_sub_assign";
        case BinaryOperator::DIV_ASSIGN:
            return "logia_intrinsics_bin_div_assign";
        case BinaryOperator::MUL_ASSIGN:
            return "logia_intrinsics_bin_mul_assign";
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