#include "logia/ast/callexpr.h"

#include "utils.h"
#include "logia/logia.h"
#include "logia/backend.h"
#include "logia/ast/program.h"
#include "logia/ast/expr.h"
#include "logia/ast/scope.h"
#include "logia/ast/llvm.h"
#include "logia/ast/memberaccessexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/function.h"
#include "logia/ast/unaryexpr.h"

#include "logia/multiple_dispatch.h"
#include "logia/type_inference.h"

#include "llvm/IR/Value.h"

#include <format>

namespace logia::AST
{
    //
    // CallExpressionArgument
    //

    CallExpressionArgument::CallExpressionArgument(
        size_t index,
        Identifier *name,
        Expression *value) : Node(value->loc), index(index)
    {
        this->has_type = false;
        this->type_inference_pass_id = TYPE_INFERENCE_MAX;

        this->push_child(name);
        name->skip_codegen = true;
        name->has_type = false;
        name->type_inference_pass_id = TYPE_INFERENCE_MAX;
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
        return std::format("CallExpressionArgument{}", Node::to_string());
    }

    Type *CallExpressionArgument::get_type()
    {
        return this->get_value()->get_type();
    }

    void CallExpressionArgument::on_after_attach() {}

    void CallExpressionArgument::validate() {}

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
    CallExpression::CallExpression(location loc) : Expression(loc)
    {
    }
    CallExpression::CallExpression(location loc, Expression *locator, std::vector<Expression *> positional_arguments) : Expression(loc)
    {
        LOGIA_VERIFY(locator != nullptr, "locator is mantadory");

        Identifier *ident;
        MemberAccessExpression *mae;
        if (locator->try_cast<Identifier>(&ident))
        {
            ident->resolve = true;
            ident->resolve_unique = false;
        }
        else if (locator->try_cast<MemberAccessExpression>(&mae))
        {
            // nothing to do for now, we will resolve it at type inference when we have more info about the left side
        }
        else
        {
            throw_compiler_error("unexpected locator type for CallExpression");
        }

        this->push_child(locator);
        for (size_t i = 0; i < positional_arguments.size(); ++i)
        {
            this->push_positional_argument(positional_arguments[i]);
        }
    }

    void CallExpression::push_named_argument(Identifier *name, Expression *expr)
    {
        LOGIA_VERIFY(name != nullptr, "name is mantadory");
        LOGIA_VERIFY(expr != nullptr, "expr is mantadory");

        node_assert<Identifier>(name, TOSTRING(__FUNCTION__) ":" TOSTRING(__LINE__));
        node_assert<Expression>(expr, TOSTRING(__FUNCTION__) ":" TOSTRING(__LINE__));

        this->push_child(new CallExpressionArgument(argument_count++, name, expr));
    }
    void CallExpression::push_positional_argument(Expression *expr)
    {
        return this->push_named_argument(new Identifier(expr->loc, ""), expr);
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
        return this->insert_named_argument(position, new Identifier(expr->loc, ""), expr);
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

        // LOG_ERR("{}/{}/{}", v.size(), v.capacity(), this->children.size());

        for (size_t i = 1; i < this->children.size(); ++i)
        {
            v.push_back(this->children[i]->as<CallExpressionArgument>()->get_value());
        }

        return v;
    }

    void CallExpression::_set_type(Type *type)
    {
        // nothing!
    }

    std::vector<Function *> CallExpression::find_candidates()
    {
        // locator is an identifier
        // a) points to a Function(s) -> direct_call
        // b) indirect_call

        LOG(DBG, "{}", this->to_string_tree());

        Expression *locator = this->get_locator();
        Identifier *ident;
        if (locator->try_cast<Identifier>(&ident))
        {
            auto result = logia::AST::scope_lookup_all(this, ident->identifier);
            if (result.is_error())
            {
                throw_semantic_error(locator, result.message);
            }

            auto nlist = result.unwrap_success();
            // did not found anything? -> error later
            if (nlist.size() == 0)
            {
                return {};
            }
            // found something
            // a) 0..n if Function(s)
            if (nlist[0]->is<Function>())
            {
                this->is_direct_call = true;
                locator->skip_codegen = true; // we are not going to cg anything here
                return nodelist_cast<Function>(nlist, true);
            }
            // b) 1 if it's not
            if (nlist.size() > 1)
            {
                throw_semantic_error(locator, "Unexpected locator with multiple resolutions");
            }

            this->is_indirect_call = true;
            auto decl = nlist[0];
            // cg to get the alloca value!
            ident->set_declaration(decl);
            auto ty = decl->get_final_type();
            if (!ty->is<Function>())
            {
                throw_semantic_error(locator, std::format(LGERR_CALLEXPR001, ty->get_repr()));
            }
            ident->set_type(ty);

            return {ty->as<Function>()};
        }

        throw_compiler_error("unreachable!");
    }

