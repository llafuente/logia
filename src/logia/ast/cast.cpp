#include "logia/ast/cast.h"

#include "logia/backend.h"
#include "logia/ast/expr.h"
#include "logia/ast/type.h"
#include "logia/ast/identifier.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/semantic_error.h"
#include "logia/ast/llvm.h"

namespace logia::AST
{
    Cast::Cast(
        location loc,
        Expression *expr,
        Type *to) : Expression(loc)
    {
        this->is_typed = true;
        this->push_child(expr);
        this->push_child(to);
    }

    Type *Cast::get_source_type()
    {
        return this->get_expr()->get_type();
    }

    Expression *Cast::get_expr()
    {
        return this->get_child<Expression>(0);
    }

    Type *Cast::get_target_type()
    {
        return this->get_child<Type>(1)->get_final_type();
    }

    std::string Cast::to_string()
    {
        return std::format("Cast{}", Expression::to_string());
    }

    Type *Cast::get_type()
    {
        return this->get_target_type();
    }

    void Cast::_set_type(Type *type)
    {
        this->children[1] = type;
    }

    void Cast::_post_type_inference()
    {
        Expression *expr = this->get_expr();
        Type *from_type = this->get_source_type();
        Type *to_type = this->get_target_type();

        if (!from_type)
        {
            return; // wait!
        }

        if (from_type->primitive == Primitives::VOID_TY)
        {
            // void to any type is not allowed
            throw_semantic_error(expr, "LGERR_CAST001 Cannot cast void to any type");
        }
        if (to_type->primitive == Primitives::VOID_TY)
        {
            // any type to void is not allowed
            throw_semantic_error(to_type, "LGERR_CAST001 Cannot cast any type to void");
        }
        // integer(s) to float(s) ?
        if ((from_type->primitive == Primitives::INTEGER_TY || from_type->primitive == Primitives::FLOATING_POINT_TY) &&
            (to_type->primitive == Primitives::INTEGER_TY || to_type->primitive == Primitives::FLOATING_POINT_TY))
        {
            // we cast in LLVM
        }
        else
        {
            // we cast using a function
            // this->callexpr = new CallExpression(this->loc, new Identifier(this->loc, "cast"), {this->get_expr()});
            throw_compiler_error("todo");
            // this need to be resolved with arguments
        }

        Expression::_post_type_inference();
    }

