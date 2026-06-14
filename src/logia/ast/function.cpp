#include "logia/ast/function.h"

#include "utils.h"
#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/program.h"
#include "logia/ast/functionblock.h"
#include "logia/ast/llvm.h"

#include "llvm/IR/Instructions.h" // AllocaInst
#include "llvm/IR/Metadata.h"     // Metadata
#include "llvm/ADT/SmallVector.h" // SmallVector

namespace logia::AST
{
    //
    // FunctionParameters
    //

    FunctionParameter::FunctionParameter(
        Identifier *name,
        Type *type,
        Node *defaultValue) : Node(name->loc), alloca_inst(nullptr)
    {
        LOGIA_VERIFY(name != nullptr);
        LOGIA_VERIFY(type != nullptr);

        this->is_typed = true;
        name->skip_codegen = true;
        name->skip_type_inference = true;
        name->set_type(type);

        this->push_child(name);
        this->push_child(type);
        if (defaultValue == nullptr)
        {
            this->push_child(new NoOp());
        }
        else
        {
            this->push_child(defaultValue);
        }
    }
    Identifier *FunctionParameter::get_name()
    {
        return this->get_child<Identifier>(0);
    }
    Type *FunctionParameter::get_type()
    {
        return this->get_child<Type>(1);
    }
    void FunctionParameter::_set_type(Type *type)
    {
        // this will make sense when templates arrive!
        throw_compiler_error("makes no sense");
    }
    Expression *FunctionParameter::get_default_value()
    {
        return this->get_child<Expression>(2);
    }
    bool FunctionParameter::has_default_value()
    {
        return !this->children[2]->is<NoOp>();
    }
    std::string FunctionParameter::to_string()
    {
        return std::format("Parameter[{}][{}]{}", this->get_name()->identifier, this->get_type()->to_string(), Node::to_string());
    }
    llvm::Value *FunctionParameter::post_codegen(logia::Backend *backend)
    {
        this->cg_value = this->alloca_inst = backend->builder->CreateAlloca((llvm::Type *)this->get_final_type()->codegen(backend), 0, nullptr, this->get_name()->identifier);
        return Node::post_codegen(backend);
    }

    void FunctionParameter::post_attach() {}

    void FunctionParameter::validate() {}
    //
    // Function
    //

    Function::Function(location loc, Identifier *name, Type *return_type, bool is_intrinsic) : Type(loc, Primitives::FUNCTION_TY)
    {
        LOGIA_VERIFY(name != nullptr, "name parameter is required");

        this->is_typed = true;
        name->skip_codegen = true;
        name->set_type(this);

        if (return_type == nullptr)
        {
            return_type = new Void(); // TODO unique!
        }

        this->push_child(name);        // get_name
        this->push_child(return_type); // get_return_type

        // auto block = new Block(nullptr, ast_create_identifier("function_param_alloca"));
        // this->push_child(block);

        auto block = new FunctionBlock({}, ast_create_identifier("function_body"));
        this->push_child(block); // get_body

        // children+3 are the arguments!

        this->is_intrinsic = is_intrinsic;
    }

    Function::~Function()
    {
        // TODO
    }

    std::string Function::to_string()
    {
        return std::format("Type.Function {}", Node::to_string());
    }

    std::string Function::get_repr()
    {
        std::string list;
        // concat each parameter type
        auto parameters = this->get_parameters();
        for (auto &param : parameters)
        {
            if (!list.empty())
            {
                list += ", ";
            }
            list += param->get_type()->get_repr();
            list += " ";
            list += param->get_name()->identifier;
            if (param->has_default_value())
            {
                list += " = ?";
                // TODO list += param->get_default_value()->get_repr();
            }
        }
        return std::format("function {} ({}) {}", this->get_name(), list, this->get_return_type()->get_repr());
    }

    std::vector<FunctionParameter *> Function::get_parameters()
    {
        std::vector<FunctionParameter *> out;
        out.reserve(this->children.size() - 3);
        for (size_t i = 3; i < this->children.size(); ++i)
        {
            out.push_back((FunctionParameter *)this->children[i]);
        }

        return out;
    }

    const char *Function::get_name()
    {
        return get_identifier()->identifier;
    }

    Identifier *Function::get_identifier()
    {
        return this->get_child<Identifier>(0);
    }

    Type *Function::get_return_type()
    {
        return this->get_child<Type>(1);
    }

    Block *Function::get_body()
    {
        return this->get_child<Block>(2);
    }

    uint32_t get_mandatory_parameters_size()
    {
        throw std::runtime_error(TOSTRING(__FUNCTION__) "to-do");
    }
    uint32_t get_optional_parameters_size()
    {
        throw std::runtime_error(TOSTRING(__FUNCTION__) "to-do");
    }

    // register myself into closest block
    void Function::post_attach()
    {
        logia::AST::scope_set(this, this->get_name(), this, false);
        // this->__register_type(this->get_name());
    }

