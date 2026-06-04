#include "logia/type_system.h"

#include "logia/ast/struct.h"
#include "logia/ast/function.h"

namespace logia::type_system
{
    using namespace logia::AST;

    type_compatibility type_compatibility::NO = type_compatibility(0x000);
    type_compatibility type_compatibility::YES = type_compatibility(0x001);
    type_compatibility type_compatibility::EXPLICIT_CAST = type_compatibility(0x010);
    type_compatibility type_compatibility::AUTOCAST_CAST = type_compatibility(0x020);
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
            if (lhs->is<Void>())
            {
                return make_success(type_compatibility::YES | type_compatibility::LAYOUT_COMPATIBLE);
            }
            // TODO pointer should check pointee type
            if (lhs->is<Pointer>())
            {
                return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
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
                        return make_error(std::format("LGERR_TS001 Incompatible types '{}' -> '{}'. Explicit cast is required, conversion changes signedness.", lhs->get_repr(), rhs->get_repr()), (type_compatibility)((uint32_t)type_compatibility::EXPLICIT_CAST | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
                    }
                    return make_success((type_compatibility)((uint32_t)type_compatibility::YES | (uint32_t)type_compatibility::LAYOUT_COMPATIBLE));
                }
                else if (lhs_int->bits < rhs_int->bits)
                {
                    return make_error(std::format("LGERR_TS002 Incompatible types '{}' -> '{}'. Explicit cast is required, conversion loses integer precision.", lhs->get_repr(), rhs->get_repr()), type_compatibility::EXPLICIT_CAST);
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
                    return make_error(std::format("LGERR_TS003 Incompatible types '{}' -> '{}'. Explicit cast is required, conversion loses floating-point precision.", lhs->get_repr(), rhs->get_repr()), type_compatibility::EXPLICIT_CAST);
                }

                return make_success(type_compatibility::AUTOCAST_CAST);
            }

            // structs, check field by field has a 100% compatible type
            // then check code compatibility, layout + aliases + getter + setters
            // REVIEW method compatibility is required ?
            if (lhs->is<Struct>())
            {
                auto lhs_st = lhs->as<Struct>();
                auto rhs_st = rhs->as<Struct>();

                if (lhs_st->field_count != rhs_st->field_count)
                {
                    return make_error(std::format("LGERR_TS004 Incompatible types '{}' -> '{}'. Types has different fields count.", lhs_st->get_repr(), rhs_st->get_repr()), type_compatibility::EXPLICIT_CAST);
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
                        return make_chained_error(std::format("LGERR_TS005 Incompatible types '{}' -> '{}'. Incompatible field at position: '{}'.", lhs_st->get_repr(), rhs_st->get_repr(), i), x);
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

            return make_error(std::format("LGERR_TS004 Incompatible types '{}' -> '{}'. Unhanlded primitive.", lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
        }
        // REVIEW
        // if they don't have the same primitive, they are incompatible
        // even it's not 100% real, we will be extra safe at the start
        return make_error(std::format("LGERR_TS002 Incompatible types '{}' -> '{}'. Types should have the same primitive.", lhs->get_repr(), rhs->get_repr()), type_compatibility::NO);
    }

}