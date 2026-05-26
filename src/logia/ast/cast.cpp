#include "logia/ast/expr.h"

#include "logia/ast/identifier.h"
#include "logia/ast/callexpr.h"

namespace logia::AST
{
    Cast::Cast(
        antlr4::ParserRuleContext *rule,
        Expression *expr,
        TypeDef *to) : Expression(rule)
    {
        this->push_child(expr);
        this->push_child(to);
    }

    Type *Cast::get_from_type()
    {
        return this->get_expr()->get_type();
    }

    Expression *Cast::get_expr()
    {
        return this->get_child<Expression>(0);
    }

    Type *Cast::get_to_type()
    {
        return this->get_child<TypeDef>(1)->get_final_type();
    }

    std::string Cast::to_string()
    {
        return std::format("Cast{}", Expression::to_string());
    }

    Type *Cast::get_type()
    {
        return this->get_to_type();
    }

    void Cast::_post_type_inference()
    {
        Expression *expr = this->get_expr();
        Type *from_type = this->get_from_type();
        Type *to_type = this->get_to_type();

        if (!from_type)
        {
            return; // wait!
        }

        if (from_type->primitive == Primitives::VOID_TY)
        {
            // void to any type is not allowed
            throw_compiler_error(expr, "cannot cast void to any type");
        }
        if (to_type->primitive == Primitives::VOID_TY)
        {
            // any type to void is not allowed
            throw_compiler_error(to_type, "cannot cast any type to void");
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
            this->callexpr = new CallExpression(this->rule, new Identifier(this->rule, "cast"), {this->get_expr()});
            // this need to be resolved with arguments
        }

        Expression::_post_type_inference();
    }

    void Cast::post_codegen(logia::Backend *backend)
    {
        Type *from_type = this->get_from_type();
        Type *to_type = this->get_to_type();

        llvm::Value *value = this->get_expr()->get_ir_value();

        // integer to integer
        if (from_type->primitive == Primitives::INTEGER_TY && to_type->primitive == Primitives::INTEGER_TY)
        {
            // both signed
            if (from_type->signed &&from_type->signed == to_type->signed)
            {
                if (from_type->bits < to_type->bits)
                {
                    // iM (M < N) -> iN. i32 to i64. upcast integer with sign
                    this->cg_value = builder.CreateSExt(value, to_type->ir_type);
                }
                else if (from_type->bits > to_type->bits)
                {
                    // iM (M > N) -> iN. i64 to i32. downcast integer with sign
                    this->cg_value = builder.CreateTrunc(value, to_type->ir_type);
                }
                else
                {
                    // i32 to i32
                    // this->cg_value = builder.CreateBitCast(value, to_type->ir_type);
                    this->cg_value = value;
                }
                return Node::post_type_inference(backend);
            }
            // both unsigned
            if (!from_type->signed &&from_type->signed == to_type->signed)
                if (from_type->bits < to_type->bits)
                {
                    // uM (M < N) -> uN. u32 to u64. upcast integer without sign
                    this->cg_value = builder.CreateZExt(value, to_type->ir_type);
                }
                else if (from_type->bits > to_type->bits)
                {
                    // uM (M > N) -> uN. u64 to u32. downcast integer without sign
                    this->cg_value = builder.CreateTrunc(value, to_type->ir_type);
                }
                else
                {
                    // same size, one signed one unsigned, just return
                    // this->cg_value = builder.CreateBitCast(value, to_type->ir_type);
                    this->cg_value = value;
                }
            return Node::post_type_inference(backend);
        }
        // signed to unsigned
        if (from_type->signed &&!to_type->signed)
        {
            if (from_type->bits < to_type->bits)
            {
                // iM (M < N) -> uN. i32 to u64. upcast signed integer to unsigned
                this->cg_value = builder.CreateZExt(value, to_type->ir_type);
            }
            else if (from_type->bits > to_type->bits)
            {
                // iM (M > N) -> uN. i64 to u32. downcast signed integer to unsigned
                this->cg_value = builder.CreateTrunc(value, to_type->ir_type);
            }
            else
            {
                // Reinterpret bits, no numeric conversion
                this->cg_value = builder.CreateBitCast(value, to_type->ir_type);
            }
            return Node::post_type_inference(backend);
        }
        // unsigned to signed
        if (!from_type->signed &&to_type->signed)
        {
            if (from_type->bits < to_type->bits)
            {
                // uM (M < N) -> iN. u32 to i64. upcast unsigned integer to signed
                this->cg_value = builder.CreateZExt(value, to_type->ir_type);
            }
            else if (from_type->bits > to_type->bits)
            {
                // uM (M > N) -> iN. u64 to i32. downcast unsigned integer to signed
                this->cg_value = builder.CreateTrunc(value, to_type->ir_type);
            }
            else
            {
                // Reinterpret bits, no numeric conversion
                this->cg_value = builder.CreateBitCast(value, to_type->ir_type);
            }
            return Node::post_type_inference(backend);
        }
        throw_compiler_error("unreachable code in cast integer to integer");
    }
    // float to float
    if (from_type->primitive == Primitives::FLOATING_POINT_TY && to_type->primitive == Primitives::FLOATING_POINT_TY)
    {
        if (from_type->bits < to_type->bits)
        {
            // fM (M < N) -> fN. f32 to f64. upcast float
            this->cg_value = builder.CreateFPExt(value, to_type->ir_type);
        }
        else if (from_type->bits > to_type->bits)
        {
            // fM (M > N) -> fN. f64 to f32. downcast float
            this->cg_value = builder.CreateFPTrunc(value, to_type->ir_type);
        }
        else
        {
            // same size, just return
            this->cg_value = value;
        }
        return Node::post_type_inference(backend);
    }

    // integer to float
    if (from_type->primitive == Primitives::INTEGER_TY && to_type->primitive == Primitives::FLOATING_POINT_TY)
    {
        // int to float
        if (from_type->signed)
        {
            this->cg_value = backend->builder.CreateSIToFP(value, llvm_to_type);
        }
        else
        {
            this->cg_value = backend->builder.CreateUIToFP(value, llvm_to_type);
        }
        return Node::post_type_inference(backend);
    }

    // float to integer
    if (from_type->primitive == Primitives::FLOATING_POINT_TY && to_type->primitive == Primitives::INTEGER_TY)
    {
        // float to int
        if (from_type->signed)
        {
            this->cg_value = backend->builder.CreateFPToSI(value, llvm_to_type);
        }
        else
        {
            this->cg_value = backend->builder.CreateFPToUI(value, llvm_to_type);
        }
        return Node::post_type_inference(backend);
    }

    return Node::post_type_inference(backend);
}