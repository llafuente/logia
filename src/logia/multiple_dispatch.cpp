#include "logia/multiple_dispatch.h"

#include "logia/backend.h"
#include "logia/ast/node.h"
#include "logia/ast/scope.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/cast.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"

#include "logia/type_system.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    constexpr auto make_error = logia::utils::make_error<float, md_type_error>;
    constexpr auto make_success = logia::utils::make_success<float, md_type_error>;
    constexpr auto make_chained_error = logia::utils::make_chained_error<float, md_type_error>;

    multiple_dispatch_result match(CallExpression *callexpr, Function *func, bool change)
    {
        float points = 1;
        auto params = func->get_parameters();
        auto used_params = std::vector<bool>(params.size(), false);
        auto used_args = std::vector<bool>(callexpr->argument_count, false);
        auto arguments = std::vector<Expression *>();
        arguments.reserve(params.size());

        // sanity checks first

        // arguments < parameters
        if (callexpr->argument_count > func->get_parameter_count())
        {
            return make_error(std::format("LGERR_MD003 Invalid argument count. '{}' takes '{}' arguments but '{}' were given.", func->get_repr(), func->get_parameter_count(), callexpr->argument_count), {callexpr, type_system::type_compatibility::NO});
        }

        // all names args are parameters
        for (size_t i = 1, j = 0; i < callexpr->children.size(); i += 2, ++j)
        {
            auto arg = callexpr->children[i]->as<CallExpressionArgument>();

            auto arg_name = arg->get_name();
            if (strlen(arg_name->identifier) > 0)
            {
                if (!func->get_parameter_by_name(arg_name->identifier))
                {
                    return make_error(std::format("LGERR_MD002 Invalid argument name '{}', could not be found in the function: '{}'.", arg_name->identifier, func->get_repr()), {arg_name, type_system::type_compatibility::NO});
                }
            }
        }

        // search all parameters by name in args, mark done if has a compatible type -> error otherwise
        size_t param_index = 0;
        for (auto param : params)
        {
            auto param_name = param->get_name();
            auto param_type = param->get_final_type();
            if (!param_name->is_empty())
            {
                auto arg = callexpr->get_argument_by_name(param_name->identifier);

                if (arg != nullptr)
                {
                    auto arg_type = arg->get_final_type();
                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (compatibility.is_success())
                    {
                        auto c = (uint32_t)compatibility.unwrap_success();
                        LOG(DBG, "paramter/argument by name[{}] of type [{}] compatibility = {}", param_name->identifier, param_type->get_repr(), c);

                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        auto value = arg->get_value();

                        if ((c & (uint32_t)type_system::type_compatibility::AUTOCAST_CAST) != 0)
                        {
                            // allow ConstExpression to be casted!
                            if (value->is<ConstExpression>())
                            {
                                if (change)
                                {
                                    value->set_type(param_type);
                                    arguments.push_back(value);
                                }
                            }
                            else
                            {
                                if (change)
                                {
                                    arguments.push_back(new Cast(arg->rule, value, param_type));
                                }
                                points *= 0.5;
                            }
                        }
                        else
                        {
                            if (change)
                            {
                                arguments.push_back(value);
                            }
                        }

                        goto next_parameter;
                    }

                    // passing 'struct a' to parameter of incompatible type 'int'

                    // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid named argument type: '{}' of type '{}'.\n{}", param_name->identifier, arg_type, compatibility.message), {param_index, compatibility.unsafe_unwrap()});
                    return make_error(std::format("{}", compatibility.message), {arg, compatibility.unwrap_error()});

                } // this param should be check in the next round, by position
            }
#ifdef _DEBUG
            {
                std::string debug = "\nparams = ";
                for (size_t i = 0; i < used_params.size(); ++i)
                {
                    debug += used_params[i] ? "1" : "0";
                }
                debug += "\n  args = ";
                for (size_t i = 0; i < used_args.size(); ++i)
                {
                    debug += used_args[i] ? "1" : "0";
                }

                LOG(DBG, "{}", debug);
            }
#endif
            // now we match by position, the first with no-name!

            // fetch the first "non-named" argument
            for (size_t i = 0; i < callexpr->argument_count; ++i)
            {
                auto arg = callexpr->get_argument_by_index(i);

                auto arg_name = arg->get_name();
                if (arg_name->is_empty() && !used_args[i])
                {
                    auto arg_type = arg->get_final_type();
                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (compatibility.is_success())
                    {
                        auto c = (uint32_t)compatibility.unwrap_success();
                        LOG(DBG, "parameter/argument by position[{}] of type [{}] compatibility = {}", param_name->identifier, param_type->get_repr(), c);

                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        auto value = arg->get_value();

                        if ((c & (uint32_t)type_system::type_compatibility::AUTOCAST_CAST) != 0)
                        {
                            if (!value->is<ConstExpression>())
                            {
                                if (change)
                                {
                                    arguments.push_back(new Cast(arg->rule, value, param_type));
                                }
                                points *= 0.5;
                            }
                            else
                            {
                                if (change)
                                {
                                    value->set_type(param_type);
                                    arguments.push_back(value);
                                }
                            }
                        }
                        else
                        {
                            if (change)
                            {
                                arguments.push_back(value);
                            }
                        }

                        goto next_parameter;
                    }
                    else
                    {
                        // if param has default, we may have another oportunity later!
                        if (param->has_default_value())
                        {
                            LOG(DBG, "parameter default value[{}] of type [{}]", param_name->identifier, param_type->get_repr());

                            used_params[param_index++] = true;
                            if (change)
                            {
                                arguments.push_back(param->get_default_value());
                            }
                            goto next_parameter;
                        }

                        return make_error(std::format("LGERR_MD001 Invalid argument type at position '{}' of type '{}', expected to match parameter '{}' of type: '{}'\n{}", i + 1, arg_type->get_repr(), param_name->identifier, param_type->get_repr(), compatibility.message), {param, compatibility.unwrap_error()});

                        // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid positional argument '{}'.\n{}", i, compatibility.message), );
                        // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid positional argument '{}'.\n{}", i, compatibility.message), {param_index, compatibility.unsafe_unwrap()});
                        // return multiple_dispatch_result(std::format("{}", compatibility.message), {param_index, compatibility.unsafe_unwrap()});
                    }
                }
            }
            // if param has default our last change!
            if (param->has_default_value())
            {
                LOG(DBG, "parameter default value[{}] of type [{}]", param_name->identifier, param_type->get_repr());
                used_params[param_index++] = true;
                if (change)
                {
                    arguments.push_back(param->get_default_value());
                }
                goto next_parameter;
            }
            return make_error(std::format("LGERR_MD004 Missing required argument '{}'. Calling '{}'", param_name->identifier, func->get_repr()), {param, type_system::type_compatibility::NO});

            // target for nested for loops :)
        next_parameter:;
        }

        if (change)
        {
            // empty
            while (callexpr->argument_count)
            {
                callexpr->remove_argument_at(0);
            }
            for (auto expr : arguments)
            {
                callexpr->push_positional_argument(expr);
            }
        }

        return make_success(points);
    }

    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *callexpr)
    {
        LOG(DBG, "Call {}", callexpr->to_string_tree());

        auto scope = callexpr->first_parent<Scope>();
        // TODO multiple dispatch is only available for Identifier and "rhs" (memberaccess) identifiers
        auto list = scope->lookup_all(callexpr->get_locator()->as<Identifier>()->identifier);
        LOG(DBG, "FOUND {} candidates", list.size());
        std::vector<std::tuple<float, Function *>> candidates;
        for (const auto &node : list)
        {
            Function *func;
            if (node->try_cast<Function>(&func))
            {
                LOG(DBG, "Function: {}", node->to_string());
                auto m = match(callexpr, func, false);
                if (m.is_success())
                {
                    auto f = m.unwrap_success();
                    LOG(DBG, "candidate points = {}", f);
                    candidates.push_back({f, func});
                }
                else
                {
                    auto f = m.unwrap_error();
                    LOG(DBG, "candidate no good = {}", f.reason.to_string());
                }
            }
            else
            {
                LOG(DBG, "Candidate is not a function?! {}", node->to_string());
            }
        }

        if (candidates.size() == 0)
        {
            std::string debug_candidates = "";
            int i = 1;
            for (const auto &node : list)
            {
                Function *func;
                if (node->try_cast<Function>(&func))
                {
                    debug_candidates += std::format("Candidate {} with: \n{}Declared {}\n", i++, func->get_repr(), func->get_debug_location(0, 0));
                }
                ++i;
            }
            throw std::runtime_error(std::format("LGERR_MD001 No matching function found for call expression.\n{}\nPossible candidates:\n{}", callexpr->get_debug_location(), debug_candidates));
        }

        if (candidates.size() == 1)
        {
            return std::get<1>(candidates[0]);
        }

        // from all valid candidates, we should have only one with "1"
        Function *candidate1 = nullptr;
        for (const auto &candidate : candidates)
        {
            if (std::get<0>(candidate) == 1)
            {
                if (candidate1 == nullptr)
                {
                    candidate1 = std::get<1>(candidate);
                }
                else
                {
                    // two with 1 ? OMG! -> show the error!
                    candidate1 = nullptr;
                    break;
                }
            }
        }
        if (candidate1 != nullptr)
        {
            return candidate1;
        }

        std::string debug_candidates = "";
        int i = 1;
        for (const auto &candidate : candidates)
        {
            auto points = std::get<0>(candidate);
            auto f = std::get<1>(candidate);
            debug_candidates += std::format("Candidate {} with {} points: \n{}Declared {}\n", i++, points, f->get_repr(), f->get_debug_location(0, 0));
        }

        // TODO find the most specific overload
        throw_semantic_error(callexpr, "LGERR_MD002 Ambiguous call expression, multiple candidates found: " + callexpr->get_debug_location() + "\n" + debug_candidates);
    }
}