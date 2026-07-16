#include "logia/ast/unaryexpr.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/types/type.h"
#include "logia/ast/types/ref.h"

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

    UnaryExpression::UnaryExpression(location loc, Operators op, Expression *operand) : Expression(loc)
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
        case Operators::PREFIX_REFERENCE:
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

    void UnaryExpression::on_after_attach() {}

    void UnaryExpression::validate()
    {
        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        case Operators::PREFIX_REFERENCE:
        {
        }
        break;
        default:
        {
            /// replace with callexpr!
            auto locator = new Identifier(this->loc, ast_operator_to_function_name(op));
            auto operand = this->get_operand();

            this->call_expr = new CallExpression(this->loc, locator, {operand});
            LOG(DBG, "transform unaryexpr into function call: {}", (void *)this->call_expr);
            // makes no sense but need to keep this node attached
            this->push_child(this->call_expr);
        }
        }
    }

    bool UnaryExpression::type_inference(size_t pass_id)
    {
        switch (pass_id)
        {
        case TYPE_INFERENCE_PRE:
        {
            auto operand_ty = this->get_operand()->get_type_decl();
            if (operand_ty == nullptr)
            {
                return false;
            }

            switch (this->op)
            {
            case Operators::PREFIX_DEREFERENCE:
            {
                Ref *ty_ref;
                if (!operand_ty->try_cast(&ty_ref))
                {
                    throw_semantic_error(this, std::format("LGERR_UNARYEXPR001", operand_ty->get_repr()));
                }
                this->set_type(ty_ref->get_pointee());
            }
            break;
            case Operators::PREFIX_REFERENCE:
            {
                this->set_type(operand_ty->get_reference_to());
            }
            break;
            default:
            {
                if (this->call_expr != nullptr)
                {
                    auto tyd = this->call_expr->get_type_decl();
                    if (tyd == nullptr)
                    {
                        return false;
                    }
                    this->set_type(tyd);
                }
            }
            } // switch (this->op)
        }
        break; // TYPE_INFERENCE_PRE
        }
        return true;
    }

    void UnaryExpression::_on_set_type(TypeDecl *ty)
    {
        LOG(DBG, "{}", ty->get_repr());

        // foward the type depending on the operator
        switch (op)
        {
        case Operators::POSTFIX_DECREMENT:
        case Operators::POSTFIX_INCREMENT:
        case Operators::PREFIX_BITWISE_NOT:
        case Operators::PREFIX_DECREMENT:
        case Operators::PREFIX_INCREMENT:
        case Operators::PREFIX_NEGATION:
            this->get_operand()->set_type(ty);

        case Operators::PREFIX_DEREFERENCE:
            this->get_operand()->set_type(ty->get_reference_to());
            break;

        case Operators::PREFIX_LOGICAL_NOT:
        case Operators::PREFIX_REFERENCE:
            break;
        default:
            throw_compiler_error("unreachable");
        }
    }

    void UnaryExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        auto operand = this->get_operand();
        auto ir_value = operand->get_codegen_value(backend);
        auto ir_ty = ir_value->getType();

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        {
            // This reads the value from the pointer.
            this->set_codegen_value(backend, backend->builder->CreateLoad(ir_ty, ir_value));
            return Expression::post_codegen(backend);
        }
        break;

        case Operators::PREFIX_REFERENCE:
        {
            // deprecated
            // auto ptr = this->cg_value = backend->builder->CreateAlloca(llvm::PointerType::get(ir_ty, 0), nullptr, "deref");
            auto alloca = backend->builder->CreateAlloca(llvm::PointerType::get(backend->context, 0), 0, nullptr);
            this->set_codegen_value(backend, alloca);

            auto store = backend->builder->CreateStore(ir_value, alloca);
            backend->set_debug_loc((llvm::Instruction *)store, this->loc);

            // this->cg_value = backend->builder->CreateLoad(operandType->getPointerTo(), ptr);
            return Expression::post_codegen(backend);
        }
        default:
            this->set_codegen_value(backend, this->call_expr->get_codegen_value(backend));
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
        case Operators::PREFIX_DEREFERENCE:
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