    void CallExpression::_early_type_inference()
    {
        // rebalance the tree
        auto locator = this->get_locator();
        MemberAccessExpression *mae;
        if (locator->try_cast<MemberAccessExpression>(&mae) && !this->is_method_call)
        {
            LOG(DBG, "start transforming tree {}", this->to_string_tree());

            this->set_child(mae->get_right(), 0);
            this->insert_positional_argument(0, mae->get_left());

            // ?? delete mae;
            LOG(DBG, "stop transforming tree {}", this->to_string_tree());
            this->is_method_call = true;
        }
        Expression::_early_type_inference();
    }

    void CallExpression::_pre_type_inference()
    {
        // type inference at this point need to "rebalance" the tree if found a MemberAccessExpression
        // because the lhs is the first parameter and rhs is the function to call
        auto locator = this->get_locator();

        // find a proper target or throws!
        auto list = this->find_candidates();
        auto result = multiple_dispatch::find_one(list, this);
        if (result.is_error())
        {
            throw_semantic_error(this, result.message);
        }

        Function *target = result.unwrap_success();

        // NOTE don't use set_type, just set the flag
        this->is_typed = true;
        this->callee = target;

        locator->set_type((Type *)target);
        // fill the gaps, order arguments, etc.
        multiple_dispatch::match(this, target, true);

        Expression::_pre_type_inference();
    }

    Type *CallExpression::get_type()
    {
        return this->callee == nullptr ? nullptr : this->callee->get_return_type()->get_final_type();
    }

    void CallExpression::on_after_attach() {}

    void CallExpression::validate() {}

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
            return std::format("CallExpression[{}, args = {}, target = {}]{}", locator->as<Identifier>()->identifier, arguments.size(), (void *)this->callee, Node::to_string());
        }

        return std::format("CallExpression[args = {}, target = {}]{}", arguments.size(), (void *)this->callee, Node::to_string());
    }

    llvm::Value *CallExpression::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());

        if (this->children.size() == 0)
        {
            throw std::runtime_error("Incomplete CallExpression");
        }

        // Look up the name in the global module table.
        auto func = this->get_locator()->get_type()->as<Function>();
        llvm::Function *CalleeF = func->ir_func;
        if (!CalleeF)
        {
            // throw std::runtime_error(std::string("Unknown function referenced: ") + name->identifier);
            throw_compiler_error("unkown function!");
        }

        auto arguments = this->get_arguments();
        // If argument mismatch error.
        // REVIEW compiler error? we should have everything tested at multiple_dispatch::find_one/match
        if (CalleeF->arg_size() != arguments.size())
        {
            throw_semantic_error(this, std::format(LGERR_CALLEXPR002, CalleeF->arg_size(), arguments.size(), func->get_repr()));
        }

        std::vector<llvm::Value *> ArgsV;
        for (size_t i = 0, e = arguments.size(); i != e; ++i)
        {
            LOG(DBG, "argument[{}]", i);

            auto argument = arguments[i];
            auto ir_argument = llvm_load_if_required(argument->post_codegen(backend), backend);
            auto ir_argument_ty = ir_argument->getType();

            auto ir_parameter = CalleeF->getArg(i);
            auto ir_parameter_ty = ir_parameter->getType();

            // check arguments type are compatible one by one
            // REVIEW compiler error? we should have everything tested at multiple_dispatch::find_one/match
            if (ir_parameter_ty != ir_argument_ty)
            {
                LOG_ERR("{}", this->to_string_tree());
                throw_semantic_error(argument, std::format(LGERR_CALLEXPR003, i + 1, llvm_type_to_string(ir_argument_ty), llvm_type_to_string(ir_parameter_ty), func->get_repr()));
            }

            ArgsV.push_back(ir_argument);
            if (!ArgsV.back())
            {
                return nullptr;
            }
        }

        // @llafuente remove name or we got duplications (same if strategy ?)
        if (is_indirect_call)
        {
            auto value = this->get_locator()->post_codegen(backend);
            value = llvm_load_if_required(value, backend);
            this->cg_value = (llvm::Value *)backend->builder->CreateCall(func->ir_functy, value, ArgsV);
        }
        else if (is_direct_call)
        {
            this->cg_value = (llvm::Value *)backend->builder->CreateCall(CalleeF, ArgsV);
        }
        else
        {
            throw_compiler_error("unreachable!");
        }

        return Expression::post_codegen(backend);
    }

} // namespace logia
