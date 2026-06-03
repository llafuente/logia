#include "logia/type_inference.h"

#include "logia/log.h"
#include "logia/ast/semantic_error.h"
#include "logia/ast/program.h"
#include "logia/ast/type.h"
#include "logia/ast/stmt.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/struct.h"
#include "logia/ast/structinitexpr.h"

#include <iostream>

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
        LOG(DBG, "{}={}", expr->to_string_tree(), enforce_type->get_repr());

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

    void type_inference_node(AST::Program *program, AST::Node *node)
    {
        // auto program = node->first_parent<Program>();
        auto default_integer = program->look<Type>("λi64");
        auto default_float = program->look<Type>("λf64");

        auto all_nodes = node->get_post_descendant();
        all_nodes.push_back(node);
        std::vector<Node *> pending;
        IntegerLiteral *ilit;
        FloatLiteral *flit;
        LOG(INF, "start pre_type_inference: found {} nodes", all_nodes.size());

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

            if (!node->is_typed)
            {
                if (node->is<IntegerLiteral>())
                {
                    // todo_type_stack.push_back({node, default_integer});
                    node->set_type(default_integer);
                }
                else if (node->is<FloatLiteral>())
                {
                    // todo_type_stack.push_back({node, default_float});
                    node->set_type(default_float);
                }
            }
        }
        LOG(INF, "end pre_type_inference: pending {} nodes", pending.size());

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
                    LOG(ERR, "{}\n{}", node->to_string(), node->get_debug_location());
                }
                throw_compiler_error("Could not finish pre_type_inference for some nodes!");
            }
        }

        LOG(DBG, "Found {}  literals", todo_type_stack.size());

        // if at the end, this nodes are not resolved, force them!
        for (const auto &it : todo_type_stack)
        {
            auto ty = it.first->get_type();
            if (ty == nullptr || ty->is<InferType>())
            {
                LOG(DBG, "set default type because it's infer {}", it.first->to_string());
                it.first->set_type(it.second);
            }
        }
        todo_type_stack.clear();

        LOG(DBG, "start post_type_inference: pending {} nodes", pending.size());

        for (auto node : all_nodes)
        {
            // DEBUG() << node->to_string() << std::endl;
            node->post_type_inference();
            // TODO queue post ?
        }

#if _DEBUG
        // do not use: all_nodes, as many node could create new ones, this is a check of those new nodes too!
        node->foreach_post_descendant([program](Node *node, int deep)
                                      { if (node->has_type && !node->is_typed) {
                                            //std::cerr << program->to_string_tree() << std::endl;
            std::cerr << node->parent_node->to_string_tree() << std::endl;
                                            std::cerr << node->get_debug_location() << std::endl;
                                            throw_compiler_error("Not able to find type, no error.");
                                            } });
#endif
    }

    void type_inference_program(AST::Program *program)
    {
        // guard!
        if (program->is_typed)
        {
            LOG(DBG, "skip type_inference, program is already typed!");
            return;
        }
        program->is_typed = true;

        LOG(INF, "Tree before type_inference!\n{}", program->to_string_tree());

        type_inference_node(program, program->intrinsics);
        type_inference_node(program, program);

        LOG(INF, "Tree after type_inference!\n{}", program->to_string_tree());
    }
}