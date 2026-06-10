#include "logia/ast/unaryexpr.h"

#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/type.h"

#include "llvm/IR/DerivedTypes.h"

namespace logia::AST
{
    //
    // UnaryExpression
    //
    std::string UnaryExpression::to_string()
    {
        // if (this->call_expr != nullptr) {}
        return std::format("UnaryExpression [{}] {}", ast_operator_to_function_name(op), Node::to_string());
    }

    UnaryExpression::UnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand) : Expression(rule)
    {
        this->op = op;
        switch (op)
        {
        case Operators::POSTFIX_DECREMENT:
        case Operators::POSTFIX_INCREMENT:
        case Operators::PREFIX_BITWISE_NOT:
        case Operators::PREFIX_DECREMENT:
        case Operators::PREFIX_INCREMENT:
        case Operators::PREFIX_NEGATION:
        case Operators::PREFIX_LOGICAL_NOT:
        case Operators::PREFIX_DEREFERENCE:
            break;
        default:
            throw_compiler_error(std::format("try to instance unary expression with and invalid operator {}", ast_operator_to_function_name(op)));
        }

        this->push_child(operand); // 0
    }

    Expression *UnaryExpression::get_operand()
    {
        return this->get_child<Expression>(0);
    }

    Type *UnaryExpression::get_type()
    {
        return this->type;
    }

    void UnaryExpression::_pre_type_inference()
    {
        auto operand_ty = this->get_operand()->get_final_type();
        if (operand_ty == nullptr)
        {
            return; // try again later!
        }

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        {
            this->set_type(operand_ty->get_pointer_to());

            break;
        }
        default:
        {
            /// replace with callexpr!
            auto locator = new Identifier(this->rule, ast_operator_to_function_name(op));
            auto operand = this->get_operand();

            this->call_expr = new CallExpression(this->rule, locator, {operand});
            // makes no sense but need to keep this node attached
            this->push_child(this->call_expr);

            this->call_expr->pre_type_inference();
            this->call_expr->post_type_inference();
            this->set_type(this->call_expr->get_type());
        }
        }
        return Expression::_pre_type_inference();
    }

    void UnaryExpression::_set_type(Type *ty)
    {
        this->type = ty;
        /*
        // foward the type depending on the operator
        switch (op)
        {
        case Operators::POSTFIX_DECREMENT:
        case Operators::POSTFIX_INCREMENT:
        case Operators::PREFIX_BITWISE_NOT:
        case Operators::PREFIX_DECREMENT:
        case Operators::PREFIX_INCREMENT:
        case Operators::PREFIX_NEGATION:
            this->get_operand()->set_type(type);
            break;
        case Operators::PREFIX_LOGICAL_NOT:
        case Operators::PREFIX_DEREFERENCE:
            break;
        default:
            throw_compiler_error("unreachable");
        }
            */
    }

    llvm::Value *UnaryExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        auto operand = this->get_operand();
        auto ir_value = operand->codegen(backend);
        auto ir_ty = ir_value->getType();

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        {
            // deprecated
            // auto ptr = this->cg_value = backend->builder->CreateAlloca(llvm::PointerType::get(ir_ty, 0), nullptr, "deref");
            auto ptr = this->cg_value = backend->builder->CreateAlloca(llvm::PointerType::get(backend->context, 0), 0, nullptr);
            backend->set_debug_loc((llvm::Instruction *)ptr, this->rule);

            auto store = backend->builder->CreateStore(ir_value, ptr);
            backend->set_debug_loc((llvm::Instruction *)store, this->rule);

            // this->cg_value = backend->builder->CreateLoad(operandType->getPointerTo(), ptr);
            return Expression::post_codegen(backend);
        }
        default:
            this->cg_value = this->call_expr->codegen(backend);
            return Expression::post_codegen(backend);
        }
    }

    //
    // constant expression
    //

    maybe_semantic_error UnaryExpression::can_execute()
    {
        switch (op)
        {
        // case Operators::PREFIX_DEREFERENCE:
        case Operators::PREFIX_NEGATION:
        case Operators::PREFIX_LOGICAL_NOT:
        case Operators::PREFIX_INCREMENT:
        case Operators::PREFIX_DECREMENT:
        case Operators::PREFIX_BITWISE_NOT:

        case Operators::POSTFIX_INCREMENT:
        case Operators::POSTFIX_DECREMENT:

            break;
        default:
            return make_semantic_error(LGERR_CONSTEX005, this);
        }

        auto operand = this->get_operand();
        if (operand->can_execute().is_error())
        {
            return make_semantic_error(LGERR_CONSTEX001a, this);
        }

        return make_semantic_success(true);
    }

    ConstExpression *UnaryExpression::execute()
    {
        /*
                auto operand = this->get_operand()->execute();

                switch (op)
                {
                case Operators::PREFIX_NEGATION:
                    return -operand;
                case Operators::PREFIX_LOGICAL_NOT:
                    return !operand;
                case Operators::PREFIX_INCREMENT:
                    return operand + new IntegerLiteral(nullptr, "1", nullptr);
                case Operators::PREFIX_DECREMENT:
                    return operand - new IntegerLiteral(nullptr, "1", nullptr);
                case Operators::PREFIX_BITWISE_NOT:
                    return ~operand;
                case Operators::POSTFIX_INCREMENT:
                    return operand + new IntegerLiteral(nullptr, "1", nullptr);
                case Operators::POSTFIX_DECREMENT:
                    return operand - new IntegerLiteral(nullptr, "1", nullptr);
                    break;
                default:
                    throw_semantic_error(this, LGERR_CONSTEX005);
                }
        */
        return nullptr;
    }
}