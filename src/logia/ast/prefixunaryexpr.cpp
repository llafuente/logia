#pragma once

#include "logia/ast/prefixunaryexpr.h"
#include "logia/ast/identifier.h"

namespace logia::AST
{
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
}