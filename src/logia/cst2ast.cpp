#include "logia/cst2ast.h"

#include "utils.h"
#include "logia/ast/type.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/if_stmt.h"
#include "logia/ast/import.h"
#include "logia/ast/operators.h"
#include "logia/ast/expr.h"
#include "logia/ast/binaryexpr.h"
#include "logia/ast/unaryexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/memberaccessexpr.h"
#include "logia/ast/structinitexpr.h"
#include "logia/ast/block.h"
#include "logia/ast/vardeclstmt.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/identifier.h"

#define CST_THROW(msg)                                         \
    do                                                         \
    {                                                          \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl; \
        throw_compiler_error(msg);                             \
    } while (false)

#define CST_TODO_BRANCH(retrieve_method, visit_method) \
    do                                                 \
    {                                                  \
        auto x = context->retrieve_method();           \
        if (x != nullptr)                              \
        {                                              \
            CST_THROW("to-do");                        \
        }                                              \
    } while (false)

#define CST_TODO_BRANCH_LIST(retrieve_method, visit_method) \
    do                                                      \
    {                                                       \
        auto x = context->retrieve_method(0);               \
        if (x != nullptr)                                   \
        {                                                   \
            CST_THROW("to-do");                             \
        }                                                   \
    } while (false)

#define CST_UNREACHABLE()         \
    do                            \
    {                             \
        CST_THROW("unreachable"); \
    } while (false)

#define CST_VISIT_BRANCH(retrieve_method, visit_method) \
    do                                                  \
    {                                                   \
        auto stmt = context->retrieve_method();         \
        if (stmt != nullptr)                            \
        {                                               \
            return this->visit_method(stmt);            \
        }                                               \
    } while (false)

#define CST_VISIT_BRANCH2(retrieve_method, visit_method, type, block)            \
    do                                                                           \
    {                                                                            \
        auto sub_context = context->retrieve_method();                           \
        if (sub_context != nullptr)                                              \
        {                                                                        \
            auto node = ANY_VOIDP_CAST(type *, this->visit_method(sub_context)); \
            block;                                                               \
            return node;                                                         \
        }                                                                        \
    } while (false)

#define CST_DEBUG_FUNCTION()                \
    do                                      \
    {                                       \
        LOG(VRB, "{}", context->getText()); \
    } while (false)

#define ANY_VOIDP_STORE(expr) (void *)(expr)
#define ANY_VOIDP_CAST(type, expr) (type)(std::any_cast<void *>(expr))

#define MAKE_LOCATION(context) \
    {this->program->entry_point_file, context->start->getLine() - 1, context->start->getCharPositionInLine(), context->stop->getLine() - 1, context->stop->getCharPositionInLine(), this->program->file_contents}

namespace logia
{
    AST::Operators tk_to_operator(size_t op)
    {
        switch (op)
        {
            /// equality
        case LogiaParser::IS_TK:
        case LogiaParser::EXTENDS_TK:
        case LogiaParser::IMPLEMENTS_TK:
        case LogiaParser::INSTANCEOF_TK:
        // memory equality (pointer comparation)
        case LogiaParser::EQUALEQUALEQUAL_TK:
        // memory inequality (pointer comparation)
        case LogiaParser::NOT_EQUALEQUAL_TK:
        // floating point equality: abs(left - right) < epsilon
        case LogiaParser::ALMOSTEQUAL_TK:
            CST_THROW("TODO");
        // value equality
        case LogiaParser::EQUALEQUAL_TK:
            return AST::Operators::BINARY_COMPARISON_EQ;
        // value inequality
        case LogiaParser::NOT_EQUAL_TK:
            return AST::Operators::BINARY_COMPARISON_NEQ;
            /// assignament

        case LogiaParser::EQUAL_TK:
            return AST::Operators::BINARY_ASSIGN;
        case LogiaParser::STAR_ASSIGN_TK:
            return AST::Operators::BINARY_MUL_ASSIGN;
        case LogiaParser::DIV_ASSIGN_TK:
            return AST::Operators::BINARY_DIV_ASSIGN;
        case LogiaParser::ADD_ASSIGN_TK:
            return AST::Operators::BINARY_ADD_ASSIGN;
        case LogiaParser::SUB_ASSIGN_TK:
            return AST::Operators::BINARY_SUB_ASSIGN;
        case LogiaParser::PIPE_TK:
            return AST::Operators::BINARY_BITWISE_OR;
        case LogiaParser::RIGHT_SHIFT_ASSIGN_TK:
            return AST::Operators::BINARY_BITWISE_LEFT_SHIFT_ASSIGN;
        case LogiaParser::LEFT_SHIFT_ASSIGN_TK:
            return AST::Operators::BINARY_BITWISE_RIGHT_SHIFT_ASSIGN;
        case LogiaParser::AND_ASSIGN_TK:
            return AST::Operators::BINARY_BITWISE_AND_ASSIGN;
        case LogiaParser::XOR_ASSIGN_TK:
            return AST::Operators::BINARY_BITWISE_XOR_ASSIGN;
        case LogiaParser::OR_ASSIGN_TK:
            return AST::Operators::BINARY_BITWISE_OR_ASSIGN;
        case LogiaParser::MOD_ASSIGN_TK:
            return AST::Operators::BINARY_MOD_ASSIGN;

        /// Additive
        case LogiaParser::PLUS_TK:
            return AST::Operators::BINARY_ADD;
        case LogiaParser::MINUS_TK:
            return AST::Operators::BINARY_SUB;

        default:
            CST_UNREACHABLE();
        }
    }

