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
        auto arguments = this->get_arguments();
        return std::string("CallExpression: ") + this->get_locator()->to_string() + "(" + std::string(itoa(arguments.size(), buffer, 10)) + " args)";
    }

    llvm::Value *CallExpression::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;

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

    BinaryExpression::BinaryExpression(antlr4::ParserRuleContext *rule, BinaryOperator op, Expression *left, Expression *right) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_binary_operator_to_string(op))));
        this->push_child(left);
        this->push_child(right);
    }

    // TODO create

    //
    // PrefixUnaryExpression
    //
    std::string PrefixUnaryExpression::to_string()
    {
        return std::string("PrefixUnaryExpression: ") + ast_prefix_unary_operator_to_string(this->op) + "(" + this->get_operand()->to_string() + ")";
    }

    PrefixUnaryExpression::PrefixUnaryExpression(antlr4::ParserRuleContext *rule, PrefixUnaryOperator op, Expression *operand) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_prefix_unary_operator_to_string(op))));
        this->push_child(operand);
    }

    // TODO create

    //
    // PostfixUnaryExpression
    //

    std::string PostfixUnaryExpression::to_string()
    {
        return std::string("PostfixUnaryExpression: ") + ast_postfix_unary_operator_to_string(this->op) + "(" + this->get_operand()->to_string() + ")";
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, PostfixUnaryOperator op, Expression *operand) : CallExpression(rule, nullptr, {})
    {
        this->op = op;
        this->push_child(ast_create_identifier(this, strdup(ast_postfix_unary_operator_to_string(op))));
        this->push_child(operand);
    }

    // TODO create

    //
    // Identifier
    //
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

    LOGIA_API Identifier *ast_create_identifier(Node *current, char *name)
    {
        LOGIA_ASSERT(current);
        LOGIA_ASSERT(name);

        auto parentBody = (Block *)ast_find_closest_parent(current, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

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