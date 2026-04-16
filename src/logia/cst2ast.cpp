#include "logia/cst2ast.h"

#include "ast/constexpr.h"

#define CST_THROW(msg)                                         \
    do                                                         \
    {                                                          \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl; \
        throw std::runtime_error(__FUNCTION__ msg);            \
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

#define VISIT_FORDWARD(retrieve_method, visit_method) \
    do                                                \
    {                                                 \
        auto stmt = context->retrieve_method();       \
        if (stmt != nullptr)                          \
        {                                             \
            return this->visit_method(stmt);          \
        }                                             \
    } while (false)

#define ANY_VOIDP_STORE(expr) (void *)(expr)
#define ANY_VOIDP_CAST(type, expr) (type)(std::any_cast<void *>(expr))

namespace logia
{

    CST2AST::CST2AST(logia::AST::Program *_program) : program(_program)
    {
        this->block = this->program;
    }

    std::any CST2AST::visitProgram(LogiaParser::ProgramContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        return this->visitChildren(context);
    }

    //
    // Expressions
    //
    std::any CST2AST::visitNumberLiteral(LogiaParser::NumberLiteralContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto text = context->getText();
        return ANY_VOIDP_STORE(AST::ast_create_int_lit(this->program, text.c_str()));
    }

    // rhsExpr it's just a container not needed
    std::any CST2AST::visitRhsExpr(LogiaParser::RhsExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        VISIT_FORDWARD(errorHandlingExprs, visitErrorHandlingExprs);
        VISIT_FORDWARD(conditionalExpr, visitConditionalExpr);
        VISIT_FORDWARD(tokenizeExpr, visitTokenizeExpr);
        VISIT_FORDWARD(anonymousfunctionDecl, visitAnonymousfunctionDecl);

        throw std::runtime_error(__FUNCTION__ "unreachable");
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
            return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGICAL_OR, right));
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
            return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGICAL_OR, right));
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
            return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::BITWISE_OR, right));
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
            return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::BITWISE_XOR, right));
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
            return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::BITWISE_AND, right));
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
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_EQ, right));
            // value inequality
            case LogiaParser::NOT_EQUAL_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_NEQ, right));
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
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_LT, right));
            case LogiaParser::LESS_EQUAL_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_LTE, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_GT, right));
            case LogiaParser::GREATER_EQUAL_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::LOGIAL_GTE, right));
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
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::BITWISE_LEFT_SHIFT, right));
            case LogiaParser::GT_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::BITWISE_RIGHT_SHIFT, right));
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
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::ADD, right));
            case LogiaParser::MINUS_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::SUB, right));
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
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::MUL, right));
            case LogiaParser::SLASH_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::DIV, right));
            case LogiaParser::MOD_TK:
                return ANY_VOIDP_STORE(AST::ast_create_binary_expr(left, AST::BinaryOperator::MOD, right));
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
        VISIT_FORDWARD(unaryNewExpression, visitUnaryNewExpression);
        VISIT_FORDWARD(unaryDeleteExpression, visitUnaryDeleteExpression);
        VISIT_FORDWARD(unaryCloneExpression, visitUnaryCloneExpression);
        VISIT_FORDWARD(unaryMockExpr, visitUnaryMockExpr);

        // auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->operand)));
        auto operand = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(context->operand));

        if (context->op != nullptr)
        {
            switch (context->op->start->getType())
            {
            // UNDER REVIEW
            case LogiaParser::AT_TK:
            case LogiaParser::AND_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::DEREFERENCE, operand));

            case LogiaParser::PLUS_TK:
                // for syntactic completeness, do nothing
                return operand;
            case LogiaParser::MINUS_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::NEGATION, operand));
            case LogiaParser::TILDE_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::BITWISE_NOT, operand));
            case LogiaParser::NOT_TK:
            case LogiaParser::NOT2_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::LOGICAL_NOT, operand));
            case LogiaParser::PLUSPLUS_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::INCREMENT, operand));
            case LogiaParser::MINUSMINUS_TK:
                return ANY_VOIDP_STORE(AST::ast_create_preunary_expr(AST::PrefixUnaryOperator::DECREMENT, operand));
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
            return ANY_VOIDP_STORE(new AST::MemberAccessExpression(nullptr, left, right));
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
        auto ast_locator = ANY_VOIDP_CAST(AST::Expression *, this->visitPostfixExpr(locator));
        auto callexpr = AST::ast_create_call_expr(ast_locator, {});

        DEBUG() << "locator" << ast_locator->to_string() << std::endl;
        parseArguments(callexpr, arguments);

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
                throw std::runtime_error(__FUNCTION__ " unreachable");
            }

            callexpr->add_positional_argument(expr);
        }
    }

    std::any CST2AST::visitPrimaryExpr(LogiaParser::PrimaryExprContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        VISIT_FORDWARD(mayBeConstant, visitMayBeConstant);

        if (context->groupExpr() != nullptr)
        {
        }
        else if (context->typeDefinition() != nullptr)
        {
        }

        throw std::runtime_error(__FUNCTION__ " todo");
    }

    std::any CST2AST::visitMayBeConstant(LogiaParser::MayBeConstantContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        VISIT_FORDWARD(constant, visitConstant);
        VISIT_FORDWARD(structConstantInitializer, visitStructConstantInitializer);

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
            if (prop->locator != nullptr) {
                auto locator = ANY_VOIDP_CAST(AST::TypeDef*, this->visitTypeLocator(prop->locator));

                sinit->add_named_property(locator, value);
            }
            else {
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
            return ANY_VOIDP_STORE(AST::ast_create_int_lit(this->program, "1"));
        case LogiaParser::FALSE_TK:
            return ANY_VOIDP_STORE(AST::ast_create_int_lit(this->program, "0"));
        case LogiaParser::NULL_TK:
            throw std::runtime_error(__FUNCTION__ " todo");
        case LogiaParser::DEFAULT_TK:
            throw std::runtime_error(__FUNCTION__ " todo");
        }

        VISIT_FORDWARD(numberLiteral, visitNumberLiteral);
        VISIT_FORDWARD(identifier, visitIdentifier);
        VISIT_FORDWARD(stringLiteral, visitStringLiteral);

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
        return ANY_VOIDP_STORE(AST::ast_create_return(ret_expr));
    }

    std::any CST2AST::visitIdentifier(LogiaParser::IdentifierContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto ident = AST::ast_create_identifier((char *)context->getText().c_str());
        ident->rule = context;

        return ANY_VOIDP_STORE(ident);
    }

    std::any CST2AST::visitStringLiteral(LogiaParser::StringLiteralContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        auto str = AST::ast_create_string_lit(context->STRING_LITERAL()->getText().c_str());
        str->rule = context;

        return ANY_VOIDP_STORE(str);
    }

    std::any CST2AST::visitFunctionDecl(LogiaParser::FunctionDeclContext *context)
    {
        DEBUG() << context->getText() << std::endl;

        LOGIA_ASSERT(!context->anonymousFunctionDef() && "not implemented");
        auto def = context->functionDef();

        // auto name = (AST::Identifier *)(this->visitIdentifier(def->functionName()->identifier()));
        auto name = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(def->functionName()->identifier()));
        auto ret_type = (AST::Type *)this->program->lookup((char *)"λi64");

        auto fn = AST::ast_create_function_type(name, ret_type);
        this->program->push_child(fn);

        auto old = this->block;
        this->block = fn->get_body();
        // this->block->push_child(ret_expr);
        this->visitFunctionBody(context->functionBody());
        this->block = old;

        return ANY_VOIDP_STORE(fn);
    }

    std::any CST2AST::visitFunctionBody(LogiaParser::FunctionBodyContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        DEBUG() << context->globalImportVarList() << std::endl;
        DEBUG() << context->functionBodyStmtList() << std::endl;

        LOGIA_ASSERT(context->globalImportVarList() != nullptr, "not supportted");

        auto stmt_list = context->functionBodyStmtList();

        for (int i = 0;; ++i)
        {
            DEBUG() << i << std::endl;
            auto stmt = stmt_list->functionBodyStmt(i);
            if (stmt == nullptr)
            {
                break;
            }
            DEBUG() << stmt->getText() << std::endl;
            auto any_node = visitFunctionBodyStmt(stmt);
            try
            {
                auto node = ANY_VOIDP_CAST(AST::Node *, any_node);
                DEBUG() << i << node->to_string() << std::endl;
                this->block->push_child(node);
            }
            catch (std::exception e)
            {
                ERROR() << stmt->toStringTree() << std::endl;
                ERROR() << e.what() << std::endl;
                throw e;
            }
        }

        return nullptr;
    }

    std::any CST2AST::visitFunctionBodyStmt(LogiaParser::FunctionBodyStmtContext *context)
    {

        VISIT_FORDWARD(labeledStatement, visitLabeledStatement);
        VISIT_FORDWARD(blockStatement, visitBlockStatement);
        VISIT_FORDWARD(aliasDeclStmt, visitAliasDeclStmt);
        VISIT_FORDWARD(typeDecl, visitTypeDecl);
        VISIT_FORDWARD(functionDecl, visitFunctionDecl);
        VISIT_FORDWARD(selectionStmts, visitSelectionStmts);
        // function exclusive
        VISIT_FORDWARD(returnStmt, visitReturnStmt);
        VISIT_FORDWARD(deferStmt, visitDeferStmt);
        VISIT_FORDWARD(blockVariableDeclStmt, visitBlockVariableDeclStmt);
        VISIT_FORDWARD(errorHandlingStmts, visitErrorHandlingStmts);
        VISIT_FORDWARD(retryUntilWhileStmt, visitRetryUntilWhileStmt);
        // preprocessor
        VISIT_FORDWARD(preprocessorStmts, visitPreprocessorStmts);
        // expression at the bottom to fix some preprocessor issues
        VISIT_FORDWARD(expression, visitExpression);

        // empty stmt
        VISIT_FORDWARD(endOfStmt, visitEndOfStmt);

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

        auto decl = AST::ast_create_var_decl(ident, nullptr, expr);
        decl->rule = context;

        return ANY_VOIDP_STORE(decl);
    }
    std::any CST2AST::visitTypedVariableDeclStmt(LogiaParser::TypedVariableDeclStmtContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        auto ident = ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->identifier()));
        AST::Expression *expr = nullptr;

        auto rhs = context->rhsExpr();
        if (rhs != nullptr)
        {
            expr = ANY_VOIDP_CAST(AST::Expression *, this->visitRhsExpr(rhs));
        }

        auto constructor_arguments = context->argumentExprList();
        if (constructor_arguments != nullptr)
        {
            auto callexpr = AST::ast_create_call_expr(AST::ast_create_identifier("new"), {});
            this->parseArguments(callexpr, constructor_arguments);
            expr = callexpr;
        }

        auto type_def = ANY_VOIDP_CAST(AST::Type *, this->visitTypeDefinition(context->typeDefinition()));

        auto type = (AST::Type *)this->program->lookup((char *)"λi64");
        // TODO!!!

        auto decl = AST::ast_create_var_decl(ident, type_def, expr);
        decl->rule = context;

        return ANY_VOIDP_STORE(decl);
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
            structure->rule = context;
            this->block->push_child(structure); // <-- TODO REVIEW so strange! program should push child stmts!!!
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

        auto structure = AST::ast_create_struct_type(nullptr);

        for (int i = 0;; ++i)
        {
            auto property = context->structProperty(i);
            if (property == nullptr)
            {
                break;
            }

            this->visitStructProperty(property, structure);
        }

        return ANY_VOIDP_STORE(structure);
    }
    std::any CST2AST::visitStructProperty(LogiaParser::StructPropertyContext *context)
    {
        CST_UNREACHABLE();
    }

    void CST2AST::visitStructProperty(LogiaParser::StructPropertyContext *context, AST::Struct *structure)
    {
        DEBUG() << context->getText() << std::endl;

        if (context->comments() != nullptr)
        {
            // CST_IGNORED();
            return;
        }
        else if (context->structPropertyDecl() != nullptr)
        {
            return this->visitStructPropertyDecl(context->structPropertyDecl(), structure);
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
    void CST2AST::visitStructPropertyDecl(LogiaParser::StructPropertyDeclContext *context, AST::Struct *structure)
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
        CST_TODO_BRANCH(functionDef, FunctionDef); // functionBody
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

        this->visitTypeLocator(context->typeLocator(), tdef);
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

        this->visitTypeLocator(context, tdef);

        return ANY_VOIDP_STORE(tdef);
    }

    void CST2AST::visitTypeLocator(LogiaParser::TypeLocatorContext *context, AST::TypeDef *tdef)
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
            tdef->push_child(ANY_VOIDP_CAST(AST::Identifier *, this->visitIdentifier(context->loc7)));
            return;
        }

        CST_UNREACHABLE();
    }
    std::any CST2AST::visitType(LogiaParser::TypeContext *context)
    {
        DEBUG() << context->getText() << std::endl;
        // TODO
        return nullptr;
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