    void Function::validate()
    {
        // TODO search if any implementation of this function name has same parameters!

        // TODO all parameters shall be unique
    }

    int64_t Function::get_parameter_count()
    {
        return this->children.size() - 3;
    }

    FunctionParameter *Function::get_parameter(uint32_t i)
    {
        return this->children[3 + i]->as<FunctionParameter>();
    }

    Identifier *Function::get_parameter_name(uint32_t i)
    {
        return this->get_parameter(i)->get_name();
    }

    FunctionParameter *Function::get_parameter_by_name(const char *name)
    {
        for (size_t i = 3; i < this->children.size(); ++i)
        {
            auto fp = this->children[i]->as<FunctionParameter>();
            if (fp->get_name()->operator==(name))
            {
                return fp;
            }
        }
        return nullptr;
    }

    void Function::codegen_parameters(logia::Backend *backend)
    {
        auto pcount = this->get_parameter_count();
        size_t i = 0;
        for (auto &Arg : this->ir_func->args())
        {
            auto param = this->get_parameter(i);
            param->codegen(backend);
            backend->builder->CreateStore(&Arg, param->alloca_inst);

            if (backend->debug)
            {
                auto ty = param->get_final_type();
                auto name = param->get_name();
                // Create a debug descriptor for the variable.
                llvm::DILocalVariable *D = backend->dbuilder->createParameterVariable(
                    this->di_subprogram, name->identifier, i + 1, backend->dfile, 1, ty->di_type,
                    true);
                backend->dbuilder->insertDeclare(param->alloca_inst, D, backend->dbuilder->createExpression(),
                                                 llvm::DILocation::get(this->di_subprogram->getContext(), name->loc.start_line, name->loc.start_column, this->di_subprogram),
                                                 this->get_body()->ir_basicblock);
            }
            ++i;
        }
    }

    // codegen order!
    // pre_codegen
    // generate return type
    // generate each parameter type
    // create the function type
    // create the function
    // * attach the function entry block (pre_codegen)
    // * alloca/store/meta of each parameter
    // post_codegen
    // generate body

    void Function::pre_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        // generate return type, as it's the first in metada
        auto rtype = this->get_return_type()->get_final_type();
        rtype->codegen(backend);

        // generate all parameters
        auto pcount = this->get_parameter_count();
        this->ir_parameters.reserve(pcount);
        llvm::SmallVector<llvm::Metadata *, 12> md_types;
        md_types.push_back(rtype->di_type);

        for (size_t i = 0; i < pcount; ++i)
        {
            // IR Type
            auto param_type = this->get_parameter(i)->get_final_type();
            param_type->codegen(backend);
            this->ir_parameters.push_back(param_type->ir_type);

            // Metadata type
            auto RSO = llvm_type_to_string(param_type->ir_type);

            // llvm::MDString *TypeNameMD = llvm::MDString::get(backend->context, RSO.c_str());
            // Wrap it in an MDNode (could also include more info)
            // llvm::MDNode *TypeNode = llvm::MDNode::get(backend->context, {TypeNameMD});
            // md_types.push_back(TypeNode);
            md_types.push_back(param_type->di_type);

            LOG(DBG, "parameter[{}] is {}", i, RSO);
        }

        // function type
        this->ir_functy = llvm::FunctionType::get(rtype->ir_type,
                                                  this->ir_parameters, // parameter list
                                                  false);              // not variadic
        // function pointer
        this->ir_type = this->ir_functy->getPointerTo(0);
        // the function
        this->ir_func = llvm::Function::Create(this->ir_functy, llvm::Function::ExternalLinkage, 0, this->get_name(), backend->module.get());

