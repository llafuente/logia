#include "logia/multiple_dispatch.h"

#include "logia/backend.h"
#include "logia/ast/node.h"
#include "logia/ast/scope.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/cast.h"
#include "logia/ast/identifier.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/unaryexpr.h"

#include "logia/type_system.h"

namespace logia::multiple_dispatch
{
    using namespace logia::AST;

    constexpr auto make_match_error = logia::utils::make_error<float, match_error>;
    constexpr auto make_match_success = logia::utils::make_success<float, match_error>;
    constexpr auto make_match_chained_error = logia::utils::make_chained_error<float, match_error>;

    match_result match(CallExpression *callexpr, Function *func, bool change)
    {
        LOG(DBG, "({}, {}, {})", (void *)callexpr, (void *)func, change);
        float points = 1;
        auto params = func->get_parameters();
        auto used_params = std::vector<bool>(params.size(), false);
        auto used_args = std::vector<bool>(callexpr->argument_count, false);
        auto arguments = std::vector<Expression *>();
        arguments.reserve(params.size());

        // sanity checks first

        // check: arguments < parameters
        if (callexpr->argument_count > func->get_parameter_count())
        {
            return make_match_error(std::format("LGERR_MD003 Invalid argument count. '{}' takes '{}' arguments but '{}' were given.", func->get_repr(), func->get_parameter_count(), callexpr->argument_count), {callexpr, type_system::type_compatibility::NO});
        }

        CallExpressionArgument *arg;
        Identifier *arg_name;
        TypeDecl *arg_type;

        // check: all named arguments are valid parameter names
        for (size_t i = 1, j = 0; i < callexpr->children.size(); i += 2, ++j)
        {
            arg = callexpr->children[i]->as<CallExpressionArgument>();

            arg_name = arg->get_name();
            if (strlen(arg_name->identifier) > 0)
            {
                if (!func->get_parameter_by_name(arg_name->identifier))
                {
                    return make_match_error(std::format("LGERR_MD002 Invalid argument name '{}', could not be found in the function: '{}'.", arg_name->identifier, func->get_repr()), {arg_name, type_system::type_compatibility::NO});
                }
            }
        }

        // search all parameters by name in arguments if types are compatible -> ok, error otherwise!
        size_t param_index = 0;
        for (auto param : params)
        {
            auto param_name = param->get_name();
            auto param_type = param->get_type_decl()->get_effective_type_decl();
            if (!param_name->is_empty())
            {
                arg = callexpr->get_argument_by_name(param_name->identifier);

                if (arg != nullptr)
                {
                    arg_type = arg->get_type_decl()->get_effective_type_decl();
                    ;
                    LOG(SILLY, "found a parameter['{}' of '{}'] for argument['{}'] by name", param_name->identifier, param_type->get_repr(), arg_type->get_repr());

                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (compatibility.is_success())
                    {
                        auto c = compatibility.unwrap_success();
                        LOG(DBG, "paramter/argument by name[{}] of type [{}] compatibility = {}", param_name->identifier, param_type->get_repr(), c.to_string());

                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        auto value = arg->get_value();

                        if (c.contains(type_system::type_compatibility::AUTOCAST_CAST))
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
                                    arguments.push_back(new Cast(arg->loc, value, param_type));
                                }
                                points *= 0.5;
                            }
                        }
                        else if (c.contains(type_system::type_compatibility::AUTOCAST_REF))
                        {
                            if (value->is<ConstExpression>())
                            {
                                return make_match_error(std::format("LGERR_MD006 Invalid argument {} of type {}. Cannot ref from a const-expr.\n{}", arg->index + 1, arg_type->get_repr(), compatibility.message), {arg, compatibility.unwrap_error()});
                            }
                            else
                            {
                                arguments.push_back(new UnaryExpression(arg->loc, Operators::PREFIX_REFERENCE, value));
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

                    return make_match_error(std::format("{}", compatibility.message), {arg, compatibility.unwrap_error()});

                } // this param should be check in the next round, by position
            }
#ifdef _SILLY
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
                if (used_args[i])
                {
                    continue;
                }

                arg = callexpr->get_argument_by_index(i);

                arg_name = arg->get_name();
                if (arg_name->is_empty())
                {
                    arg_type = arg->get_type_decl()->get_effective_type_decl();
                    ;
#ifdef _DEBUG
                    if (arg_type == nullptr)
                    {
                        arg->get_type_decl()->get_effective_type_decl();
                        ;
                        std::cout << arg_type->to_string_tree() << std::endl;
                    }
#endif
                    LOG(SILLY, "found a parameter['{}' of '{}'] for argument['{}'] by position {}", param_name->identifier, param_type->get_repr(), arg_type->get_repr(), i);

                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (compatibility.is_success())
                    {
                        auto c = compatibility.unwrap_success();
                        LOG(DBG, "parameter/argument by position[{}] of type [{}] compatibility = {}", param_name->identifier, param_type->get_repr(), c.to_string());

                        used_params[param_index++] = true;
                        used_args[arg->index] = true;
                        auto value = arg->get_value();

                        if (c.contains(type_system::type_compatibility::AUTOCAST_CAST))
                        {
                            if (!value->is<ConstExpression>())
                            {
                                if (change)
                                {
                                    arguments.push_back(new Cast(arg->loc, value, param_type));
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
                        else if (c.contains(type_system::type_compatibility::AUTOCAST_REF))
                        {
                            if (value->is<ConstExpression>())
                            {
                                return make_match_error(std::format("LGERR_MD006 Invalid argument {} of type {}. Cannot ref from a const-expr.\n{}", arg->index + 1, arg_type->get_repr(), compatibility.message), {arg, compatibility.unwrap_error()});
                            }
                            else
                            {
                                arguments.push_back(new UnaryExpression(arg->loc, Operators::PREFIX_REFERENCE, value));
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

                        return make_match_error(std::format("LGERR_MD001 Invalid argument type at position '{}' of type '{}', expected to match parameter '{}' of type: '{}'\n{}", i + 1, arg_type->get_repr(), param_name->identifier, param_type->get_repr(), compatibility.message), {param, compatibility.unwrap_error()});

                        // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid positional argument '{}'.\n{}", i, compatibility.message), );
                        // return maybe_error<std::tuple<size_t, type_system::type_compatibility>>(std::format("Invalid positional argument '{}'.\n{}", i, compatibility.message), {param_index, compatibility.unsafe_unwrap()});
                        // return match_result(std::format("{}", compatibility.message), {param_index, compatibility.unsafe_unwrap()});
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
            return make_match_error(std::format("LGERR_MD004 Missing required argument '{}'. Calling '{}'", param_name->identifier, func->get_repr()), {param, type_system::type_compatibility::NO});

            // target for nested for loops :)
        next_parameter:;
        }

        for (size_t i = 0; i < callexpr->argument_count; ++i)
        {
            if (!used_args[i])
            {
                return make_match_error(std::format("LGERR_MD005 Unused argument at position '{}'. Calling '{}'", i, func->get_repr()), {callexpr->get_argument_by_index(i), type_system::type_compatibility::NO});
            }
        }

        if (change)
        {
            // empty
            while (callexpr->argument_count)
            {
                callexpr->remove_argument_at(0);
            }
            size_t i = 0;
            for (auto expr : arguments)
            {
                // callexpr->push_positional_argument(expr);
                auto param = params[i++];
                auto nid = node_clone<Identifier>(param->get_name());
                nid->set_type(param->get_type_decl()->get_effective_type_decl());
                callexpr->push_named_argument(nid, expr);
            }
        }

        return make_match_success(points);
    }

    constexpr auto make_find_error = logia::utils::make_error<Function *, find_error>;
    constexpr auto make_find_success = logia::utils::make_success<Function *, find_error>;
    constexpr auto make_find_chained_error = logia::utils::make_chained_error<Function *, find_error>;

    find_one_result find_one(std::vector<Function *> functions, CallExpression *callexpr)
    {
        std::vector<std::tuple<float, Function *>> candidates(functions.size(), {0, nullptr});
        candidates.clear();

        Function *func;
        for (const auto &node : functions)
        {
            if (node->try_cast<Function>(&func))
            {
                LOG(DBG, "Function: {}", node->to_string());
                auto m = match(callexpr, func, false);
                if (m.is_success())
                {
                    auto f = m.unwrap_success();
                    LOG(DBG, "candidate OK with {} points", f);
                    candidates.push_back({f, func});
                }
                else
                {
                    auto f = m.unwrap_error();
                    LOG(DBG, "candidate KO {} / {}", f.reason.to_string(), m.message);
                }
            }
            else
            {
                LOG_ERR("Candidate is not a function?! {}", node->to_string());
                throw_compiler_error("unexpected type found");
            }
        }

        if (candidates.size() == 0)
        {
            std::string debug_candidates = "";
            int i = 1;
            for (const auto &node : functions)
            {
                Function *func;
                if (node->try_cast<Function>(&func))
                {
                    debug_candidates += std::format("Candidate {}:\n{}\n\nDeclared {}\n", i++, func->get_repr(), func->loc.get_debug_location(0, 0));
                }
                ++i;
            }
            std::string debug_arg_types = "";
            for (uint32_t i = 0; i < callexpr->argument_count; ++i)
            {
                debug_arg_types += debug_arg_types.size() ? ", " : "";
                debug_arg_types += callexpr->get_argument_by_index(i)->get_type_decl()->get_repr();
            }

            throw std::runtime_error(std::format("LGERR_MD001 No matching function found for call expression: ({})\n{}\nPossible candidates:\n{}", debug_arg_types, callexpr->loc.get_debug_location(), debug_candidates));
        }

        if (candidates.size() == 1)
        {
            auto f = std::get<1>(candidates[0]);
            LOG(DBG, "return the only viable candidate: {} {}", (void *)f, f->get_repr());
            return make_find_success(f);
        }

        // from all valid candidates, we should have only one with "1"
        Function *candidate_with_1point = nullptr;
        for (const auto &candidate : candidates)
        {
            if (std::get<0>(candidate) == 1)
            {
                if (candidate_with_1point == nullptr)
                {
                    candidate_with_1point = std::get<1>(candidate);
                }
                else
                {
                    // two with 1 ? OMG! -> show the error!
                    candidate_with_1point = nullptr;
                    break;
                }
            }
        }
        if (candidate_with_1point != nullptr)
        {
            LOG(DBG, "return the only 1 point candidate: {} {}", (void *)candidate_with_1point, candidate_with_1point->get_repr());
            return make_find_success(candidate_with_1point);
        }

        std::string debug_candidates = "";
        int i = 1;
        for (const auto &candidate : candidates)
        {
            auto points = std::get<0>(candidate);
            auto f = std::get<1>(candidate);
            debug_candidates += std::format("Candidate {} with {} points: \n{}Declared {}\n", i++, points, f->get_repr(), f->loc.get_debug_location(0, 0));
        }

        return make_find_error(std::format("LGERR_MD002 Ambiguous call expression, multiple candidates found for:\n{}\n{}", callexpr->loc.get_debug_location(), debug_candidates), {candidates, callexpr});
    }
}
