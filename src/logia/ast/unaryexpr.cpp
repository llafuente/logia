#pragma once

#include "logia/ast/unaryexpr.h"
#include "logia/ast/identifier.h"

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
            // this->set_type(this->first_parent<Scope>()->lookup<Type>("ptr"));
            this->set_type(operand_ty);

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
    }

    llvm::Value *UnaryExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        auto operand = this->get_operand();
        auto operand_value = this->get_operand()->codegen(backend);
        auto operand_ty = operand_value->getType();

        switch (this->op)
        {
        case Operators::PREFIX_DEREFERENCE:
        {
            // return builder->CreateIntToPtr(operandValue, llvm::PointerType::get(codegen->context, 0));
            // return builder->CreateLoad(llvm::PointerType::get(codegen->context, 0), operandValue);
            // return builder->CreateLoad(operandType->getPointerTo(), operandValue, false);
            auto ptr = this->cg_value = backend->builder->CreateAlloca(operand_ty->getPointerTo(), nullptr, "deref");
            backend->set_debug_loc((llvm::Instruction *)ptr, this->rule);

            auto store = backend->builder->CreateStore(operand_value, ptr);
            backend->set_debug_loc((llvm::Instruction *)store, this->rule);

            // this->cg_value = backend->builder->CreateLoad(operandType->getPointerTo(), ptr);
            return Expression::post_codegen(backend);
        }
        default:
            this->cg_value = this->call_expr->codegen(backend);
            return Expression::post_codegen(backend);
        }
    }

    LOGIA_API LOGIA_LEND UnaryExpression *ast_create_ref(Expression *operand)
    {
        UnaryExpression *expr = new UnaryExpression(nullptr, Operators::PREFIX_DEREFERENCE, operand);
        return expr;
    }

    LOGIA_API LOGIA_LEND UnaryExpression *ast_create_preunary_expr(Operators op, Expression *operand)
    {
        return new UnaryExpression(nullptr, op, operand);
    }
}