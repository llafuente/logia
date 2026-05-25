#include "logia/ast/postfixunaryexpr.h"
#include "logia/ast/identifier.h"

namespace logia::AST
{
    //
    // PostfixUnaryExpression
    //

    std::string PostfixUnaryExpression::to_string()
    {
        auto id = this->get_locator()->as<Identifier>();
        return std::format("PostfixUnaryExpression [{}]", id->identifier, Node::to_string());
    }

    PostfixUnaryExpression::PostfixUnaryExpression(antlr4::ParserRuleContext *rule, Operators op, Expression *operand) : CallExpression(rule)
    {
        this->op = op;

        // NOTE start as null, because we may don't know the types yet
        auto ident = new Identifier(rule, "");
        ident->skip_codegen = true;
        this->push_child(ident);

        this->push_positional_argument(operand);
    }

    Expression *PostfixUnaryExpression::get_operand()
    {
        return this->get_argument_expr(0);
    }

    void PostfixUnaryExpression::_post_type_inference()
    {
        auto operand = this->get_operand()->get_final_type();
        auto ident = this->get_locator()->as<Identifier>();
        ident->identifier = strdup(ast_postfix_unary_operator_to_string(this->op, operand));

        CallExpression::_post_type_inference();
    }

}