        if (!this->is_intrinsic)
        {
            if (backend->debug)
            {
                // TODO STUDY only defined function can be coverage
                // intrinsics are defined elsewhere, we may required something to be able to notice call count...
                llvm::DISubroutineType *DISig = backend->dbuilder->createSubroutineType(backend->dbuilder->getOrCreateTypeArray(md_types));

                this->di_subprogram = backend->dbuilder->createFunction(
                    backend->dcompilation_unit->getFile(),
                    this->get_name(),
                    llvm::StringRef(),
                    backend->dfile,
                    this->loc.start_line, // Line number
                    DISig,
                    0,                      // STUDY first line in the scope is "0" ?
                    llvm::DINode::FlagZero, // STUDY FlagPrototyped ??
                    llvm::DISubprogram::SPFlagDefinition);

                // assign after initialize parameters!
                this->ir_func->setSubprogram(this->di_subprogram);
            }
            backend->set_debug_information(this->loc, this->di_subprogram);

            backend->dscopes.push_back(this->di_subprogram);
            this->get_body()->pre_codegen(backend);
            backend->dscopes.pop_back();
        }
        LOG(DBG, "exit!");
        Node::pre_codegen(backend);
    }

    llvm::Value *Function::post_codegen(logia::Backend *backend)
    {
        // Create a basic block and insert a return

        backend->dscopes.push_back(this->di_subprogram);
        this->get_body()->post_codegen(backend);
        backend->dscopes.pop_back();

        return Type::post_codegen(backend);
    }

    void Function::_pre_type_inference()
    {
        auto return_ty = this->get_final_type();
        this->foreach_descendant<ReturnStmt>([return_ty](auto rstmt, auto deep)
                                             { rstmt->set_type(return_ty); return false; });
        Type::_pre_type_inference();
    }

    void Function::push_parameter(FunctionParameter *param)
    {

        LOGIA_VERIFY(this->is_attached == false, "Function type should be created before attached");
        param->index = this->get_parameter_count();
        this->push_child(param);
        if (!this->is_intrinsic)
        {
            this->get_body()->scope_set(param->get_name()->identifier, param);
        }
        else
        {
            param->get_name()->skip_type_inference = true;
        }
    }

    void Function::insert_parameter(size_t position, FunctionParameter *param)
    {
        this->push_parameter(param);
        // now we swap position and length-1 to put the new argument in the right place
        auto last = position + 3;
        LOG(SILLY, "from {}  to {}", this->children.size() - 1, last);
        for (size_t i = this->children.size() - 1; i > last; --i)
        {
            auto index = i - 3;
            LOG(SILLY, "swap {} index {}", i, index);
            std::swap(this->children[i], this->children[i - 1]);
            this->children[i]->as<FunctionParameter>()->index = index;
        }
        this->children[last]->as<FunctionParameter>()->index = position;
    }

    void Function::validate_and_fill_call(CallExpression *callee)
    {
        auto cpy = callee->children;
        callee->children.clear();
        // reset CallExpression
        callee->children.push_back(cpy[0]); // copy locator
        callee->argument_count = 0;

        auto param_count = this->get_parameter_count();
        for (auto i = 0; i < param_count; ++i)
        {
            auto param = this->get_parameter(i);
            auto param_name = param->get_name();

            // search if any argument has the name -> true use it, false use the first
            auto found = false;
            for (auto j = 0; j < cpy.size(); j += 2)
            {
                auto arg_name = cpy[j]->as<Identifier>();
                if (arg_name == param_name)
                {
                    callee->push_named_argument(param_name, cpy[j + 1]->as<Expression>());
                    cpy.erase(cpy.begin() + j, cpy.begin() + j + 1);

                    found = true;
                    break;
                }
            }

            if (!found)
            {
                // fetch the first "non-named" param
                for (auto j = 0; j < cpy.size(); j += 2)
                {
                    auto arg_name = cpy[j]->as<Identifier>();
                    if (arg_name->operator==(""))
                    {
                        callee->push_named_argument(param_name, cpy[j + 1]->as<Expression>());
                        cpy.erase(cpy.begin() + j, cpy.begin() + j + 1);

                        found = true;
                        break;
                    }
                }
            }

            // still not found ? -> default ?
            if (!found && param->has_default_value())
            {
                found = true;
                callee->push_named_argument(param_name, param->get_default_value());
            }

            if (!found)
            {
                throw_semantic_error(callee, std::format("parameter '{}' not sent", param_name->identifier));
            }
        }
    }

    //
    // Operator
    //

    Operator::Operator(location loc, Operators op, Type *return_type) : Function(loc, new Identifier(loc, ast_operator_to_function_name(op)), return_type, false)
    {
        switch (op)
        {
        case Operators::PREFIX_DEREFERENCE:
            throw_semantic_error(this, "LGERR_OP001 dereference operator is not allowed to overload");
            break;
        }
        this->op = op;
    }
    Operator::~Operator() {}

    //
    // Intrinsics
    //
    Intrinsic::Intrinsic(llvm::Function *ir, const char *real_name, const char *scope_name, Type *return_type, std::vector<Type *> arguments) : Function({}, new Identifier({}, scope_name), return_type, true)
    {
        this->real_name = _strdup(real_name);

        for (auto t : arguments)
        {
            this->push_parameter(new FunctionParameter(new Identifier({}, ""), t, nullptr));
        }
        // configure/hack the function!
        this->is_post_type_inference = this->is_pre_type_inference = true; // ignore type inference, but no skip
        this->is_post_codegen = this->is_pre_codegen = true;               // ignore codegen, but no skip
        // set codegen result
        this->cg_value = this->ir_func = ir;
        this->ir_functy = (llvm::FunctionType *)ir->getType();
    }

    std::string Intrinsic::to_string()
    {
        return std::format("Type.Function.Intrinsic[{}] {}", this->real_name, Node::to_string());
    }

    llvm::Value *Intrinsic::post_codegen(logia::Backend *backend)
    {
        // skip Function::post_codegen, because we dont have a body block!
        return Type::post_codegen(backend);
    }
}