#include "logia/ast/memberaccessexpr.h"

#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/struct.h"
#include "logia/ast/function.h"
#include "logia/ast/unaryexpr.h"
#include "logia/ast/llvm.h"

namespace logia::AST
{
    //
    // MemberAccessExpression
    //
    MemberAccessExpression::MemberAccessExpression(location loc, Expression *left, Expression *right) : Expression(loc)
    {
        this->push_child(left);
        this->push_child(right);

        // left recursive parser
        // MemberAccessExpression(x,y) -> x resolve, y !resolve
        // MemberAccessExpression(MemberAccessExpression(x,y), z) x resolve, y !resolve, z !resolve

        Identifier *ident;
        // if (this->parent_node->is<MemberAccessExpression>()) {
        if (left->try_cast<Identifier>(&ident))
        {
            ident->resolve = true;
            ident->resolve_unique = true; // TODO REVIEW atm we do not let to resolve multiple dispatch directly!
        }
        if (right->try_cast<Identifier>(&ident))
        {
            ident->resolve = false;
        }
    }
    Expression *MemberAccessExpression::get_left()
    {
        return this->get_child<Expression>(0);
    }
    Expression *MemberAccessExpression::get_right()
    {
        return this->get_child<Expression>(1);
    }
    Type *MemberAccessExpression::get_type()
    {
        return this->type == nullptr ? nullptr : this->type;
    }
    void MemberAccessExpression::_set_type(Type *ty)
    {
        this->type = ty;
    }

    void MemberAccessExpression::on_after_attach() {}

    void MemberAccessExpression::validate() {}

    void MemberAccessExpression::_pre_type_inference()
    {
        auto left = this->get_left();
        if (left->type_inference_pass_id != TYPE_INFERENCE_PRE)
        {
            return; // later...
        }
        auto left_ty = left->get_type();

        // autoderef ?
        Ref *left_ty_ref;
        if (left_ty->try_cast<Ref>(&left_ty_ref))
        {
            // we will auto reference!
            auto deref = new UnaryExpression(left->loc, Operators::PREFIX_DEREFERENCE, left);
            this->set_child(deref, 0);
            left_ty = left_ty_ref->get_pointee()->get_final_type();
        }

        Struct *left_ty_stuct;

        if (left_ty->try_cast<Struct>(&left_ty_stuct))
        {
            auto right = this->get_right()->as<Identifier>(); // TODO
            auto prop = left_ty_stuct->get_property(right->identifier);

            auto ty = prop->get_final_type();

            right->set_type(ty);
            this->set_type(ty);
            // TODO support multiple-dispatch

            return Node::_pre_type_inference();
        }

        throw_semantic_error(this, std::format(LGERR_MAEXPR001, left_ty->get_repr()));
    }

    std::string MemberAccessExpression::to_string()
    {
        return std::format("MemberAccessExpression {}", Node::to_string());
    }

    llvm::Value *MemberAccessExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string_tree());
        auto ty = this->get_type();
        Function *fn;
        if (ty->try_cast<Function>(&fn))
        {
            this->cg_value = fn->ir_func;

            return Expression::post_codegen(backend);
        }
        // TODO handle left side to be a pointer to struct or struct itself, for now we assume it's always a pointer
        auto left = this->get_left();
        auto left_type = left->get_final_type();
        auto left_value = left->post_codegen(backend);

        if (!left_type->is<Struct>())
        {
            LOG_ERR("{}", left->to_string_tree());
            throw_semantic_error(left, "Expected left to be a struct");
        }
        auto struct_ty = left_type->as<Struct>();
        struct_ty->post_codegen(backend);

        auto right = this->get_right();
        if (!right->is<Identifier>())
        {
            LOG_ERR("{}", left->to_string_tree());
            throw_semantic_error(left, "Expected right to be an identifier");
        }
        auto right_ident = right->as<Identifier>();

        auto field = struct_ty->get_field(right_ident->identifier);
        if (field == nullptr)
        {
            throw_semantic_error(left, std::format("struct '{}' do not contains a property with name '{}'", struct_ty->get_name(), right_ident->identifier));
        }

        auto gep = backend->builder->CreateStructGEP(struct_ty->ir_type, left_value, field->index);
        this->cg_value = gep;

        return Expression::post_codegen(backend);
    }
} // namespace logia
