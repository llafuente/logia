#include "logia/type_inference.h"
#include "ast/semantic_error.h"
#include "ast/program.h"
#include "ast/type.h"
#include "ast/stmt.h"
#include "ast/constexpr.h"

namespace logia
{
    using namespace logia::AST;

    std::vector<std::pair<Node *, Type *>> todo_type_stack;

    void type_inference_assert_not_equals(Type *expr_ty, Type *enforce_type)
    {
        expr_ty = expr_ty->get_final_type();
        enforce_type = enforce_type->get_final_type();

        if (expr_ty->cg_value != enforce_type->cg_value)
        {
            std::cerr << expr_ty->to_string_tree() << std::endl;
            throw_semantic_error(expr_ty, std::format("expected {} to be {}", expr_ty->get_repr(), enforce_type->get_repr()));
        }
    }

    void type_inference_expression(Expression *expr, Type *enforce_type)
    {
        // DEBUG() << expr->to_string_tree() << "=" << enforce_type->get_repr() << "|" << expr->rule->getText() << std::endl;
        DEBUG() << expr->to_string_tree() << "=" << enforce_type->get_repr() << std::endl;

        StructInitializer *sinit;
        if (expr->try_cast<StructInitializer>(&sinit))
        {
            if (!enforce_type->is<Struct>())
            {
                throw_semantic_error(expr, std::format("LGER030 incompatible type '{}', expected a struct", enforce_type->get_repr()));
            }
            if (!sinit->is_typed)
            {
                sinit->set_type(enforce_type);
            }
            else
            {
                // TODO warning, "double type!?"
            }
        }
        else if (expr->is<IntegerLiteral>() || expr->is<FloatLiteral>() || expr->is<StringLiteral>())
        {
            expr->set_type(enforce_type);
        }
        else if (expr->is<CallExpression>())
        {
            /*
            auto call = expr->as<CallExpression>();
            for (const auto& it : call->get_arguments()) {

            }
            */
            // TODO enforece return type!
        }
        // more rules ?
    }

    void type_inference(AST::Program *program)
    {
        // guard!
        if (program->is_typed)
        {
            DEBUG() << "skip type_inference, program is already typed!" << std::endl;
            return;
        }
        program->is_typed = true;

        DEBUG() << program->to_string_tree() << std::endl;

        auto default_integer = program->look<Type>("λi64");
        auto default_float = program->look<Type>("λf64");

        auto all_nodes = program->get_post_descendant();
        std::vector<Node *> pending;
        std::reverse(all_nodes.begin(), all_nodes.end());
        IntegerLiteral *ilit;
        FloatLiteral *flit;
        for (auto node : all_nodes)
        {
            // DEBUG() << node->to_string() << std::endl;
            node->pre_type_inference();
            // pre_type_inference could be imposible to be done for some nodes (like Identifiers)
            // it' may require that everyone around has pre_type_inference, so we need to introduce a way to delay retry this call again
            if (!node->skip_type_inference && !node->is_pre_type_inference)
            {
                pending.push_back(node);
            }

            if (node->is<IntegerLiteral>())
            {
                todo_type_stack.push_back({node, default_integer});
            }
            else if (node->is<FloatLiteral>())
            {
                todo_type_stack.push_back({node, default_float});
            }
        }

        while (pending.size())
        {
            auto before = pending.size();

            pending.erase(std::remove_if(pending.begin(), pending.end(),
                                         [](auto node)
                                         {
                                             node->pre_type_inference();
                                             return node->is_pre_type_inference;
                                         }),
                          pending.end());

            if (before == pending.size())
            {
                for (auto node : pending)
                {
                    LERROR() << node->to_string() << std::endl;
                    LERROR() << node->get_debug_location() << std::endl;
                }
                throw_compiler_error("Could not finish pre_type_inference for some nodes!");
            }
        }

        /*
                program->foreach_post_descendant([default_integer, default_float](Node *node, int deep)
                                                 {
                                                    node->pre_type_inference();
        if (node->is<IntegerLiteral>()) {
                                                    todo_type_stack.push_back({node, default_integer});
                                                }
                                                else if (node->is<FloatLiteral>()) {
                                                    todo_type_stack.push_back({ node, default_float });
                                                } });
                                                */
        DEBUG() << "Found " << todo_type_stack.size() << " literals" << std::endl;
        program->foreach_post_descendant([program](Node *node, int deep)
                                         {
                                        if (node->is<VarDeclStmt>()) {

                                        } else if (node->is<Function>()) {
                                            DEBUG() << node->to_string() << std::endl;

                                            auto f = node->as<Function>();
                                            auto return_ty = f->get_return_type()->get_final_type();
                                            if (!return_ty->is<InferType>()) {
                                                // we are going to type all return stmt!
                                                //type_inference_return_stmt(f->get_body(), return_ty);
                                            }

                                        } });

        // if at the end, this nodes are not resolved, force them!
        for (const auto &it : todo_type_stack)
        {
            auto ty = it.first->get_type();
            if (ty == nullptr || ty->is<InferType>())
            {
                DEBUG() << "set default type because it's infer" << it.first->to_string() << std::endl;
                it.first->set_type(it.second);
            }
        }
        todo_type_stack.clear();

        program->foreach_post_descendant([](Node *node, int deep)
                                         { node->post_type_inference(); });

        // check
        program->foreach_post_descendant([](Node *node, int deep)
                                         { if (node->has_type && !node->is_typed) {
                                            std::cerr << node->to_string_tree() << std::endl;
                                            std::cerr << node->parent_node->to_string_tree() << std::endl;
                                            throw_compiler_error("Not able to find type, no error.");
                                         } });

        DEBUG() << program->to_string_tree() << std::endl;
    }
}