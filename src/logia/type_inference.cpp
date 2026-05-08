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
            throw_semantic_error(expr_ty, std::format("expected {} to be {}", expr_ty->get_repr(), enforce_type->get_repr()));
        }
    }

    void type_inference_expression(Expression *expr, Type *enforce_type)
    {
        DEBUG() << expr->to_string_tree() << "=" << enforce_type->get_repr() << std::endl;

        StructInitializer *sinit;
        if (expr->try_cast<StructInitializer>(&sinit))
        {
            if (!enforce_type->is<Struct>())
            {
                throw_semantic_error(expr, std::format("expected a struct type but found {}", enforce_type->get_repr()));
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

    void type_inference_return_stmt(Block *block, Type *enforce_type)
    {
        block->foreach_descendant([enforce_type](Node *node, int deep)
                                  {
    if (node->is<ReturnStmt>()) {
        auto rstmt = node->as<ReturnStmt>();
        DEBUG() << rstmt->to_string() << std::endl;

        auto expr = rstmt->get_expr();
        auto expr_ty = expr->get_type(); // TODO get_final_type ?

        DEBUG() << "START TYPE = " << expr_ty->to_string() << std::endl;
        if (expr_ty->is<InferType>()) {
            type_inference_expression(expr, enforce_type);
            return false;
        }

        type_inference_assert_not_equals(expr_ty, enforce_type);

        //DEBUG() << rstmt->to_string() << rstmt->get_final_type()->to_string() << std::endl;
        return false; // dont need to continue further as we just want all returns
    }
return true; });
    }

    void type_inference_vardecl(AST::VarDeclStmt *vardecl)
    {
        DEBUG() << vardecl->to_string() << std::endl;

        auto expr = vardecl->get_expr();
        if (vardecl->is_typed)
        {
            // TODO determine type if possible
            // TODO what we do when we cant ? push somewhere and back later ?
            auto ty = vardecl->get_final_type();

            type_inference_expression(expr, ty);
        }
        else
        {
            auto ty = expr->get_final_type();
            if (ty->is<InferType>())
            {
                throw_semantic_error(vardecl, "TODO");
            }
            vardecl->set_type(ty);
        }
    }

    void type_inference(AST::Program *program)
    {
        DEBUG() << program->to_string_tree() << std::endl;

        program->foreach_descendant([](Node *node, int deep)
                                    { node->pre_type_inference(); return true; });

        program->foreach_descendant([program](Node *node, int deep)
                                    {


                                        if (node->is<VarDeclStmt>()) {
                                            type_inference_vardecl(node->as<VarDeclStmt>());
                                        } else if (node->is<Function>()) {
                                            DEBUG() << node->to_string() << std::endl;

                                            auto f = node->as<Function>();
                                            auto return_ty = f->get_return_type()->get_final_type();
                                            if (!return_ty->is<InferType>()) {
                                                // we are going to type all return stmt!
                                                type_inference_return_stmt(f->get_body(), return_ty);
                                            }

                                        } else if (node->is<IntegerLiteral>()) {
                                            todo_type_stack.push_back({node, program->look<Type>("λi64")});
                                        }
                                        else if (node->is<FloatLiteral>()) {
                                            todo_type_stack.push_back({ node, program->look<Type>("λf64") });
                                        }
                                    return true; });

        // if at the end, this nodes are not resolved, force them!
        for (const auto &it : todo_type_stack)
        {
            if (it.first->get_type()->is<InferType>())
            {
                DEBUG() << "set default type because it's infer" << it.first->to_string() << std::endl;
                it.first->set_type(it.second);
            }
        }
        todo_type_stack.clear();

        program->foreach_post_descendant([](Node *node, int deep)
                                         { node->post_type_inference(); });

        DEBUG() << program->to_string_tree() << std::endl;
    }
}