    CST2AST::CST2AST(logia::AST::File *_file) : program(_file)
    {
    }

    std::any CST2AST::visitProgram(LogiaParser::ProgramContext *context)
    {
        CST_DEBUG_FUNCTION();
        this->program->loc = MAKE_LOCATION(context);
        return this->visitChildren(context);
    }

    //
    // Expressions
    //
    std::any CST2AST::visitIntegerLiteral(LogiaParser::IntegerLiteralContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto text = context->getText();
        return ANY_VOIDP_STORE(new AST::IntegerLiteral(MAKE_LOCATION(context), text.c_str()));
    }

    std::any CST2AST::visitFloatLiteral(LogiaParser::FloatLiteralContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto text = context->getText();
        return ANY_VOIDP_STORE(new AST::FloatLiteral(MAKE_LOCATION(context), text.c_str()));
    }

    // rhsExpr it's just a container not needed
    std::any CST2AST::visitRhsExpr(LogiaParser::RhsExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_VISIT_BRANCH(errorHandlingExprs, visitErrorHandlingExprs);
        CST_VISIT_BRANCH(conditionalExpr, visitConditionalExpr);
        CST_VISIT_BRANCH(tokenizeExpr, visitTokenizeExpr);
        CST_VISIT_BRANCH(anonymousfunctionDecl, visitAnonymousfunctionDecl);

        throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
    }
    std::any CST2AST::visitAssignmentExpr(LogiaParser::AssignmentExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_TODO_BRANCH(errorHandlingExprs, ErrorHandlingExprs);
        CST_VISIT_BRANCH(conditionalExpr, visitConditionalExpr);

        // right associative
        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitUnaryExpr(context->left));
        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAssignmentExpr(context->right));

        return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, tk_to_operator(context->op->start->getType()), right));
    }

    std::any CST2AST::visitConditionalExpr(LogiaParser::ConditionalExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        LOGIA_VERIFY(context->true_expr == nullptr, "TO-DO: not supported");
        LOGIA_VERIFY(context->false_expr == nullptr, "TO-DO: not supported");

        return this->visitLogicalOrExpr(context->condition);
    }
    std::any CST2AST::visitLogicalOrExpr(LogiaParser::LogicalOrExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalAndExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_LOGICAL_OR, right));
        }
        return ANY_VOIDP_STORE(left);
    }
    std::any CST2AST::visitLogicalAndExpr(LogiaParser::LogicalAndExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitInclusiveOrExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalAndExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_LOGICAL_AND, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitInclusiveOrExpr(LogiaParser::InclusiveOrExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitExclusiveOrExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitInclusiveOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_BITWISE_OR, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitExclusiveOrExpr(LogiaParser::ExclusiveOrExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitAndExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitExclusiveOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_BITWISE_XOR, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitAndExpr(LogiaParser::AndExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitEqualityExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAndExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_BITWISE_AND, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitEqualityExpr(LogiaParser::EqualityExprContext *context)
    {
        // right associative
        CST_DEBUG_FUNCTION();

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitRelationalExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitEqualityExpr(context->left));

            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, tk_to_operator(context->op->start->getType()), right));
            // TODO:  '<' '>'
            throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitRelationalExpr(LogiaParser::RelationalExprContext *context)
    {
        // right associative
        CST_DEBUG_FUNCTION();

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitShiftExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitRelationalExpr(context->left));

            switch (context->op->start->getType())
            {
            case LogiaParser::LT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_COMPARISON_LT, right));
            case LogiaParser::LESS_EQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_COMPARISON_LTE, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_COMPARISON_GT, right));
            case LogiaParser::GREATER_EQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_COMPARISON_GTE, right));
            }
            throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitShiftExpr(LogiaParser::ShiftExprContext *context)
    {
        // right associative
        CST_DEBUG_FUNCTION();

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAdditiveExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitShiftExpr(context->left));
            // NOTE checking the first token is enough
            switch (context->op->start->getType())
            {
            case LogiaParser::LT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_BITWISE_LEFT_SHIFT, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_BITWISE_RIGHT_SHIFT, right));
            }
            throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitAdditiveExpr(LogiaParser::AdditiveExprContext *context)
    {
        // right associative
        CST_DEBUG_FUNCTION();

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitMultiplicativeExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitAdditiveExpr(context->left));

            return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, tk_to_operator(context->op->start->getType()), right));
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitMultiplicativeExpr(LogiaParser::MultiplicativeExprContext *context)
    {
        // right associative
        CST_DEBUG_FUNCTION();

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitCastExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitMultiplicativeExpr(context->left));

            switch (context->op->start->getType())
            {
            case LogiaParser::STAR_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_MUL, right));
            case LogiaParser::SLASH_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_DIV, right));
            case LogiaParser::MOD_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(MAKE_LOCATION(context), left, AST::Operators::BINARY_MOD, right));
            }
            throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitCastExpr(LogiaParser::CastExprContext *context)
    {
        CST_DEBUG_FUNCTION();
        // TODO syntax not final
        return this->visitUnaryExpr(context->left);
    }

    std::any CST2AST::visitUnaryExpr(LogiaParser::UnaryExprContext *context)
    {
        CST_DEBUG_FUNCTION();
        CST_VISIT_BRANCH(unaryNewExpression, visitUnaryNewExpression);
        CST_VISIT_BRANCH(unaryDeleteExpression, visitUnaryDeleteExpression);
        CST_VISIT_BRANCH(unaryCloneExpression, visitUnaryCloneExpression);
        CST_VISIT_BRANCH(unaryMockExpr, visitUnaryMockExpr);

        // auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->operand)));
        auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->operand));

        if (context->op != nullptr)
        {
            switch (context->op->start->getType())
            {
            // UNDER REVIEW
            case LogiaParser::AT_TK:
            case LogiaParser::AND_TK:
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_REFERENCE, operand));

            case LogiaParser::PLUS_TK:
                // for syntactic completeness, do nothing
                return operand;
            case LogiaParser::MINUS_TK:
                // if it's a number, we negate the string
                // we cannot do it later, because some numbers are out-of-range as positives!
                if (operand->is<AST::IntegerLiteral>())
                {
                    operand->as<AST::IntegerLiteral>()->negate();
                    return ANY_VOIDP_STORE(operand);
                }
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_NEGATION, operand));
            case LogiaParser::TILDE_TK:
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_BITWISE_NOT, operand));
            case LogiaParser::NOT_TK:
            case LogiaParser::NOT2_TK:
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_LOGICAL_NOT, operand));
            case LogiaParser::PLUSPLUS_TK:
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_INCREMENT, operand));
            case LogiaParser::MINUSMINUS_TK:
                return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::PREFIX_DECREMENT, operand));
            }
            throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
        }

        return ANY_VOIDP_STORE(operand);
    }

    std::any CST2AST::visitPostfixExpr(LogiaParser::PostfixExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        if (context->expr1 != nullptr)
        {
            // postfixBracesMemberAccessExpr
            CST_THROW("todo");
        }
        else if (context->expr2 != nullptr)
        {
            // postfixDotMemberAccessExpr
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->expr2));
            LOGIA_VERIFY(context->identifierName()->keywords() == nullptr, "TO-DO");
            auto right = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifierName()->identifier()));
            return ANY_VOIDP_STORE(new AST::MemberAccessExpression(MAKE_LOCATION(context), left, right));
        }
        else if (context->expr3 != nullptr)
        {
            // postfixSliceExpr
            CST_THROW("todo");
        }
        else if (context->expr4 != nullptr)
        {
            // rangeExpr
            CST_THROW("todo");
        }
        else if (context->expr5 != nullptr)
        {
            // postfixCallExprExpr
            return this->postfixCallExpr(context->expr5, context->arguments);
        }
        else if (context->expr6 != nullptr)
        {
            // preprocessorMemberMacroCallExpr
            CST_THROW("todo");
        }
        else if (context->expr7 != nullptr)
        {
            // postfixIndecrementExpr
            for (int i = 0;; ++i)
            {
                if (context->PLUSPLUS_TK() != nullptr)
                {
                    auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->expr7));
                    return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::POSTFIX_INCREMENT, operand));
                }
                if (context->MINUSMINUS_TK() != nullptr)
                {
                    auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->expr7));
                    return ANY_VOIDP_STORE(new AST::UnaryExpression(MAKE_LOCATION(context), AST::Operators::POSTFIX_DECREMENT, operand));
                }
                CST_UNREACHABLE();
            }
        }
        else
        {
            // primaryExpr
            return this->visitPrimaryExpr(context->primaryExpr());
        }

        CST_UNREACHABLE();
    }

    std::any CST2AST::postfixCallExpr(LogiaParser::PostfixExprContext *locator, LogiaParser::ArgumentExprListContext *arguments)
    {
        auto locator_expr = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(locator));
        auto callexpr = new AST::CallExpression(MAKE_LOCATION(locator), locator_expr, {});

        LOG(VRB, "locator {}", locator_expr->to_string());
        if (arguments != nullptr)
        {
            parseArguments(callexpr, arguments);
        }

        return ANY_VOIDP_STORE(callexpr);
    }

    void CST2AST::parseArguments(AST::CallExpression *callexpr, LogiaParser::ArgumentExprListContext *arguments)
    {
        // NAMED
        LogiaParser::NamedArgumentContext *namedArg;

        for (int i = 0;; ++i)
        {
            namedArg = arguments->namedArgument(i);
            if (namedArg == nullptr)
            {
                break;
            }
            callexpr->push_named_argument(
                ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(namedArg->name)), ANY_VOIDP_CAST(AST::Expression *, this->visitConditionalExpr(namedArg->expr)));
        }

        // POSITIONAL
        LogiaParser::PositionalArgumentContext *posArg;
        for (int i = 0;; ++i)
        {
            posArg = arguments->positionalArgument(i);
            if (posArg == nullptr)
            {
                break;
            }
            AST::Expression *expr;
            auto cond = posArg->conditionalExpr();
            auto anomfn = posArg->anonymousfunctionDecl();
            if (cond != nullptr)
            {
                expr = ANY_VOIDP_CAST(AST::Expression *, this->visitConditionalExpr(cond));
                LOG(VRB, "arg[{}] = {}", i, expr->to_string());
            }
            else if (anomfn != nullptr)
            {
                throw std::runtime_error("not supportted yet: anonymous function");
            }
            else
            {
                CST_UNREACHABLE();
            }

            callexpr->push_positional_argument(expr);
        }
    }

    std::any CST2AST::visitPrimaryExpr(LogiaParser::PrimaryExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_VISIT_BRANCH(mayBeConstant, visitMayBeConstant);
        CST_VISIT_BRANCH(groupExpr, visitGroupExpr);

        if (context->typeDefinition() != nullptr)
        {
            CST_THROW("typeDefinition - todo");
        }

        CST_UNREACHABLE();
    }

    std::any CST2AST::visitGroupExpr(LogiaParser::GroupExprContext *context)
    {
        CST_VISIT_BRANCH(expression, visitExpression);
        CST_UNREACHABLE();
    }

    std::any CST2AST::visitMayBeConstant(LogiaParser::MayBeConstantContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_VISIT_BRANCH(constant, visitConstant);
        CST_VISIT_BRANCH(structConstantInitializer, visitStructConstantInitializer);

        if (context->arrayInitializer() != nullptr)
        {
        }
        else if (context->structInitializer() != nullptr)
        {
        }

        throw std::runtime_error(TOSTRING(__FUNCTION__) " todo");
    }
    std::any CST2AST::visitStructConstantInitializer(LogiaParser::StructConstantInitializerContext *context)
    {
        auto sinit = new AST::StructInitializer(MAKE_LOCATION(context));
        auto list = context->structProperyInitializerList();
        if (list != nullptr)
        {
            for (int i = 0;; ++i)
            {
                auto prop = list->structProperyInitializer(i);
                if (prop == nullptr)
                {
                    break;
                }

                auto value = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(prop->value));
                if (prop->locator != nullptr)
                {
                    // auto locator = ANY_VOIDP_CAST(AST::TypeDef *, this->visitTypeLocator(prop->locator));
                    auto locator = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(prop->locator));

                    sinit->add_named_property(locator, value);
                }
                else
                {
                    sinit->add_positional_property(value);
                }
            }
        }

        return ANY_VOIDP_STORE(sinit);
    }

    std::any CST2AST::visitConstant(LogiaParser::ConstantContext *context)
    {
        CST_DEBUG_FUNCTION();
        // token literals, like true/false
        switch (context->start->getType())
        {
        case LogiaParser::TRUE_TK:
            return ANY_VOIDP_STORE(new AST::IntegerLiteral(MAKE_LOCATION(context), "1", scope_look_one<AST::Type>(this->program, "bool")));
        case LogiaParser::FALSE_TK:
            return ANY_VOIDP_STORE(new AST::IntegerLiteral(MAKE_LOCATION(context), "0", scope_look_one<AST::Type>(this->program, "bool")));
        case LogiaParser::NULL_TK:
            throw std::runtime_error(TOSTRING(__FUNCTION__) " todo");
        case LogiaParser::DEFAULT_TK:
            throw std::runtime_error(TOSTRING(__FUNCTION__) " todo");
        }

        CST_VISIT_BRANCH(integerLiteral, visitIntegerLiteral);
        CST_VISIT_BRANCH(floatLiteral, visitFloatLiteral);
        CST_VISIT_BRANCH(identifier, visitIdentifier);
        CST_VISIT_BRANCH(stringLiteral, visitStringLiteral);

        if (context->preprocessorExpr() != nullptr)
        {
        }
        else if (context->regularExpressionLiteral() != nullptr)
        {
        }

        throw std::runtime_error(TOSTRING(__FUNCTION__) " todo");
    }

    std::any CST2AST::visitReturnStmt(LogiaParser::ReturnStmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto rhs = context->rhsExpr();
        LOGIA_VERIFY(rhs != nullptr, "not supportted yet: empty return");
        auto ret_expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        return ANY_VOIDP_STORE(new AST::ReturnStmt(MAKE_LOCATION(context), ret_expr));
    }
    std::any CST2AST::visitLabeledStmt(LogiaParser::LabeledStmtContext *context)
    {
        // identifier ':' (BlockStmt | endOfStmt* blockStmt)
        CST_DEBUG_FUNCTION();

        CST_TODO_BRANCH(stmt, visitStmt);
        auto id = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));

        auto block = new AST::Block(MAKE_LOCATION(context), id);

        this->parseBlock(context->blockStmt(), block);

        return ANY_VOIDP_STORE(block);
    }
    std::any CST2AST::visitGotoStmt(LogiaParser::GotoStmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto id = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));

        return ANY_VOIDP_STORE(new AST::GotoStmt(MAKE_LOCATION(context), id));
    }
    std::any CST2AST::visitIfStmt(LogiaParser::IfStmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto first_if_stmt = context->ifSelectionStmt(0);

        AST::IfStmt *ifstmt;
        auto condition = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(first_if_stmt->expr));
        ifstmt = new AST::IfStmt(MAKE_LOCATION(context), condition);
        this->parseBlock(first_if_stmt->blockStmt(), ifstmt->get_then());

        // (else if)*
        for (int i = 1;; ++i)
        {
            LOG(VRB, "{}", i);
            auto stmt = context->ifSelectionStmt(i);
            if (stmt == nullptr)
            {
                break;
            }

            auto condition2 = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(stmt->expr));
            auto deep_ifstmt = new AST::IfStmt(MAKE_LOCATION(context), condition2);
            this->parseBlock(stmt->blockStmt(), deep_ifstmt->get_then());
            ifstmt->get_else()->push_child(deep_ifstmt);

            auto last = context->children[context->children.size() - 1]; // TODO deep
            auto last_context = dynamic_cast<antlr4::ParserRuleContext *>(last);
            deep_ifstmt->get_continue_block()->loc = deep_ifstmt->get_else()->loc = MAKE_LOCATION(last_context);
        }
        // else
        auto else_stmt = context->elseSelectionStmt();
        auto last = context->children[context->children.size() - 1]; // TODO deep
        if (else_stmt != nullptr)
        {
            this->parseBlock(else_stmt->blockStmt(), ifstmt->get_else());
        }
        else
        {
            // else is optional but the basicblock not, set location
            auto last_context = dynamic_cast<antlr4::ParserRuleContext *>(last);
            ifstmt->get_else()
                ->loc = MAKE_LOCATION(last_context);
        }
        LOGIA_VERIFY(ifstmt->get_then()->loc.file != nullptr);
        LOGIA_VERIFY(ifstmt->get_else()->loc.file != nullptr);
        // REVIEW token ?
        // continue block should be at last "context"
        auto last_context = dynamic_cast<antlr4::ParserRuleContext *>(last);
        ifstmt->get_continue_block()
            ->loc = MAKE_LOCATION(last_context);

        return ANY_VOIDP_STORE(ifstmt);
    }

    std::vector<AST::Identifier *> CST2AST::parseIdentifierChain(LogiaParser::IdentifierChainContext *context)
    {
        auto list = std::vector<AST::Identifier *>();
        for (int i = 0;; ++i)
        {
            auto ident = context->identifier(i);
            if (ident == nullptr)
            {
                break;
            }
            auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(ident));
            list.push_back(name);
        }
        return list;
    }

    std::vector<AST::Identifier *> CST2AST::parseIdentifierList(LogiaParser::IdentifierListContext *context)
    {
        auto list = std::vector<AST::Identifier *>();
        for (int i = 0;; ++i)
        {
            auto ident = context->identifier(i);
            if (ident == nullptr)
            {
                break;
            }
            auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(ident));
            list.push_back(name);
        }
        return list;
    }

    std::any CST2AST::visitImportStmt(LogiaParser::ImportStmtContext *context)
    {
        auto imp = new AST::Import(MAKE_LOCATION(context));
        if (context->locator != nullptr)
        {
            imp->set_package(this->parseIdentifierChain(context->locator));
            imp->set_import_into_scope();
        }
        else if (context->locator2 != nullptr)
        {
            imp->set_package(this->parseIdentifierChain(context->locator2));

            if (context->all != nullptr)
            {
                imp->set_import_all();
            }
            imp->set_import_list(this->parseIdentifierList(context->identifierList()));
        }
        else
        {
            CST_UNREACHABLE();
        }
        return ANY_VOIDP_STORE(imp);
    }

    std::any CST2AST::visitIdentifier(LogiaParser::IdentifierContext *context)
    {
        CST_DEBUG_FUNCTION();

        return ANY_VOIDP_STORE(new AST::Identifier(MAKE_LOCATION(context), _strdup(context->getText().c_str())));
    }

    std::any CST2AST::visitStringLiteral(LogiaParser::StringLiteralContext *context)
    {
        CST_DEBUG_FUNCTION();

        return ANY_VOIDP_STORE(new AST::StringLiteral(MAKE_LOCATION(context), _strdup(context->STRING_LITERAL()->getText().c_str())));
    }

    std::any CST2AST::visitFunctionDecl(LogiaParser::FunctionDeclContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_TODO_BRANCH(anonymousFunctionDef, visitAnonymousFunctionDef);

        auto fn = this->parseFunctionDef(context->functionDef());

        this->parseBlock(context->blockStmt(), fn->get_body());

        return ANY_VOIDP_STORE(fn);
    }
    std::any CST2AST::visitOperatorFunctionDecl(LogiaParser::OperatorFunctionDeclContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto fn = this->parseOperatorDef(context->operatorFunctionDef());
        this->program->push_child(fn);

        this->parseBlock(context->blockStmt(), fn->get_body());

        return ANY_VOIDP_STORE(fn);
    }

    AST::Function *CST2AST::parseFunctionDef(LogiaParser::FunctionDefContext *context)
    {
        auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->functionName()->identifier()));
        auto ret_type = this->parseTypeDefinition(context->return_type);
        auto fn = new AST::Function(MAKE_LOCATION(context), name, ret_type);
        this->parseParameterList(context->parameters, fn);

        return fn;
    }

    AST::Operator *CST2AST::parseOperatorDef(LogiaParser::OperatorFunctionDefContext *context)
    {
        // auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->functionName()->identifier()));
        auto ret_type = this->parseTypeDefinition(context->return_type);
        auto fn = new AST::Operator(MAKE_LOCATION(context), tk_to_operator(context->op->start->getType()), ret_type);
        this->parseParameterList(context->parameters, fn);

        return fn;
    }

    AST::Type *CST2AST::parseTypeDefinition(LogiaParser::TypeDefinitionContext *context)
    {
        if (context == nullptr)
        {
            return new AST::InferType();
        }
        return ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context));
    }

    void CST2AST::parseParameterList(LogiaParser::FunctionParameterListContext *plist, AST::Function *fn)
    {
        if (plist == nullptr)
        {
            return;
        }

        for (int i = 0;; ++i)
        {
            auto param = plist->functionParameter(i);

            if (param == nullptr)
            {
                break;
            }
            auto type_def = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(param->typeDefinition()));
            auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(param->identifier()));
            auto rhs = param->rhsExpr();
            AST::Expression *param_default = nullptr;
            if (rhs != nullptr)
            {
                param_default = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
            }

            // TODO LOGIA_VERIFY(param->functionParametersTypeModifiers().size() > 0);
            // TODO REWIEW accept rhsExpr vs constExpr ???
            fn->push_parameter(new AST::FunctionParameter(name, type_def, (AST::ConstExpression *)param_default));
        }
    }

    std::any CST2AST::visitBlockStmt(LogiaParser::BlockStmtContext *context)
    {
        auto block = new AST::Block(MAKE_LOCATION(context), new AST::Identifier(MAKE_LOCATION(context), ""));

        this->parseBlock(context, block);

        return ANY_VOIDP_STORE(block);
    }

    void CST2AST::parseBlock(LogiaParser::BlockStmtContext *context, AST::Block *block)
    {
        // some blocks are created inside decl or stmt, we override rule here so they have proper location!
        block->loc = MAKE_LOCATION(context);

        if (context == nullptr)
        {
            LOG(VRB, "{}", "empty block");
            return;
        }

        CST_DEBUG_FUNCTION();

        for (int i = 0;; ++i)
        {
            auto stmt = context->stmt(i);
            if (stmt == nullptr)
            {
                break;
            }
            LOG(VRB, "stmt[{}] = {}", i, stmt->getText());

            auto any_node = visitStmt(stmt);
            try
            {
                auto node = ANY_VOIDP_CAST(AST::Node *, any_node);
                LOG(VRB, "stmt[{}] = {} = {}", i, (void *)node, (node != nullptr ? node->to_string() : ""));
                if (node != nullptr)
                {
                    block->push_child(node);
                }
            }
            catch (std::exception e)
            {
                LOG_ERR("{}\n{}", stmt->toStringTree(), e.what());
                throw e;
            }
        }
    }

    std::any CST2AST::visitProgramStmt(LogiaParser::ProgramStmtContext *context)
    {
        auto functionDecl = context->functionDecl();
        if (functionDecl != nullptr)
        {
            // TODO where we attach this function ?!
            auto fn = ANY_VOIDP_CAST(AST::Function *, this->visitFunctionDecl(functionDecl));
            this->program->push_child(fn);
            return fn;
        }

        auto typeDecl = context->typeDecl();
        if (typeDecl != nullptr)
        {
            // TODO where we attach this function ?!
            auto ty = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDecl(typeDecl));
            this->program->push_child(ty);
            return ty;
        }

        CST_UNREACHABLE();
    }

    std::any CST2AST::visitStmt(LogiaParser::StmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_VISIT_BRANCH(labeledStmt, visitLabeledStmt);
        CST_VISIT_BRANCH(importStmt, visitImportStmt);

        CST_TODO_BRANCH(globalImportVar, visitGlobalImportVar);
        CST_VISIT_BRANCH(blockStmt, visitBlockStmt);
        CST_VISIT_BRANCH(aliasDeclStmt, visitAliasDeclStmt);
        CST_VISIT_BRANCH(typeDecl, visitTypeDecl);

        // CST_VISIT_BRANCH2(functionDecl, visitFunctionDecl, AST::Function, this->program->push_child(node););

        auto functionDecl = context->functionDecl();
        if (functionDecl != nullptr)
        {
            // TODO where we attach this function ?!
            auto fn = ANY_VOIDP_CAST(AST::Function *, this->visitFunctionDecl(functionDecl));
            this->program->push_child(fn);
            return fn;
        }

        CST_VISIT_BRANCH(operatorFunctionDecl, visitOperatorFunctionDecl);

        CST_VISIT_BRANCH(selectionStmts, visitSelectionStmts);
        // function exclusive
        CST_VISIT_BRANCH(returnStmt, visitReturnStmt);
        CST_VISIT_BRANCH(deferStmt, visitDeferStmt);
        CST_VISIT_BRANCH(blockVariableDeclStmt, visitBlockVariableDeclStmt);
        CST_VISIT_BRANCH(errorHandlingStmts, visitErrorHandlingStmts);
        CST_VISIT_BRANCH(retryUntilWhileStmt, visitRetryUntilWhileStmt);
        // preprocessor
        CST_VISIT_BRANCH(preprocessorStmts, visitPreprocessorStmts);
        // expression at the bottom to fix some preprocessor issues
        CST_VISIT_BRANCH(expression, visitExpression);

        // empty stmt
        CST_VISIT_BRANCH(endOfStmt, visitEndOfStmt);

        throw std::runtime_error(TOSTRING(__FUNCTION__) "unreachable");
    }

    std::any CST2AST::visitEndOfStmt(LogiaParser::EndOfStmtContext *context)
    {
        return ANY_VOIDP_STORE(new AST::NoOp());
    }

    //
    // variables
    //
    std::any CST2AST::visitInferVariableDeclStmt(LogiaParser::InferVariableDeclStmtContext *context)
    {
        CST_DEBUG_FUNCTION();
        auto ident = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));
        AST::Expression *expr = nullptr;

        auto rhs = context->rhsExpr();
        if (rhs != nullptr)
        {
            expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        }

        return ANY_VOIDP_STORE(new AST::VarDeclStmt(MAKE_LOCATION(context), ident, nullptr, expr));
    }
    std::any CST2AST::visitTypedVariableDeclStmt(LogiaParser::TypedVariableDeclStmtContext *context)
    {
        CST_DEBUG_FUNCTION();
        auto ident = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));
        AST::Expression *expr = nullptr;

        auto rhs = context->rhsExpr();
        auto constructor_arguments = context->argumentExprList();
        if (rhs != nullptr)
        {
            expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        }
        else if (constructor_arguments != nullptr)
        {
            auto locator = new AST::Identifier(MAKE_LOCATION(context), _strdup("new"));
            auto callexpr = new AST::CallExpression(MAKE_LOCATION(context), locator, {});
            this->parseArguments(callexpr, constructor_arguments);
            expr = callexpr;
        }
        else
        {
            CST_UNREACHABLE();
        }

        auto type_def = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context->typeDefinition()));

        return ANY_VOIDP_STORE(new AST::VarDeclStmt(MAKE_LOCATION(context), ident, type_def, expr));
    }

    //
    // types
    //
    std::any CST2AST::visitTypeDecl(LogiaParser::TypeDeclContext *context)
    {
        CST_DEBUG_FUNCTION();
        auto type_name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->name));
        // types that support templates
        CST_TODO_BRANCH(templateDefinition, visitTemplateDefinition);

        if (context->structTypeDecl())
        {
            auto structure = this->parseStructTypeDecl(type_name, context->structTypeDecl());

            return ANY_VOIDP_STORE(structure);
        }

        CST_TODO_BRANCH(interfaceTypeDecl, visitInterfaceTypeDecl);
        CST_TODO_BRANCH(anonymousFunctionDef, visitAnonymousFunctionDef);
        CST_TODO_BRANCH(aggregateTypeDecl, visitAggregateTypeDecl);
        CST_TODO_BRANCH(aliasTypeDecl, visitAliasTypeDecl);

        // types that DON'T support templates

        CST_UNREACHABLE();
    }
    // std::any CST2AST::visitStructTypeDecl(LogiaParser::StructTypeDeclContext *context)
    AST::Struct *CST2AST::parseStructTypeDecl(AST::Identifier *name, LogiaParser::StructTypeDeclContext *context)
    {
        CST_DEBUG_FUNCTION();
        CST_TODO_BRANCH_LIST(typeExtendsDecl, visitTypeExtendsDecl);
        CST_TODO_BRANCH_LIST(typeImplementsDecl, visitTypeImplementsDecl);

        auto structure = new AST::Struct(MAKE_LOCATION(context), name);

        for (int i = 0;; ++i)
        {
            auto property = context->structProperty(i);
            if (property == nullptr)
            {
                break;
            }

            this->parseStructProperty(property, structure);
        }

        return structure;
    }
    std::any CST2AST::visitStructProperty(LogiaParser::StructPropertyContext *context)
    {
        CST_UNREACHABLE();
    }

    void CST2AST::parseStructProperty(LogiaParser::StructPropertyContext *context, AST::Struct *structure)
    {
        CST_DEBUG_FUNCTION();

        if (context->comments() != nullptr)
        {
            // CST_IGNORED();
            return;
        }
        else if (context->structPropertyDecl() != nullptr)
        {
            return this->parseStructPropertyDecl(context->structPropertyDecl(), structure);
        }
        else if (context->endOfStmt())
        {
            // CST_IGNORED();
            return;
        }
        CST_UNREACHABLE();
    }
    std::any CST2AST::visitStructPropertyDecl(LogiaParser::StructPropertyDeclContext *context)
    {
        CST_UNREACHABLE();
    }
    void CST2AST::parseStructPropertyDecl(LogiaParser::StructPropertyDeclContext *context, AST::Struct *structure)
    {
        CST_DEBUG_FUNCTION();

        if (context->identifierName() != nullptr)
        {
            auto type = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context->typeDefinition()));
            LOGIA_VERIFY(context->identifierName()->identifier() != nullptr, "TO-DO");
            auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifierName()->identifier()));

            auto rhs = context->rhsExpr();
            AST::Expression *default_value = nullptr;
            if (rhs != nullptr)
            {
                default_value = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
            }
            structure->add_field(MAKE_LOCATION(context), name, type, default_value, "");
            return;
        }

        auto operatorDecl = context->operatorFunctionDecl();
        if (operatorDecl != nullptr)
        {
            auto def = operatorDecl->operatorFunctionDef();

            auto name = new AST::Identifier(MAKE_LOCATION(def->op), AST::ast_operator_to_function_name(tk_to_operator(def->op->getStart()->getType())));
            auto ret_type = parseTypeDefinition(def->return_type);
            auto fn = new AST::Function(MAKE_LOCATION(def), name, ret_type);
            this->parseParameterList(def->parameters, fn);

            this->program->push_child(fn);

            this->parseBlock(operatorDecl->blockStmt(), fn->get_body());

            return;
        }

        auto propertyAlias = context->propertyAlias();
        if (propertyAlias != nullptr)
        {
            auto from = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(propertyAlias->from));
            auto to = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(propertyAlias->to));
            structure->add_alias(MAKE_LOCATION(propertyAlias), from, to, "");
            return;
        }

        auto functionDecl = context->functionDecl();
        if (functionDecl != nullptr)
        {
            auto fn = ANY_VOIDP_CAST(AST::Function *, this->visitFunctionDecl(functionDecl));
            structure->add_method(fn);
            return;
        }

        CST_TODO_BRANCH(memoryFunctionDecl, MemoryFunctionDecl);
        CST_TODO_BRANCH(structGetterDecl, StructGetterDecl);
        CST_TODO_BRANCH(structSetterDecl, StructSetterDecl);
        CST_UNREACHABLE();
    }

    std::any CST2AST::visitTypeDefinition(LogiaParser::TypeDefinitionContext *context)
    {
        CST_DEBUG_FUNCTION();
        auto tdef = new AST::TypeDef();

        for (int i = 0;; ++i)
        {
            auto mod = context->typeModifiers(i);
            if (mod == nullptr)
            {
                break;
            }
            if (mod->LEND_TK() != nullptr)
            {
                tdef->lend_memory = true;
                continue;
            }
            else if (mod->OWN_TK() != nullptr)
            {
                tdef->own_memory = true;
                continue;
            }
            else if (mod->READONLY_TK() != nullptr)
            {
                tdef->own_memory = true;
                continue;
            }
            else if (mod->UNINITIALIZED_TK() != nullptr)
            {
                tdef->own_memory = true;
                continue;
            }
            CST_UNREACHABLE();
        }

        this->parseTypeLocator(context->typeLocator(), tdef);
        if (context->optional)
        {
            tdef->is_optional = true;
        }

        // TODO
        return ANY_VOIDP_STORE(tdef);
    }
    std::any CST2AST::visitTypeLocator(LogiaParser::TypeLocatorContext *context)
    {
        CST_DEBUG_FUNCTION();
        auto tdef = new AST::TypeDef();

        this->parseTypeLocator(context, tdef);

        return ANY_VOIDP_STORE(tdef);
    }

    void CST2AST::parseTypeLocator(LogiaParser::TypeLocatorContext *context, AST::TypeDef *tdef)
    {
        CST_DEBUG_FUNCTION();
        // TODO
        if (context->loc1 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc2 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc3 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc4 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc5 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc6 != nullptr)
        {
            CST_THROW("todo");
        }
        else if (context->loc7 != nullptr)
        {
            tdef->add_locator(ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->loc7)));
            return;
        }

        CST_UNREACHABLE();
    }
    std::any CST2AST::visitType(LogiaParser::TypeContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_THROW("todo");
    }

    // Fallback: delegate to children
    antlrcpp::Any CST2AST::visitChildren(antlr4::tree::ParseTree *node)
    {
        std::any result = 0;
        for (size_t i = 0; i < node->children.size(); i++)
        {
            result = visit(node->children[i]);
        }
        return result;
    }
}
