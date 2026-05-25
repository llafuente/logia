#include "logia/multiple_dispatch.h"
#include "logia/type_system.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    constexpr auto make_error = logia::utils::make_error<size_t, md_type_error>;
    constexpr auto make_success = logia::utils::make_success<size_t, md_type_error>;
    constexpr auto make_chained_error = logia::utils::make_chained_error<size_t, md_type_error>;

    multiple_dispatch_result match(CallExpression *callexpr, Function *func, bool change)
    {
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
            if (!param_name->is_empty()) {
                auto arg = callexpr->get_argument_by_name(param_name->identifier);

                if (arg != nullptr)
                {
                    auto arg_type = arg->get_final_type();
                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (!compatibility.is_error())
                    {
                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        arguments.push_back(arg->get_value());
                        goto next_parameter;
                    }

                    // passing 'struct a' to parameter of incompatible type 'int'

                    // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid named argument type: '{}' of type '{}'.\n{}", param_name->identifier, arg_type, compatibility.message), {param_index, compatibility.unsafe_unwrap()});
                    return make_error(std::format("{}", compatibility.message), { arg, compatibility.unwrap_error() });

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

                DEBUG() << debug << std::endl;
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
                    if (!compatibility.is_error())
                    {
                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        arguments.push_back(arg->get_value());
                        goto next_parameter;
                    }
                    else
                    {
                        // if param has default, we may have another oportunity later!
                        if (param->has_default_value())
                        {
                            used_params[param_index++] = true;
                            arguments.push_back(param->get_default_value());
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
                used_params[param_index++] = true;
                arguments.push_back(param->get_default_value());
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

        return make_success(0);
    }

    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *callexpr)
    {
        DEBUG() << std::format("Call ") << callexpr->to_string_tree() << std::endl;
        auto scope = callexpr->first_parent<Scope>();
        // TODO multiple dispatch is only available for Identifier and "rhs" (memberaccess) identifiers
        auto list = scope->lookup_all(callexpr->get_locator()->as<Identifier>()->identifier);
        DEBUG() << std::format("FOUND {} candidates", list.size()) << std::endl;
        std::vector<Function *> candidates;
        for (const auto &node : list)
        {
            Function *func;
            if (node->try_cast<Function>(&func))
            {
                DEBUG() << std::format("Function: ") << node->to_string() << std::endl;

                if (!match(callexpr, func, false).is_error())
                {
                    candidates.push_back(func);
                }
            }
            else
            {
                DEBUG() << std::format("Candidate is not a function?!") << node->to_string() << std::endl;
            }
        }
        if (candidates.size() == 1)
        {
            return candidates[0];
        }
        else if (candidates.size() > 1)
        {
            std::string debug_candidates = "";
            int i = 1;
            for (const auto &candidate : candidates)
            {
                debug_candidates += std::format("Candidate {}: \n{}Declared {}\n", i++, candidate->get_repr(), candidate->get_debug_location(0, 0));
            }

            // TODO find the most specific overload
            throw_semantic_error(callexpr, "LGERR_MD002 Ambiguous call expression, multiple candidates found: " + callexpr->get_debug_location() + "\n" + debug_candidates);
        }
        else
        {
            throw std::runtime_error(std::format("LGERR_MD001 No matching function found for call expression.\n{}", callexpr->get_debug_location()));
        }
    }
}