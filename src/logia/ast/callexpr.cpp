#include "logia/ast/callexpr.h"
#include "logia/ast/expr.h"
#include "logia/ast/llvm.h"
#include "logia/ast/memberaccessexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/function.h"

namespace logia::AST
{
    //
    // CallExpressionArgument
    //

    CallExpressionArgument::CallExpressionArgument(
        size_t index,
        Identifier *name,
        Expression *value) : Node(value->rule)
    {
        this->has_type = false;
        this->skip_type_inference = true;

        this->push_child(name);
        name->skip_codegen = true;
        name->skip_type_inference = true;
        name->has_type = false;
        this->push_child(value);
    }

    bool CallExpressionArgument::is_named()
    {
        return strlen(this->get_name()->identifier) > 0;
    }

    Identifier *CallExpressionArgument::get_name()
    {
        return this->get_child<Identifier>(0);
    }

    Expression *CallExpressionArgument::get_value()
    {
        return this->get_child<Expression>(1);
    }

    std::string CallExpressionArgument::to_string()
    {
        return std::format("CallExpressionArgument");
    }

    Type *CallExpressionArgument::get_type()
    {
        return this->get_value()->get_type();
    }

    void CallExpressionArgument::_set_type(Type *type)
    {
        return this->get_value()->set_type(type);
    }

    void CallExpressionArgument::_post_type_inference()
    {
        this->is_typed = this->get_value()->is_typed;
        Node::_post_type_inference();
    }

