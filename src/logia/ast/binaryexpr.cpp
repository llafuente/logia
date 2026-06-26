#include "logia/ast/binaryexpr.h"

#include "utils.h"
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
#include "logia/ast/function.h"

#include <format>

#include "llvm/IR/Instructions.h"

namespace logia::AST
{
    uint64_t expr_logical_count = 0;
    //
    // BinaryExpression
    //

    std::string BinaryExpression::to_string()
    {
        return std::format("BinaryExpression [{}]{}", ast_operator_to_function_name(this->op), Node::to_string());
    }

    BinaryExpression::BinaryExpression(location loc, Expression *left, Operators op, Expression *right) : Expression(loc)
    {
        this->op = op;

        if (op == Operators::BINARY_ASSIGN)
        {
            this->push_child(left);
        }
        else if (is_assignament_operator(this->op))
        {
            this->push_child(new UnaryExpression(this->loc, Operators::PREFIX_DEREFERENCE, left));
        }
        else
        {
            this->push_child(left);
        }

        this->push_child(right);
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

    void BinaryExpression::__enforce_assignament_type(Type *left_ty, Type *right_ty)
    {
        auto right = this->get_right();
        auto err = type_system::type_is_compatible(left_ty, right_ty);
        if (err.is_error())
        {
            auto derr = err.unwrap_error();
            // pass left type to right expr if it's a ConstExpression
            if (derr.contains(type_system::type_compatibility::EXPLICIT_CAST) && right->is<ConstExpression>())
            {
                right->set_type(left_ty);
                return;
            }
            // display the casting error
            throw_semantic_error(this, err.message);
        }
        auto result = err.unwrap_success();
        // it's ok -> autocast ?
        if (result.contains(type_system::type_compatibility::AUTOCAST_CAST))
        {
            this->replace(right, new Cast(right->loc, right, left_ty));
            type_inference_node(this->first_parent<Program>(), this->get_right());
            LOG(DBG, "\n\n\n\n\n{}", this->to_string_tree());
            return;
        }
        // it's ok, fully compatible
        if (((uint32_t)result & (uint32_t)type_system::type_compatibility::LAYOUT_COMPATIBLE) != 0 || ((uint32_t)result & (uint32_t)type_system::type_compatibility::YES) != 0)
        {
            right->set_type(left_ty);
            return;
        }
        // hell!
        throw_compiler_error("unreable");
    }

    void BinaryExpression::_pre_type_inference()
    {
        auto left = this->get_left();
        // left->pre_type_inference();
        auto left_ty = left->get_final_type();

        auto right = this->get_right();
        auto right_ty = right->get_final_type();

        if (left_ty == nullptr)
        {
            LOG(DBG, "lhs is not ready {}", this->to_string_tree());
            return; // next time!
        }
        if (right_ty == nullptr)
        {
            LOG(DBG, "rhs is not ready {}", this->to_string_tree());
            return; // next time!
        }

        if (op == Operators::BINARY_ASSIGN)
        {
            // same/comptible types ?!
            if (left->is<ConstExpression>())
            {
                throw_semantic_error(this, LGERR_BINEXPR002);
            }

            this->__enforce_assignament_type(left_ty, right_ty);
            this->set_type(left_ty);
        }
        else if (is_assignament_operator(this->op))
        {
            if (left->is<ConstExpression>())
            {
                throw_semantic_error(this, LGERR_BINEXPR002);
            }

            // right_ty should be a ref!
            Ref *ref_left_ty;
            if (!left_ty->try_cast<Ref>(&ref_left_ty))
            {
                LOG_ERR("{}", this->to_string_tree());
                throw_semantic_error(right, std::format(LGERR_BINEXPR001, left_ty->get_repr()));
            }
            this->__enforce_assignament_type(ref_left_ty->get_pointee(), right_ty);
        }
        else if (is_logical_operator(this->op))
        {
            this->set_type(scope_lookup_first(this, "bool")->as<Type>());
        }

        switch (op)
        {
        case Operators::BINARY_ASSIGN:
        case Operators::BINARY_LOGICAL_AND:
        case Operators::BINARY_LOGICAL_OR:
            break;
        default:
            auto locator = new Identifier(this->loc, ast_operator_to_function_name(op));

            this->call_expr = new CallExpression(this->loc, locator, {left, right});
            LOG(DBG, "transform binaryexpr into function call: {}", (void *)this->call_expr);
            // makes no sense but need to keep this node attached
            this->push_child(this->call_expr);

            this->call_expr->type_inference(TYPE_INFERENCE_PRE);
            LOGIA_VERIFY(this->call_expr->type_inference_pass_id == TYPE_INFERENCE_PRE); // ensure it's done!
            LOGIA_VERIFY(this->call_expr->callee != nullptr);

            this->set_type(this->call_expr->get_type());
        }

        Expression::_pre_type_inference();
    }

    void BinaryExpression::_post_type_inference()
    {
        switch (this->op)
        {
        case Operators::BINARY_LOGICAL_AND:
        case Operators::BINARY_LOGICAL_OR:
        {
            Integer *int_ty;
            auto left = this->get_left();
            if (!left->get_final_type()->try_cast<Integer>(&int_ty))
            {
                throw_semantic_error(left, LGERR_BINEXPR003);
            }
            if (int_ty->bits != 1)
            {
                throw_semantic_error(left, LGERR_BINEXPR003);
            }

            auto right = this->get_right();
            if (!right->get_final_type()->try_cast<Integer>(&int_ty))
            {
                throw_semantic_error(right, LGERR_BINEXPR004);
            }
            if (int_ty->bits != 1)
            {
                throw_semantic_error(right, LGERR_BINEXPR004);
            }
            break;
        }
        }
        Expression::_post_type_inference();
    }
    llvm::Value *BinaryExpression::post_codegen(logia::Backend *backend)
    {
        auto left = this->get_left();
        auto right = this->get_right();

        switch (this->op)
        {
        case Operators::BINARY_ASSIGN:
        {
            // auto left_ty = left->get_final_type();
            auto left_value = left->post_codegen(backend);
            // auto right_ty = right->get_final_type();
            auto right_value = right->post_codegen(backend);

            right_value = llvm_load_if_required(right_value, backend);

            auto store = backend->builder->CreateStore(right_value, left_value, false);
            backend->set_debug_loc((llvm::Instruction *)store, this->loc);
            this->cg_value = left_value;

            return left_value;
        }
        case Operators::BINARY_LOGICAL_AND:
        {
            auto start_bb = backend->builder->GetInsertBlock();
            auto func = this->first_parent<Function>();

            const char *test_rhs_name = std::format("logia_test_rhs_{}", expr_logical_count).c_str();
            llvm::BasicBlock *test_rhs = llvm::BasicBlock::Create(backend->context, test_rhs_name, func->ir_func);
            const char *phi_bb_name = std::format("logia_phi_bb_{}", expr_logical_count).c_str();
            llvm::BasicBlock *phi_bb = llvm::BasicBlock::Create(backend->context, phi_bb_name, func->ir_func);
            ++expr_logical_count;

            auto left_value = left->post_codegen(backend);
            left_value = llvm_load_if_required(left_value, backend);
            backend->builder->CreateCondBr(left_value, test_rhs, phi_bb);
            backend->builder->SetInsertPoint(test_rhs);

            auto right_value = right->post_codegen(backend);
            right_value = llvm_load_if_required(right_value, backend);
            backend->builder->CreateBr(phi_bb);

            backend->builder->SetInsertPoint(phi_bb);
            auto i1 = scope_lookup_first(this, "bool")->as<Type>();
            auto phi = backend->builder->CreatePHI(i1->ir_type, 2);

            phi->addIncoming((new AST::IntegerLiteral({}, "0", i1))->post_codegen(backend), start_bb);
            phi->addIncoming(right_value, test_rhs);

            backend->set_debug_loc((llvm::Instruction *)phi, this->loc);
            this->cg_value = phi;

            return phi;
        }
        case Operators::BINARY_LOGICAL_OR:
        {
            auto start_bb = backend->builder->GetInsertBlock();
            auto func = this->first_parent<Function>();

            const char *test_rhs_name = std::format("logia_test_rhs_{}", expr_logical_count).c_str();
            llvm::BasicBlock *test_rhs = llvm::BasicBlock::Create(backend->context, test_rhs_name, func->ir_func);
            const char *phi_bb_name = std::format("logia_phi_bb_{}", expr_logical_count).c_str();
            llvm::BasicBlock *phi_bb = llvm::BasicBlock::Create(backend->context, phi_bb_name, func->ir_func);
            ++expr_logical_count;

            auto left_value = left->post_codegen(backend);
            left_value = llvm_load_if_required(left_value, backend);
            backend->builder->CreateCondBr(left_value, phi_bb, test_rhs);
            backend->builder->SetInsertPoint(test_rhs);

            auto right_value = right->post_codegen(backend);
            right_value = llvm_load_if_required(right_value, backend);
            backend->builder->CreateBr(phi_bb);

            backend->builder->SetInsertPoint(phi_bb);
            auto i1 = scope_lookup_first(this, "bool")->as<Type>();
            auto phi = backend->builder->CreatePHI(i1->ir_type, 2);

            phi->addIncoming((new AST::IntegerLiteral({}, "1", i1))->post_codegen(backend), start_bb);
            phi->addIncoming(right_value, test_rhs);

            backend->set_debug_loc((llvm::Instruction *)phi, this->loc);
            this->cg_value = phi;

            return phi;
        }
        }

        this->cg_value = this->call_expr->post_codegen(backend);

        return Expression::post_codegen(backend);
    }

    void BinaryExpression::on_after_attach() {}

    void BinaryExpression::validate() {}

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
