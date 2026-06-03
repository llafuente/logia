#include "logia/ast/memberaccessexpr.h"

#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/struct.h"

namespace logia::AST
{
    //
    // MemberAccessExpression
    //
    MemberAccessExpression::MemberAccessExpression(antlr4::ParserRuleContext *rule, Node *left, Identifier *right) : Expression(rule)
    {
        this->push_child(left);
        this->push_child(right);
        right->skip_type_inference = true;
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
        return this->type == nullptr ? nullptr : this->type;
    }
    void MemberAccessExpression::_set_type(Type *ty)
    {
        this->type = ty;
    }
    Node *MemberAccessExpression::resolve()
    {
        this->pre_type_inference();
        return this->get_type();
    }

    void MemberAccessExpression::_pre_type_inference()
    {
        auto left = this->get_left();
        left->pre_type_inference();
        auto left_ty = left->get_type();

        if (!left_ty->is<Struct>())
        {
            throw_compiler_error("TODO! Only structs can be resolved atm.");
        }

        auto left_ty_stuct = left_ty->as<Struct>();
        auto right = this->get_right();
        auto ty = left_ty_stuct->get_field_type(right)->get_final_type();
        right->set_type(ty);
        this->set_type(ty);
        Node::_pre_type_inference();
    }

    std::string MemberAccessExpression::to_string()
    {
        return std::format("MemberAccessExpression {}", Node::to_string());
    }

    llvm::Value *MemberAccessExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string_tree());
        // TODO handle left side to be a pointer to struct or struct itself, for now we assume it's always a pointer
        auto left = this->get_left();
        auto left_type = left->get_type();
        auto left_value = left->codegen(backend);

        if (!left_type->is<Struct>())
        {
            LOG_ERR("{}", left->to_string_tree());
            throw_semantic_error(left, "Expected left to be a struct");
        }
        auto struct_ty = left_type->as<Struct>();

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

        // auto property_ty = (llvm::Type *)field->get_final_type()->codegen(backend);
        // left_value = llvm_load_if_required(left_value, backend);
        auto gep = backend->builder->CreateStructGEP(struct_ty->ir_type, left_value, field->index);
        this->cg_value = gep;

        return Expression::post_codegen(backend);
    }
} // namespace logia
