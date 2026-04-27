#include "logia/cst2ast.h"

#include "ast/type.h"
#include "ast/constexpr.h"
#include "ast/if_stmt.h"
#include "ast/import.h"

#define CST_THROW(msg)                                         \
    do                                                         \
    {                                                          \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl; \
        throw std::runtime_error(__FUNCTION__ " " msg);        \
    } while (false)

#define CST_TODO_BRANCH(retrieve_method, visit_method) \
    do                                                 \
    {                                                  \
        auto x = context->retrieve_method();           \
        if (x != nullptr)                              \
        {                                              \
            CST_THROW("todo");                         \
        }                                              \
    } while (false)

#define CST_TODO_BRANCH_LIST(retrieve_method, visit_method) \
    do                                                      \
    {                                                       \
        auto x = context->retrieve_method(0);               \
        if (x != nullptr)                                   \
        {                                                   \
            CST_THROW("todo");                              \
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

#define CST_DEBUG_FUNCTION() \
    DEBUG() << context->getText() << std::endl;

#define ANY_VOIDP_STORE(expr) (void *)(expr)
#define ANY_VOIDP_CAST(type, expr) (type)(std::any_cast<void *>(expr))

namespace logia
{
    CST2AST::CST2AST(logia::AST::Program *_program) : program(_program)
    {
    }

    std::any CST2AST::visitProgram(LogiaParser::ProgramContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        this->program->rule = context;
        return this->visitChildren(context);
    }

    //
    // Expressions
    //
    std::any CST2AST::visitNumberLiteral(LogiaParser::NumberLiteralContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto text = context->getText();
        return ANY_VOIDP_STORE(new AST::IntegerLiteral(context, text.c_str(), new AST::InferType()));
    }

    // rhsExpr it's just a container not needed
    std::any CST2AST::visitRhsExpr(LogiaParser::RhsExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        CST_VISIT_BRANCH(errorHandlingExprs, visitErrorHandlingExprs);
        CST_VISIT_BRANCH(conditionalExpr, visitConditionalExpr);
        CST_VISIT_BRANCH(tokenizeExpr, visitTokenizeExpr);
        CST_VISIT_BRANCH(anonymousfunctionDecl, visitAnonymousfunctionDecl);

        throw std::runtime_error(__FUNCTION__ "unreachable");
    }
    std::any CST2AST::visitAssignmentExpr(LogiaParser::AssignmentExprContext *context)
    {
        CST_DEBUG_FUNCTION();

        CST_TODO_BRANCH(errorHandlingExprs, ErrorHandlingExprs);
        CST_VISIT_BRANCH(conditionalExpr, visitConditionalExpr);

        // right associative
        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitUnaryExpr(context->left));
        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAssignmentExpr(context->right));

        //'%=' |  '<<=' | '>>=' | '&=' | '^=' | '|='
        switch (context->op->start->getType())
        {
        case LogiaParser::EQUAL_TK:
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::ASSIGN, right));
        case LogiaParser::STAR_ASSIGN_TK:
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::MUL_ASSIGN, right));
        case LogiaParser::DIV_ASSIGN_TK:
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::DIV_ASSIGN, right));
        case LogiaParser::ADD_ASSIGN_TK:
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::ADD_ASSIGN, right));
        case LogiaParser::SUB_ASSIGN_TK:
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::SUB_ASSIGN, right));

        // case RIGHT_SHIFT_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_LEFT_SHIFT_ASSIGN, right));
        // case LEFT_SHIFT_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_RIGHT_SHIFT_ASSIGN, right));
        // case AND_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_AND_ASSIGN, right));
        // case XOR_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_XOR_ASSIGN, right));
        // case OR_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_OR_ASSIGN, right));
        // case LogiaParser::MOD_ADDIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::MOD_ASSIGN, right));
        // case LogiaParser::OR_ASSIGN_TK:
        // return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::OR, right));
        default:
            CST_UNREACHABLE();
        }
    }

    std::any CST2AST::visitConditionalExpr(LogiaParser::ConditionalExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        LOGIA_ASSERT(context->true_expr != nullptr, "not supported");
        LOGIA_ASSERT(context->false_expr != nullptr, "not supported");

        return this->visitLogicalOrExpr(context->condition);
    }
    std::any CST2AST::visitLogicalOrExpr(LogiaParser::LogicalOrExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalAndExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGICAL_OR, right));
        }
        return ANY_VOIDP_STORE(left);
    }
    std::any CST2AST::visitLogicalAndExpr(LogiaParser::LogicalAndExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitInclusiveOrExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitLogicalAndExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGICAL_OR, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitInclusiveOrExpr(LogiaParser::InclusiveOrExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitExclusiveOrExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitInclusiveOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_OR, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitExclusiveOrExpr(LogiaParser::ExclusiveOrExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitAndExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitExclusiveOrExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_XOR, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitAndExpr(LogiaParser::AndExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitEqualityExpr(context->left));

        if (context->right != nullptr)
        {
            auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAndExpr(context->right));
            return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_AND, right));
        }
        return ANY_VOIDP_STORE(left);
    }

    std::any CST2AST::visitEqualityExpr(LogiaParser::EqualityExprContext *context)
    {
        // right associative
        DEBUG() << context->getText() << std::endl;

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitRelationalExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitEqualityExpr(context->left));

            switch (context->op->start->getType())
            {
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
            // value equality
            case LogiaParser::EQUALEQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_EQ, right));
            // value inequality
            case LogiaParser::NOT_EQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_NEQ, right));
            }
            // TODO:  '<' '>'
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitRelationalExpr(LogiaParser::RelationalExprContext *context)
    {
        // right associative
        DEBUG() << context->getText() << std::endl;

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitShiftExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitRelationalExpr(context->left));

            switch (context->op->start->getType())
            {
            case LogiaParser::LT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_LT, right));
            case LogiaParser::LESS_EQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_LTE, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_GT, right));
            case LogiaParser::GREATER_EQUAL_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::LOGIAL_GTE, right));
            }
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitShiftExpr(LogiaParser::ShiftExprContext *context)
    {
        // right associative
        DEBUG() << context->getText() << std::endl;

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitAdditiveExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitShiftExpr(context->left));
            // NOTE checking the first token is enough
            switch (context->op->start->getType())
            {
            case LogiaParser::LT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_LEFT_SHIFT, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::BITWISE_RIGHT_SHIFT, right));
            }
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitAdditiveExpr(LogiaParser::AdditiveExprContext *context)
    {
        // right associative
        DEBUG() << context->getText() << std::endl;

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitMultiplicativeExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitAdditiveExpr(context->left));

            switch (context->op->start->getType())
            {
            case LogiaParser::PLUS_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::ADD, right));
            case LogiaParser::MINUS_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::SUB, right));
            }
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitMultiplicativeExpr(LogiaParser::MultiplicativeExprContext *context)
    {
        // right associative
        DEBUG() << context->getText() << std::endl;

        auto right = ANY_VOIDP_CAST(AST::Expression *, this->visitCastExpr(context->right));

        if (context->left != nullptr)
        {
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitMultiplicativeExpr(context->left));

            switch (context->op->start->getType())
            {
            case LogiaParser::STAR_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::MUL, right));
            case LogiaParser::SLASH_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::DIV, right));
            case LogiaParser::MOD_TK:
                return ANY_VOIDP_STORE(new AST::BinaryExpression(context, left, AST::BinaryOperator::MOD, right));
            }
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }
        return ANY_VOIDP_STORE(right);
    }

    std::any CST2AST::visitCastExpr(LogiaParser::CastExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        // TODO syntax not final
        return this->visitUnaryExpr(context->left);
    }

    std::any CST2AST::visitUnaryExpr(LogiaParser::UnaryExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;
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
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::DEREFERENCE, operand));

            case LogiaParser::PLUS_TK:
                // for syntactic completeness, do nothing
                return operand;
            case LogiaParser::MINUS_TK:
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::NEGATION, operand));
            case LogiaParser::TILDE_TK:
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::BITWISE_NOT, operand));
            case LogiaParser::NOT_TK:
            case LogiaParser::NOT2_TK:
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::LOGICAL_NOT, operand));
            case LogiaParser::PLUSPLUS_TK:
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::INCREMENT, operand));
            case LogiaParser::MINUSMINUS_TK:
                return ANY_VOIDP_STORE(new AST::PrefixUnaryExpression(context, AST::PrefixUnaryOperator::DECREMENT, operand));
            }
            throw std::runtime_error(__FUNCTION__ "unreachable");
        }

        return ANY_VOIDP_STORE(operand);
    }

    std::any CST2AST::visitPostfixExpr(LogiaParser::PostfixExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        if (context->expr1 != nullptr)
        {
            // postfixBracesMemberAccessExpr
            CST_THROW("todo");
        }
        else if (context->expr2 != nullptr)
        {
            // postfixDotMemberAccessExpr
            auto left = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->expr2));
            LOGIA_ASSERT(context->identifierName()->keywords() != nullptr, "TODO");
            auto right = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifierName()->identifier()));
            return ANY_VOIDP_STORE(new AST::MemberAccessExpression(context, left, right));
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
            CST_THROW("todo");
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
        auto callexpr = new AST::CallExpression(locator, locator_expr, {});

        DEBUG() << "locator" << locator_expr->to_string() << std::endl;
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
            callexpr->add_named_argument(
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
                DEBUG() << "arg " << i << expr->to_string() << std::endl;
            }
            else if (anomfn != nullptr)
            {
                throw std::runtime_error("not supportted yet: anonymous function");
            }
            else
            {
                CST_UNREACHABLE();
            }

            callexpr->add_positional_argument(expr);
        }
    }

    std::any CST2AST::visitPrimaryExpr(LogiaParser::PrimaryExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

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
        DEBUG() << context->getText() << std::endl;

        CST_VISIT_BRANCH(constant, visitConstant);
        CST_VISIT_BRANCH(structConstantInitializer, visitStructConstantInitializer);

        if (context->arrayInitializer() != nullptr)
        {
        }
        else if (context->structInitializer() != nullptr)
        {
        }

        throw std::runtime_error(__FUNCTION__ " todo");
    }
    std::any CST2AST::visitStructConstantInitializer(LogiaParser::StructConstantInitializerContext *context)
    {
        auto list = context->structProperyInitializerList();
        auto sinit = new AST::StructInitializer(context);
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
                auto locator = ANY_VOIDP_CAST(AST::TypeDef *, this->visitTypeLocator(prop->locator));

                sinit->add_named_property(locator, value);
            }
            else
            {
                sinit->add_positional_property(value);
            }
        }

        return ANY_VOIDP_STORE(sinit);
    }

    std::any CST2AST::visitConstant(LogiaParser::ConstantContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        // token literals, like true/false
        switch (context->start->getType())
        {
        case LogiaParser::TRUE_TK:
            return ANY_VOIDP_STORE(new AST::IntegerLiteral(context, "1", this->program->look<AST::Type>("bool")));
        case LogiaParser::FALSE_TK:
            return ANY_VOIDP_STORE(new AST::IntegerLiteral(context, "0", this->program->look<AST::Type>("bool")));
        case LogiaParser::NULL_TK:
            throw std::runtime_error(__FUNCTION__ " todo");
        case LogiaParser::DEFAULT_TK:
            throw std::runtime_error(__FUNCTION__ " todo");
        }

        CST_VISIT_BRANCH(numberLiteral, visitNumberLiteral);
        CST_VISIT_BRANCH(identifier, visitIdentifier);
        CST_VISIT_BRANCH(stringLiteral, visitStringLiteral);

        if (context->preprocessorExpr() != nullptr)
        {
        }
        else if (context->regularExpressionLiteral() != nullptr)
        {
        }

        throw std::runtime_error(__FUNCTION__ " todo");
    }

    std::any CST2AST::visitReturnStmt(LogiaParser::ReturnStmtContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto rhs = context->rhsExpr();
        LOGIA_ASSERT(rhs && "not supportted yet: empty return");
        auto ret_expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        return ANY_VOIDP_STORE(new AST::ReturnStmt(context, ret_expr));
    }
    std::any CST2AST::visitLabeledStmt(LogiaParser::LabeledStmtContext *context)
    {
        // identifier ':' (BlockStmt | endOfStmt* blockStmt)
        DEBUG() << context->getText() << std::endl;

        CST_TODO_BRANCH(stmt, visitStmt);
        auto id = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));

        auto block = new AST::Block(context, id);

        this->parseBlock(context->blockStmt(), block);

        return ANY_VOIDP_STORE(block);
    }
    std::any CST2AST::visitGotoStmt(LogiaParser::GotoStmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto id = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));

        return ANY_VOIDP_STORE(new AST::GotoStmt(context, id));
    }
    std::any CST2AST::visitIfStmt(LogiaParser::IfStmtContext *context)
    {
        CST_DEBUG_FUNCTION();

        auto first_if_stmt = context->ifSelectionStmt(0);

        AST::IfStmt *ifstmt;
        auto condition = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(first_if_stmt->expr));
        ifstmt = new AST::IfStmt(context, condition);
        this->parseBlock(first_if_stmt->blockStmt(), ifstmt->get_then());

        // (else if)*
        for (int i = 1;; ++i)
        {
            DEBUG() << i << std::endl;
            auto stmt = context->ifSelectionStmt(i);
            if (stmt == nullptr)
            {
                break;
            }

            auto condition2 = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(stmt->expr));
            auto deep_ifstmt = new AST::IfStmt(context, condition2);
            this->parseBlock(stmt->blockStmt(), deep_ifstmt->get_then());
            ifstmt->get_else()->push_child(deep_ifstmt);

            auto last = context->children[context->children.size() - 1]; // TODO deep
            deep_ifstmt->get_continue_block()->rule = deep_ifstmt->get_else()->rule = dynamic_cast<antlr4::ParserRuleContext *>(last);
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
            ifstmt->get_else()->rule = dynamic_cast<antlr4::ParserRuleContext *>(last);
        }
        LOGIA_ASSERT(ifstmt->get_then()->rule);
        LOGIA_ASSERT(ifstmt->get_else()->rule);
        // REVIEW token ?
        // continue block should be at last "context"
        ifstmt->get_continue_block()->rule = dynamic_cast<antlr4::ParserRuleContext *>(last);

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
        auto imp = new AST::Import(context);
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
    }

    std::any CST2AST::visitIdentifier(LogiaParser::IdentifierContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        // TODO REVIEW strdup necessary ? that memory should have similar life
        return ANY_VOIDP_STORE(new AST::Identifier(context, strdup(context->getText().c_str())));
    }

    std::any CST2AST::visitStringLiteral(LogiaParser::StringLiteralContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        // TODO REVIEW strdup necessary ? that memory should have similar life
        return ANY_VOIDP_STORE(new AST::StringLiteral(context, strdup(context->STRING_LITERAL()->getText().c_str())));
    }

    std::any CST2AST::visitFunctionDecl(LogiaParser::FunctionDeclContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        CST_TODO_BRANCH(anonymousFunctionDef, visitAnonymousFunctionDef);
        auto def = context->functionDef();

        // auto name = (AST::Identifier *)(this->visitIdentifier(def->functionName()->identifier()));
        auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(def->functionName()->identifier()));
        // auto ret_type = this->program->lookup<AST::Type>("λi64");
        AST::Type *ret_type;
        if (def->return_type == nullptr)
        {
            ret_type = new AST::InferType();
        }
        else
        {
            ret_type = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(def->return_type));
        }
        auto fn = new AST::Function(context, name, ret_type);

        auto plist = def->functionParameterList();
        if (plist != nullptr)
        {
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

                // TODO LOGIA_ASSERT(param->functionParametersTypeModifiers().size() > 0);
                // TODO REWIEW accept rhsExpr vs constExpr ???
                fn->add_param(new AST::FunctionParameter(name, type_def, (AST::ConstExpression *)param_default));
            }
        }

        this->program->push_child(fn);

        this->parseBlock(context->blockStmt(), fn->get_body());

        return ANY_VOIDP_STORE(fn);
    }

    std::any CST2AST::visitBlockStmt(LogiaParser::BlockStmtContext *context)
    {
        auto block = new AST::Block(context, new AST::Identifier(context, ""));

        this->parseBlock(context, block);

        return ANY_VOIDP_STORE(block);
    }

    void CST2AST::parseBlock(LogiaParser::BlockStmtContext *context, AST::Block *block)
    {
        // some blocks are created inside decl or stmt, we override rule here so they have proper location!
        block->rule = context;

        if (context == nullptr)
        {
            DEBUG() << "empty block" << std::endl;
            return;
        }

        DEBUG() << context->getText() << block << std::endl;

        for (int i = 0;; ++i)
        {
            DEBUG() << i << std::endl;
            auto stmt = context->stmt(i);
            if (stmt == nullptr)
            {
                break;
            }
            DEBUG() << stmt->getText() << std::endl;
            auto any_node = visitStmt(stmt);
            try
            {
                auto node = ANY_VOIDP_CAST(AST::Node *, any_node);
                DEBUG() << i << node << (node != nullptr ? node->to_string() : "") << std::endl;
                if (node != nullptr)
                {
                    block->push_child(node);
                }
            }
            catch (std::exception e)
            {
                LERROR() << stmt->toStringTree() << std::endl;
                LERROR() << e.what() << std::endl;
                throw e;
            }
        }
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
        CST_VISIT_BRANCH(functionDecl, visitFunctionDecl);
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

        throw std::runtime_error(__FUNCTION__ "unreachable");
    }

    std::any CST2AST::visitEndOfStmt(LogiaParser::EndOfStmtContext *context)
    {
        DEBUG() << std::endl;
        return ANY_VOIDP_STORE(new AST::NoOp());
    }

    //
    // variables
    //
    std::any CST2AST::visitInferVariableDeclStmt(LogiaParser::InferVariableDeclStmtContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        auto ident = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));
        AST::Expression *expr = nullptr;

        auto rhs = context->rhsExpr();
        if (rhs != nullptr)
        {
            expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        }

        return ANY_VOIDP_STORE(new AST::VarDeclStmt(context, ident, nullptr, expr));
    }
    std::any CST2AST::visitTypedVariableDeclStmt(LogiaParser::TypedVariableDeclStmtContext *context)
    {
        DEBUG() << context->getText() << std::endl;
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
            auto locator = new AST::Identifier(context, strdup("new"));
            auto callexpr = new AST::CallExpression(context, locator, {});
            this->parseArguments(callexpr, constructor_arguments);
            expr = callexpr;
        }
        else
        {
            CST_UNREACHABLE();
        }

        auto type_def = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context->typeDefinition()));

        return ANY_VOIDP_STORE(new AST::VarDeclStmt(context, ident, type_def, expr));
    }

    //
    // types
    //
    std::any CST2AST::visitTypeDecl(LogiaParser::TypeDeclContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        auto type_name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->name));
        // types that support templates
        CST_TODO_BRANCH(templateDefinition, visitTemplateDefinition);

        if (context->structTypeDecl())
        {
            auto structure = ANY_VOIDP_CAST(AST::Struct *, this->visitStructTypeDecl(context->structTypeDecl()));
            structure->set_identifier(type_name);

            this->program->push_child(structure); // <-- TODO push to program atm :)
            return ANY_VOIDP_STORE(structure);
        }

        CST_TODO_BRANCH(interfaceTypeDecl, visitInterfaceTypeDecl);
        CST_TODO_BRANCH(anonymousFunctionDef, visitAnonymousFunctionDef);
        CST_TODO_BRANCH(aggregateTypeDecl, visitAggregateTypeDecl);
        CST_TODO_BRANCH(aliasTypeDecl, visitAliasTypeDecl);

        // types that DON'T support templates

        CST_UNREACHABLE();
    }
    std::any CST2AST::visitStructTypeDecl(LogiaParser::StructTypeDeclContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        CST_TODO_BRANCH_LIST(typeExtendsDecl, visitTypeExtendsDecl);
        CST_TODO_BRANCH_LIST(typeImplementsDecl, visitTypeImplementsDecl);

        auto structure = new AST::Struct(context, nullptr);

        for (int i = 0;; ++i)
        {
            auto property = context->structProperty(i);
            if (property == nullptr)
            {
                break;
            }

            this->parseStructProperty(property, structure);
        }

        return ANY_VOIDP_STORE(structure);
    }
    std::any CST2AST::visitStructProperty(LogiaParser::StructPropertyContext *context)
    {
        CST_UNREACHABLE();
    }

    void CST2AST::parseStructProperty(LogiaParser::StructPropertyContext *context, AST::Struct *structure)
    {
        DEBUG() << context->getText() << std::endl;

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
        DEBUG() << context->getText() << std::endl;

        if (context->identifierName() != nullptr)
        {
            auto type = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context->typeDefinition()));
            LOGIA_ASSERT(context->identifierName()->identifier() != nullptr, "TODO");
            auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifierName()->identifier()));

            auto rhs = context->rhsExpr();
            AST::Expression *default_value = nullptr;
            if (rhs != nullptr)
            {
                default_value = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
            }
            structure->add_field(context, name, type, default_value, "");
            return;
        }

        CST_TODO_BRANCH(propertyAlias, PropertyAlias);
        CST_TODO_BRANCH(functionDecl, FunctionDecl); // functionBody
        CST_TODO_BRANCH(memoryFunctionDecl, MemoryFunctionDecl);
        CST_TODO_BRANCH(operatorFunctionDecl, OperatorFunctionDecl);
        CST_TODO_BRANCH(structGetterDecl, StructGetterDecl);
        CST_TODO_BRANCH(structSetterDecl, StructSetterDecl);
        CST_UNREACHABLE();
    }

    std::any CST2AST::visitTypeDefinition(LogiaParser::TypeDefinitionContext *context)
    {
        DEBUG() << context->getText() << std::endl;
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
        DEBUG() << context->getText() << std::endl;
        auto tdef = new AST::TypeDef();

        this->parseTypeLocator(context, tdef);

        return ANY_VOIDP_STORE(tdef);
    }

    void CST2AST::parseTypeLocator(LogiaParser::TypeLocatorContext *context, AST::TypeDef *tdef)
    {
        DEBUG() << context->getText() << std::endl;
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
        DEBUG() << context->getText() << std::endl;

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
