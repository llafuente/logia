#include "logia/ast/binaryexpr.h"

#include "logia/log.h"
#include "logia/type_system.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/operators.h"
#include "logia/ast/unaryexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/cast.h"
#include "logia/ast/llvm.h"
#include "logia/ast/program.h"

#include <format>

#include "llvm/IR/Instructions.h"

namespace logia::AST
{
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
            this->push_child(new UnaryExpression(this->rule, Operators::PREFIX_DEREFERENCE, left));
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
    void BinaryExpression::_set_type(Type *ty)
    {
        this->type = ty;
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
                throw_semantic_error(this, "LGER032 lhs cannot be a constant expression");
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
                auto derr = err.unwrap_error();
                // pass left type to right type if it's ConstExpression
                if (derr.contains(type_system::type_compatibility::EXPLICIT_CAST) && right->is<ConstExpression>())
                {
                    right->set_type(left_ty);
                    return Expression::_pre_type_inference();
                }
                throw_semantic_error(this, err.message);
            }
            auto result = err.unwrap_success();
            if (result.contains(type_system::type_compatibility::AUTOCAST_CAST))
            {
                this->replace(right, new Cast(right->rule, right, left_ty));
                type_inference_node(this->first_parent<Program>(), this->get_right());
                LOG(DBG, "\n\n\n\n\n{}", this->to_string_tree());
                return Expression::_pre_type_inference();
            }
            if (((uint32_t)result & (uint32_t)type_system::type_compatibility::LAYOUT_COMPATIBLE) != 0 || ((uint32_t)result & (uint32_t)type_system::type_compatibility::YES) != 0)
            {
                right->set_type(left_ty);
                return Expression::_pre_type_inference();
            }
            throw_compiler_error("unreable");
        }
        else if (is_logical_operator(this->op))
        {
            // this->set_type(this->look)
        }

        Expression::_pre_type_inference();
    }

    void BinaryExpression::_post_type_inference()
    {
        auto left = this->get_left();
        auto left_ty = left->get_final_type();
        if (left_ty->is<InferType>())
        {
            LOG_ERR("{}", this->to_string_tree());
            throw_compiler_error("Unexpected left side infer type");
        }
        auto right = this->get_right();
        auto right_ty = right->get_final_type();
        if (right_ty->is<InferType>())
        {
            LOG_ERR("{}", this->to_string_tree());
            throw_compiler_error("Unexpected right side infer type");
        }
        switch (op)
        {
        case Operators::BINARY_ASSIGN:
        {
            this->set_type(left_ty);
        }
        break;
        default:
            auto locator = new Identifier(this->rule, ast_operator_to_function_name(op));

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
        // auto left_ty = left->get_final_type();
        auto left_value = left->codegen(backend);
        auto right = this->get_right();
        // auto right_ty = right->get_final_type();
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

    //
    // constant expression
    //

    maybe_semantic_error BinaryExpression::can_execute()
    {
        switch (op)
        {
        case Operators::BINARY_ADD:
        case Operators::BINARY_SUB:
        case Operators::BINARY_MUL:
        case Operators::BINARY_DIV:
        case Operators::BINARY_MOD:
        case Operators::BINARY_COMPARISON_EQ:
        case Operators::BINARY_COMPARISON_NEQ:
        case Operators::BINARY_COMPARISON_LT:
        case Operators::BINARY_COMPARISON_GT:
        case Operators::BINARY_COMPARISON_LTE:
        case Operators::BINARY_COMPARISON_GTE:
        case Operators::BINARY_BITWISE_AND:
        case Operators::BINARY_BITWISE_OR:
        case Operators::BINARY_BITWISE_XOR:
        case Operators::BINARY_BITWISE_LEFT_SHIFT:
        case Operators::BINARY_BITWISE_RIGHT_SHIFT:
            break;
        default:
            return make_semantic_error(LGERR_CONSTEX005, this);
        }

        auto left = this->get_left();
        if (left->can_execute().is_error())
        {
            return make_semantic_error(LGERR_CONSTEX001a, this);
        }

        auto right = this->get_right();
        if (right->can_execute().is_error())
        {
            return make_semantic_error(LGERR_CONSTEX001b, this);
        }

        return make_semantic_success(true);
    }

    ConstExpression *BinaryExpression::execute()
    {
        auto left = this->get_left()->execute();
        auto right = this->get_right()->execute();

        switch (op)
        {
        case Operators::BINARY_ADD:
            return left->operator+(right);
            /*
                    case Operators::BINARY_SUB:
                        return left - right;
                    case Operators::BINARY_MUL:
                        return left * right;
                    case Operators::BINARY_DIV:
                        return left / right;
                    case Operators::BINARY_MOD:
                        return left % right;
                    case Operators::BINARY_COMPARISON_EQ:
                        return left == right;
                    case Operators::BINARY_COMPARISON_NEQ:
                        return left != right;
                    case Operators::BINARY_COMPARISON_LT:
                        return left < right;
                    case Operators::BINARY_COMPARISON_GT:
                        return left > right;
                    case Operators::BINARY_COMPARISON_LTE:
                        return left <= right;
                    case Operators::BINARY_COMPARISON_GTE:
                        return left >= right;
                    case Operators::BINARY_BITWISE_AND:
                        return left & right;
                    case Operators::BINARY_BITWISE_OR:
                        return left | right;
                    case Operators::BINARY_BITWISE_XOR:
                        return left ^ right;
                    case Operators::BINARY_BITWISE_LEFT_SHIFT:
                        return left << right;
                    case Operators::BINARY_BITWISE_RIGHT_SHIFT:
                        return left >> right;
                        break;
            */
        default:
            throw_semantic_error(this, LGERR_CONSTEX005);
        }
    }

} // namespace logia
