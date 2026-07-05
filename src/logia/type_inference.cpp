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

    void type_inference_pass(AST::Program *program, AST::Node *start_node, size_t pass_id)
    {
        // update after each pass!
        auto all_nodes = start_node->get_post_descendant();
        all_nodes.push_back(start_node);
        std::vector<Node *> pending;

        LOG(INF, "start type_inference pass {}: found {} nodes", pass_id, all_nodes.size());

        // 1st step, initialize literal types!
        if (pass_id == TYPE_INFERENCE_EARLY)
        {
            // TODO cache
            // auto program = node->first_parent<Program>();
            auto default_integer = scope_look_one<Type>(program, "λi64");
            auto default_float = scope_look_one<Type>(program, "λf64");
            for (auto node : all_nodes)
            {
                if (!node->is_typed)
                {
                    if (node->is<IntegerLiteral>())
                    {
                        node->set_type(default_integer);
                    }
                    else if (node->is<FloatLiteral>())
                    {
                        node->set_type(default_float);
                    }
                }
            }
        }

        for (auto node : all_nodes)
        {
            // if any node create more nodes, those fall behind!
            // this is not really performant, but there is no way atm to try until "current_pass_id"
            for (size_t current_pass_id = node->type_inference_pass_id + 1; current_pass_id <= pass_id; ++current_pass_id)
            {
                node->type_inference(pass_id);
            }
            // pre_type_inference could be imposible to be done for some nodes (like Identifiers)
            // it' may require that everyone around has pre_type_inference, so we need to introduce a way to delay retry this call again
            if (node->type_inference_pass_id < pass_id)
            {
                LOG(DBG, "Node can't finish pre_type_inference queue: {}", node->to_string_tree());
                pending.push_back(node);
            }
        }

        // process 2nd queue
        LOG(INF, "end pre_type_inference: pending {} nodes", pending.size());
        while (pending.size())
        {
            LOG(DBG, "start pre_type_inference pending with {} items", pending.size());

            auto before = pending.size();

            pending.erase(std::remove_if(pending.begin(), pending.end(),
                                         [pass_id](auto node)
                                         {
                                             for (size_t pass_id = node->type_inference_pass_id + 1; pass_id <= pass_id; ++pass_id)
                                             {
                                                 node->type_inference(pass_id);
                                             }

                                             return node->type_inference_pass_id >= pass_id;
                                         }),
                          pending.end());

            if (before == pending.size())
            {
                for (auto node : pending)
                {
                    std::cerr << std::format("tree: {}\nlocation: {}", node->to_string_tree(), node->loc.get_debug_location());
                }
                throw_compiler_error(std::format("Could not finish pass_id: {} for some nodes!", pass_id));
            }
        }
    }

    void type_inference_node(AST::Program *program, AST::Node *node)
    {
        for (size_t current_pass_id = 1; current_pass_id <= TYPE_INFERENCE_MAX; ++current_pass_id)
        {
            type_inference_pass(program, node, current_pass_id);
        }

#if _DEBUG
        // do not use: all_nodes, as many node could create new ones, this is a check of those new nodes too!
        node->foreach_post_descendant([program](Node *node, int deep)
                                      { if (node->has_type && !node->is_typed) {
                                            //std::cerr << program->to_string_tree() << std::endl;
            std::cerr << node->parent_node->to_string_tree() << std::endl;
                                            std::cerr << node->loc.get_debug_location() << std::endl;
                                            throw_compiler_error(std::format("Not able to find type, no error: {}", node->to_string()));
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

        START_INTRINSICS();
        LOG(INF, "type_inference -> intrinsics!");
        type_inference_node(program, program->intrinsics);
        STOP_INTRINSICS();

        LOG(INF, "type_inference -> program!");
        type_inference_node(program, program);

        LOG(INF, "Tree after type_inference!\n{}", program->to_string_tree());
    }
}