#include "logia/type_system.h"

#include "logia/ast/struct.h"
#include "logia/ast/function.h"
#include "logia/ast/semantic_error.h"

namespace logia::type_system
{
    using namespace logia::AST;

    type_compatibility type_compatibility::NO = type_compatibility(0x000);
    type_compatibility type_compatibility::YES = type_compatibility(0x001);
    type_compatibility type_compatibility::EXPLICIT_CAST = type_compatibility(0x010);
    type_compatibility type_compatibility::AUTOCAST_CAST = type_compatibility(0x020);
    type_compatibility type_compatibility::AUTOCAST_REF = type_compatibility(0x040);
    type_compatibility type_compatibility::AUTOCAST_DEREF = type_compatibility(0x080);
    type_compatibility type_compatibility::LAYOUT_COMPATIBLE = type_compatibility(0x100);
    type_compatibility type_compatibility::CODE_COMPATIBLE = type_compatibility(0x200);

    constexpr auto make_error = logia::utils::make_error<type_compatibility, type_compatibility>;
    constexpr auto make_success = logia::utils::make_success<type_compatibility, type_compatibility>;
    constexpr auto make_chained_error = logia::utils::make_chained_error<type_compatibility, type_compatibility>;

    type_compatibility_result type_is_compatible(Type *lhs, Type *rhs)
    {
        // if rhs is infer, it's compatible with everything
        if (rhs->is<InferType>() || rhs == nullptr)
        {
            return make_success(type_compatibility::YES);
        }
        // if both has the same primitive they may be compatible
        if (lhs->primitive == rhs->primitive)
        {
            // NOTE: remember to check here from the deepest in the hierarchy to the parent!
            if (lhs->is<Void>())
            {
                return make_success(type_compatibility::YES | type_compatibility::LAYOUT_COMPATIBLE);
            }

            if (lhs->is<Ref>())
            {
                // NOTE: while pointee can be autocasted the pointer itself DONT!
                auto lhs_ptr = lhs->as<Ref>();
                auto rhs_ptr = rhs->as<Ref>();
                auto result = type_is_compatible(lhs_ptr->get_pointee()->get_final_type(), rhs_ptr->get_pointee()->get_final_type());
                if (result.is_error())
                {
                    return make_chained_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), result);
                }
                // also an error FULL COMPAT!
                if (result.unwrap_success().contains(type_compatibility::AUTOCAST_CAST))
                {
                    return make_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
                }
                return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
            }

            // pointer are opaque and always compatible, this is the way to have problems, we know...
            if (lhs->is<Pointer>())
            {
                return make_success(type_compatibility::YES);
            }

            // integers, check signedness and bits
            if (lhs->is<Integer>())
            {
                auto lhs_int = lhs->as<Integer>();
                auto rhs_int = rhs->as<Integer>();

                // rhs is compatible if has more precision!
                if (lhs_int->bits == rhs_int->bits)
                {
                    if (lhs_int->is_signed != rhs_int->is_signed)
                    {
                        return make_error(std::format(LGERR_TS_INT001, lhs->get_repr(), rhs->get_repr()), (type_compatibility)((uint32_t)type_compatibility::EXPLICIT_CAST | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
                    }
                    return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
                }
                else if (lhs_int->bits < rhs_int->bits)
                {
                    return make_error(std::format(LGERR_TS_INT002, lhs->get_repr(), rhs->get_repr()), type_compatibility::EXPLICIT_CAST);
                }

                return make_success(type_compatibility::AUTOCAST_CAST);
            }
            // floats, check bits
            if (lhs->is<Float>())
            {
                auto lhs_flt = lhs->as<Float>();
                auto rhs_flt = rhs->as<Float>();

                // rhs is compatible if has more precision!
                if (lhs_flt->bits == rhs_flt->bits)
                {
                    return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
                }
                else if (lhs_flt->bits < rhs_flt->bits)
                {
                    return make_error(std::format(LGERR_TS_FLT001, lhs->get_repr(), rhs->get_repr()), type_compatibility::EXPLICIT_CAST);
                }

                return make_success(type_compatibility::AUTOCAST_CAST);
            }

            // structs, check field by field shall have 100% compatible type (no autocast, no explicit cast)
            // then check code compatibility, layout + aliases + getter + setters
            // REVIEW method compatibility is required ?
            if (lhs->is<Struct>())
            {
                auto lhs_st = lhs->as<Struct>();
                auto rhs_st = rhs->as<Struct>();

                if (lhs_st->field_count != rhs_st->field_count)
                {
                    return make_error(std::format(LGERR_TS_ST001, lhs_st->get_repr(), rhs_st->get_repr()), type_compatibility::EXPLICIT_CAST);
                }

                // TODO REVIEW allow struct casting ?
                // both structs could be compatible at "layout" level
                // auto lhs_types = std::vector<Type*>(lhs_st->field_count);
                for (int i = 0; i < lhs_st->field_count; ++i)
                {
                    // test one by one, DO NOT ALLOW BIANRY COMPATIBILITY: i32, i32 is not i64! <-- no UNION!
                    auto left = lhs_st->get_field_by_index(i)->get_final_type();
                    auto right = rhs_st->get_field_by_index(i)->get_final_type();

                    auto x = type_is_compatible(left, right);
                    if (x.is_error())
                    {
                        return make_chained_error(std::format(LGERR_TS_ST002, lhs_st->get_repr(), rhs_st->get_repr(), i), x);
                    }

                    // TODO viable? should be the above right ?
                    auto data = x.unwrap_success();
                    if (((uint32_t)data & (uint32_t)type_compatibility::EXPLICIT_CAST) == (uint32_t)type_compatibility::EXPLICIT_CAST)
                    {
                        return make_error(std::format("LGERR_TS003 Incompatible types '{}' -> '{}'. Field at position '{}' requires an explicit cast.", left->get_repr(), right->get_repr(), i), type_compatibility::EXPLICIT_CAST);
                    }
                }
                /*
                                // check if it's code-compatible
                                // atp we know we are layout compatible
                                // we need to check field/alias should match something in the other side with same name.
                                StructField * sf;
                                StructAlias * sa;
                                for (auto left : lhs_st->children) {
                                    if (left->try_cast<StructField>(&sf)) {
                                        ->get_name()

                                        rhs_st->get_field_by_name()

                                    }

                                }
                */

                return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE | (uint32_t)type_compatibility::CODE_COMPATIBLE));
            }