    llvm::Value *Cast::post_codegen(logia::Backend *backend)
    {
        Type *from_type = this->get_source_type();
        Type *to_type = this->get_target_type();

        auto value = llvm_load_if_required(this->get_expr()->post_codegen(backend), backend);

        // integer to integer
        if (from_type->primitive == Primitives::INTEGER_TY && to_type->primitive == Primitives::INTEGER_TY)
        {
            auto from_int_ty = from_type->as<Integer>();
            auto to_int_ty = to_type->as<Integer>();

            // both signed
            if (from_int_ty->is_signed && from_int_ty->is_signed == to_int_ty->is_signed)
            {
                if (from_int_ty->bits < to_int_ty->bits)
                {
                    // iM (M < N) -> iN. i32 to i64. upcast integer with sign
                    this->cg_value = backend->builder->CreateSExt(value, to_int_ty->ir_type);
                }
                else if (from_int_ty->bits > to_int_ty->bits)
                {
                    // iM (M > N) -> iN. i64 to i32. downcast integer with sign
                    this->cg_value = backend->builder->CreateTrunc(value, to_int_ty->ir_type);
                }
                else
                {
                    // i32 to i32
                    // this->cg_value = builder->CreateBitCast(value, to_int_ty->ir_type);
                    this->cg_value = value;
                }
                return Expression::post_codegen(backend);
            }
            // both unsigned
            if (!from_int_ty->is_signed && from_int_ty->is_signed == to_int_ty->is_signed)
            {
                if (from_int_ty->bits < to_int_ty->bits)
                {
                    // uM (M < N) -> uN. u32 to u64. upcast integer without sign
                    this->cg_value = backend->builder->CreateZExt(value, to_int_ty->ir_type);
                }
                else if (from_int_ty->bits > to_int_ty->bits)
                {
                    // uM (M > N) -> uN. u64 to u32. downcast integer without sign
                    this->cg_value = backend->builder->CreateTrunc(value, to_int_ty->ir_type);
                }
                else
                {
                    // same size, one signed one unsigned, just return
                    // this->cg_value = builder->CreateBitCast(value, to_int_ty->ir_type);
                    this->cg_value = value;
                }
                return Expression::post_codegen(backend);
            }

            // signed to unsigned
            if (from_int_ty->is_signed && !to_int_ty->is_signed)
            {
                if (from_int_ty->bits < to_int_ty->bits)
                {
                    // iM (M < N) -> uN. i32 to u64. upcast signed integer to unsigned
                    this->cg_value = backend->builder->CreateZExt(value, to_int_ty->ir_type);
                }
                else if (from_int_ty->bits > to_int_ty->bits)
                {
                    // iM (M > N) -> uN. i64 to u32. downcast signed integer to unsigned
                    this->cg_value = backend->builder->CreateTrunc(value, to_int_ty->ir_type);
                }
                else
                {
                    // Reinterpret bits, no numeric conversion
                    this->cg_value = backend->builder->CreateBitCast(value, to_int_ty->ir_type);
                }
                return Expression::post_codegen(backend);
            }
            // unsigned to signed
            if (!from_int_ty->is_signed && to_int_ty->is_signed)
            {
                if (from_int_ty->bits < to_int_ty->bits)
                {
                    // uM (M < N) -> iN. u32 to i64. upcast unsigned integer to signed
                    this->cg_value = backend->builder->CreateZExt(value, to_int_ty->ir_type);
                }
                else if (from_int_ty->bits > to_int_ty->bits)
                {
                    // uM (M > N) -> iN. u64 to i32. downcast unsigned integer to signed
                    this->cg_value = backend->builder->CreateTrunc(value, to_int_ty->ir_type);
                }
                else
                {
                    // Reinterpret bits, no numeric conversion
                    this->cg_value = backend->builder->CreateBitCast(value, to_int_ty->ir_type);
                }
                return Expression::post_codegen(backend);
            }
            throw_compiler_error("unreachable code in cast integer to integer");
        }
        // float to float
        if (from_type->primitive == Primitives::FLOATING_POINT_TY && to_type->primitive == Primitives::FLOATING_POINT_TY)
        {
            auto from_flt_ty = from_type->as<Float>();
            auto to_flt_ty = to_type->as<Float>();

            if (from_flt_ty->bits < to_flt_ty->bits)
            {
                // fM (M < N) -> fN. f32 to f64. upcast float
                this->cg_value = backend->builder->CreateFPExt(value, to_type->ir_type);
            }
            else if (from_flt_ty->bits > to_flt_ty->bits)
            {
                // fM (M > N) -> fN. f64 to f32. downcast float
                this->cg_value = backend->builder->CreateFPTrunc(value, to_type->ir_type);
            }
            else
            {
                // same size, just return
                this->cg_value = value;
            }
            return Expression::post_codegen(backend);
        }

        // integer to float
        if (from_type->primitive == Primitives::INTEGER_TY && to_type->primitive == Primitives::FLOATING_POINT_TY)
        {
            auto from_int_ty = from_type->as<Integer>();
            auto to_flt_ty = to_type->as<Float>();

            // int to float
            if (from_int_ty->is_signed)
            {
                this->cg_value = backend->builder->CreateSIToFP(value, to_flt_ty->ir_type);
            }
            else
            {
                this->cg_value = backend->builder->CreateUIToFP(value, to_flt_ty->ir_type);
            }
            return Expression::post_codegen(backend);
        }

        // float to integer
        if (from_type->primitive == Primitives::FLOATING_POINT_TY && to_type->primitive == Primitives::INTEGER_TY)
        {
            // auto from_flt_ty = from_type->as<Float>();
            auto to_int_ty = to_type->as<Integer>();
            // float to int
            if (to_int_ty->is_signed)
            {
                this->cg_value = backend->builder->CreateFPToSI(value, to_int_ty->ir_type);
            }
            else
            {
                this->cg_value = backend->builder->CreateFPToUI(value, to_int_ty->ir_type);
            }
            return Expression::post_codegen(backend);
        }

        throw_compiler_error("to-do: cast non-implemented");
    }

    void Cast::on_after_attach() {}

    void Cast::validate() {}
}