    //
    // CallExpression
    //
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule) : Expression(rule)
    {
    }
    CallExpression::CallExpression(antlr4::ParserRuleContext *rule, Expression *locator, std::vector<Expression *> positional_arguments) : Expression(rule)
    {
        LOGIA_ASSERT(locator && "locator is mantadory");

        // these two rules are couple atm, but we should handle identifiers in other ways in the future...
        node_assert<Identifier, MemberAccessExpression>(locator, __FUNCTION__ ":" TOSTRING(__LINE__));
        locator->skip_type_inference = true;
        locator->skip_codegen = true;

        this->push_child(locator);
        for (size_t i = 0; i < positional_arguments.size(); ++i)
        {
            this->push_positional_argument(positional_arguments[i]);
        }
    }

    void CallExpression::push_named_argument(Identifier *name, Expression *expr)
    {
        LOGIA_ASSERT(name && "name is mantadory");
        LOGIA_ASSERT(expr && "expr is mantadory");

        node_assert<Identifier>(name, __FUNCTION__ ":" TOSTRING(__LINE__));
        node_assert<Expression>(expr, __FUNCTION__ ":" TOSTRING(__LINE__));

        this->push_child(new CallExpressionArgument(argument_count++, name, expr));
    }
    void CallExpression::push_positional_argument(Expression *expr)
    {
        return this->push_named_argument(new Identifier(expr->rule, ""), expr);
    }

    void CallExpression::insert_named_argument(size_t position, Identifier *name, Expression *expr)
    {
        this->push_named_argument(name, expr);
        // now we swap position and length-1 to put the new argument in the right place
        for (size_t i = this->argument_count - 1; i > position; --i)
        {
            std::swap(this->children[i], this->children[i - 1]);
            this->children[i]->as<CallExpressionArgument>()->index = i;
        }
    }

    void CallExpression::insert_positional_argument(size_t position, Expression *expr)
    {
        return this->insert_named_argument(position, ast_create_identifier((char *)""), expr);
    }

    void CallExpression::remove_argument_at(size_t position)
    {
        if (position >= this->argument_count)
        {
            throw_compiler_error("Argument position out of range");
        }
        this->children.erase(this->children.begin() + position + 1); // +1 to skip locator
        this->argument_count--;
        // update indices
        for (size_t i = position; i < this->argument_count; ++i)
        {
            this->children[i + 1]->as<CallExpressionArgument>()->index = i; // +1 to skip locator
        }
    }

    CallExpressionArgument *CallExpression::get_argument_by_name(const char *name)
    {
        for (size_t i = 1; i < this->children.size(); ++i)
        {
            auto arg = this->get_child<CallExpressionArgument>(i);
            auto arg_name = arg->get_name();
            if (arg_name->operator==(name))
            {
                return arg;
            }
        }
        return nullptr;
    }

    CallExpressionArgument *CallExpression::get_argument_by_index(uint32_t index)
    {
        if (index >= this->argument_count)
        {
            return nullptr;
        }
        return this->get_child<CallExpressionArgument>(index + 1);
    }

    Expression *CallExpression::get_locator()
    {
        return this->get_child<Expression>(0);
    }

    Expression *CallExpression::get_argument_expr(uint32_t pos)
    {

        return this->get_argument_by_index(pos)->get_value();
    }

    Identifier *CallExpression::get_argument_name(uint32_t pos)
    {
        return this->get_argument_by_index(pos)->get_name();
    }

    std::vector<Expression *> CallExpression::get_arguments()
    {
        auto v = std::vector<Expression *>();
        v.reserve(this->argument_count);

        DEBUG() << v.size() << "/" << v.capacity() << "/" << this->children.size() << std::endl;

        for (size_t i = 1; i < this->children.size(); ++i)
        {
            v.push_back(this->children[i]->as<CallExpressionArgument>()->get_value());
        }

        return v;
    }

    void CallExpression::_set_type(Type *type)
    {
        // NOTE side-effect of Node::set_type: this implicity check that our return type is the expecte :P
        this->type = type;
    }

    void CallExpression::_pre_type_inference()
    {
        // it's possible to solve at this point if the locator where final
        // maybe it's a problem with BinaryExpression ??
        // auto locator = this->get_locator();
        // locator->pre_type_inference();

        auto locator = this->get_locator();
        locator->skip_type_inference = false;
        locator->pre_type_inference();
        locator->post_type_inference();

        auto locator_ty = locator->get_type();
        locator->set_type(locator_ty);

        Function *f = nullptr;
        if (!locator_ty->try_cast<Function>(&f))
        {
            LERROR() << this->to_string_tree() << std::endl;
            LERROR() << locator_ty->to_string_tree() << std::endl;
            throw_semantic_error(this, std::format("LGERR033 This expression is not callable is: '{}'", locator_ty->get_repr()));
            // cannot be used as a function
        }

        this->set_type(f->get_return_type()->get_final_type());

        Expression::_pre_type_inference();
    }

    Type *CallExpression::get_type()
    {
        return this->type;
    }

    std::string CallExpression::to_string()
    {
        auto locator = this->get_locator();
        if (!locator)
        {
            return std::string("CallExpression: incomplete");
        }

        auto arguments = this->get_arguments();

        if (locator->is<Identifier>())
        {
            return std::format("CallExpression[{}][{} arguments]{}", locator->as<Identifier>()->identifier, arguments.size(), Node::to_string());
        }

        return std::format("CallExpression[{} arguments]{}", arguments.size(), Node::to_string());
    }

    llvm::Value *CallExpression::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;

        if (this->children.size() == 0)
        {
            throw std::runtime_error("Incomplete CallExpression");
        }

        // Look up the name in the global module table.
        auto name = this->get_locator()->as<Identifier>();

        llvm::Function *CalleeF = backend->getFunction(name->identifier);
        if (!CalleeF)
        {
            throw std::runtime_error(std::string("Unknown function referenced: ") + name->identifier);
        }

        auto arguments = this->get_arguments();
        // If argument mismatch error.
        if (CalleeF->arg_size() != arguments.size())
        {
            throw std::runtime_error(std::format("Expected arguments {} arguments passed {} calling {}", CalleeF->arg_size(), arguments.size(), name->identifier));
        }
        auto arg_itr = CalleeF->arg_begin();

        std::vector<llvm::Value *> ArgsV;
        for (size_t i = 0, e = arguments.size(); i != e; ++i)
        {
            DEBUG() << "argument[" << i << "]" << std::endl;

            auto argument = arguments[i];
            auto ir_argument = llvm_load_if_required(argument->codegen(backend), backend);
            auto ir_argument_ty = ir_argument->getType();

            auto ir_parameter = CalleeF->getArg(i);
            auto ir_parameter_ty = ir_parameter->getType();

            // check arguments type are compatible one by one
            if (ir_parameter_ty != ir_argument_ty)
            {
                LERROR() << this->to_string_tree();
                throw_semantic_error(argument, std::format("Invalid argument {} '{}' of type '{}' expected type '{}'", i + 1, name->identifier, llvm_type_to_string(ir_argument_ty), llvm_type_to_string(ir_parameter_ty)));
            }

            ArgsV.push_back(ir_argument);
            if (!ArgsV.back())
            {
                return nullptr;
            }
        }

        // @llafuente remove name or we got duplications (same if strategy ?)
        auto call = backend->builder->CreateCall(CalleeF, ArgsV);

        this->cg_value = (llvm::Value *)call;
        return Expression::post_codegen(backend);
    }

    LOGIA_API CallExpression *ast_create_call_expr(Expression *locator, std::vector<Expression *> arguments)
    {
        auto callexpr = new CallExpression(nullptr, locator, arguments);

        return callexpr;
    }
} // namespace logia