            if (lhs->is<Function>())
            {
                // NOTE: this is not multiple dispatch compatibility, function-function has no posibility of casting due
                // function pointer is a runtime feature, so both "types" shall be exactly the same!

                auto lhs_fn = lhs->as<Function>();
                auto rhs_fn = rhs->as<Function>();

                if (lhs_fn->get_parameter_count() != rhs_fn->get_parameter_count())
                {
                    return make_error(std::format(LGERR_TS_FN001, lhs_fn->get_repr(), rhs_fn->get_repr()), type_compatibility::EXPLICIT_CAST);
                }

                {
                    auto l = lhs_fn->get_return_type()->get_final_type();
                    auto r = rhs_fn->get_return_type()->get_final_type();
                    // now from return type to each arguments, all shall be compatible 100%
                    auto result = type_is_compatible(l, r);
                    if (result.is_error())
                    {
                        return make_chained_error(std::format(LGERR_TS_FN002, lhs_fn->get_repr(), rhs_fn->get_repr()), result);
                    }
                    // also an error FULL COMPAT!
                    if (!result.unwrap_success().contains(type_compatibility::YES))
                    {
                        return make_error(std::format(LGERR_TS_FN002, lhs_fn->get_repr(), rhs_fn->get_repr()), type_compatibility::NO);
                    }
                }

                for (auto i = 0; i < lhs_fn->get_parameter_count(); ++i)
                {
                    auto l = lhs_fn->get_parameter(i)->get_final_type();
                    auto r = rhs_fn->get_parameter(i)->get_final_type();

                    auto result = type_is_compatible(l, r);
                    if (result.is_error())
                    {
                        return make_chained_error(std::format(LGERR_TS_FN003, lhs_fn->get_repr(), rhs_fn->get_repr(), i + 1), result);
                    }
                    // also an error FULL COMPAT!
                    if (!result.unwrap_success().contains(type_compatibility::YES))
                    {
                        return make_error(std::format(LGERR_TS_FN003, lhs_fn->get_repr(), rhs_fn->get_repr(), i + 1), type_compatibility::NO);
                    }
                }

                return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE | (uint32_t)type_compatibility::CODE_COMPATIBLE));
            }

            return make_error(std::format(LGERR_TS000, lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
        }

        // can be an AUTOCAST_REF ?
        if (rhs->is<Ref>())
        {
            // NOTE: while pointee can be autocasted the pointer itself DONT!
            auto rhs_ptr = rhs->as<Ref>();

            auto result = type_is_compatible(lhs, rhs_ptr->get_pointee()->get_final_type());
            if (result.is_error())
            {
                return make_chained_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), result);
            }
            // also an error FULL COMPAT!
            if (result.unwrap_success().contains(type_compatibility::AUTOCAST_CAST))
            {
                return make_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
            }
            return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::AUTOCAST_REF));
        }
        // can be an AUTOCAST_DEREF ?
        if (lhs->is<Ref>())
        {
            // NOTE: while pointee can be autocasted the pointer itself DONT!
            auto lhs_ptr = lhs->as<Ref>();

            auto result = type_is_compatible(lhs_ptr->get_pointee()->get_final_type(), rhs);
            if (result.is_error())
            {
                return make_chained_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), result);
            }
            // also an error FULL COMPAT!
            if (result.unwrap_success().contains(type_compatibility::AUTOCAST_CAST))
            {
                return make_error(std::format(LGERR_TS_PTR001, lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
            }
            return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::AUTOCAST_DEREF));
        }

        // REVIEW
        // if they don't have the same primitive, they are incompatible
        // even it's not 100% real, we will be extra safe at the start
        return make_error(std::format(LGERR_TS001, lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
    }

}