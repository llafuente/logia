#include "logia/multiple_dispatching.h"

namespace logia::multiple_dispatching
{
    using namespace logia::AST;
    bool match(CallExpression *call_expression, Function *func, bool change)
    {
        auto params = func->get_parameters();
        auto used_params = std::vector<bool>(params.size(), false);
        auto args = call_expression->get_arguments();
        auto used_args = std::vector<bool>(args.size(), false);

        // search all parameters by name in args, mark done if has a compatible type -> throw otherwise
        for (auto param : params)
        {
            auto param_name = param->get_name();
            auto arg = call_expression->get_argument_by_name(param_name->identifier);

            if (arg != nullptr)
            {
                auto arg_type = arg->get_final_type();
                auto param_type = param->get_final_type();
                auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                if (!compatibility.is_error())
                {
                    used_params[i] = true;
                    used_args[arg->index] = true;
                    continue; // eat param/arg, next!
                }

                return false; // type mismatch, TODO return more info!

            } // this param should be check in the next round, by position

#ifdef _DEBUG
            {
                std::string debug = "params = ";
                for (size_t i = 0; i < used_params.size(); ++i)
                {
                    debug += used_params[i] ? "1" : "0";
                }
                debug += "\nargs = ";
                for (size_t i = 0; i < used_args.size(); ++i)
                {
                    debug += used_args[i] ? "1" : "0";
                }

                DEBUG() << debug << std::endl;
            }
#endif
            // now we match by position, the first with no-name!

            // fetch the first "non-named" argument
            for (size_t i = 1, j = 0; i < call_expression->children->size(); i += 2, ++j)
            {
                auto arg_name = call_expression->children[i]->as<Identifier>();
                if (strlen(arg_name->identifier) == 0 && !used_args[j])
                {
                    auto arg = call_expression->children[i + 1]->as<Expression>();
                    auto arg_type = arg->get_final_type();
                    auto param_type = param->get_final_type();
                    auto compatibility = type_system::type_is_compatible(arg_type, param_type);
                    if (!compatibility.is_error())
                    {
                        used_params[i] = true;
                        used_args[arg->index] = true;
                        continue; // eat param/arg, next!
                    }
                    else
                    {
                        // if param has default, we may have another oportunuty later!
                        if (!param->has_default_value())
                        {
                            used_params[i] = true;
                            continue; // eat param, next!
                        }
                        return false; // type mismatch
                    }
                }
            }
        }

        return true;
    }

    /// @brief Searches for the most specific function overload that matches the given call expression.
    /// @param call_expression The call expression to match against available overloads.
    Function *find(CallExpression *call_expression)
    {
        auto scope = call_expression->first_paren<Scope>();
        auto list = scope->lookup(call_expression->get_name());
        std::vector<Function *> candidates;
        for (const auto &node : list)
        {
            Function *func;
            if (node->try_cast<Function>(&func))
            {
                if (match(call_expression, func))
                {
                    candidates.push_back(func);
                }
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
            throw_semantic_error(call_expression, "LGERR_MD002 Ambiguous call expression, multiple candidates found: " + call_expression->get_debug_location() + "\n" + debug_candidates);
        }
        else
        {
            throw std::runtime_error(std::format("LGERR_MD001 No matching function found for call expression.\n{}", call_expression->get_debug_location()));
        }
    }
}