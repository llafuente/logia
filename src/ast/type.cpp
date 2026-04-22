#include "ast/constexpr.h"
#include "ast/type.h"
#include "ast/traverse.h"
#include "utils.h"

#include "llvm/IR/DIBuilder.h"

namespace logia::AST
{
    LOGIA_API LOGIA_LEND char *ast_primitives_to_string(Primitives prim)
    {
        switch (prim)
        {
        case Primitives::VOID_TY:
            return strdup("void");
        case Primitives::I8_TY:
            return strdup("i8");
        case Primitives::I16_TY:
            return strdup("i16");
        case Primitives::I32_TY:
            return strdup("i32");
        case Primitives::I64_TY:
            return strdup("i64");
        case Primitives::U8_TY:
            return strdup("u8");
        case Primitives::U16_TY:
            return strdup("u16");
        case Primitives::U32_TY:
            return strdup("u32");
        case Primitives::U64_TY:
            return strdup("u64");
        case Primitives::F16_TY:
            return strdup("f16");
        case Primitives::F32_TY:
            return strdup("f32");
        case Primitives::F64_TY:
            return strdup("f64");
        case Primitives::F128_TY:
            return strdup("f128");
        case Primitives::BOOL_TY:
            return strdup("bool");
        case Primitives::PTR_TY:
            return strdup("ptr");
        case Primitives::STRUCT_TY:
            return strdup("struct");
        case Primitives::FUNCTION_TY:
            return strdup("function");
        default:
            throw std::exception("unreachable");
        }
    }

    //
    // Type
    //

    Type::Type(antlr4::ParserRuleContext *rule, Primitives prim) : Node(rule)
    {
        this->primitive = prim;
    }
    Type::~Type()
    {
        // TODO
    }

    std::string Type::to_string()
    {
        return std::format("Type[{}] ({:p})", ast_primitives_to_string(this->primitive), static_cast<void *>(this->parent_node));
    }

    std::string Type::get_repr()
    {
        return this->to_string();
    }

    bool Type::isFunction() { return this->primitive == Primitives::FUNCTION_TY; };
    bool Type::isStruct() { return this->primitive == Primitives::STRUCT_TY; };

    Type *Type::get_type()
    {
        return this;
    }

    void Type::post_attach()
    {
        // only once, when a type is used it will be attached many times as references
        if (!this->is_attached)
        {
            this->is_attached = true;

            // NOTE the rest of types has no name, because primitives are handled at ast_create_program
        }
    }

    void Type::__register_type(const char *name)
    {
        // TODO REVIEW function block scope ? -> or closest block scope!?
        auto block = this->first_parent<Block>();
        block->set(name, this);
    }

    llvm::Value *Type::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir_type)
        {
            return (llvm::Value *)this->ir_type;
        }

        // TODO
        throw std::exception(__FUNCTION__ "todo");
    }

    //
    // Integer
    //

    Integer::Integer(bool is_signed, int bits) : Type(nullptr, Primitives::I8_TY), is_signed(is_signed), bits(bits) {}
    Integer::~Integer() {}

    std::string Integer::to_string()
    {
        return std::format("Type[{}]", this->get_repr());
    }
    std::string Integer::get_repr()
    {
        return std::format("{}{}", (this->is_signed ? "i" : "u"), this->bits);
    }

    llvm::Value *Integer::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        switch (this->bits)
        {
        case 1:
            this->ir_type = llvm::Type::getInt1Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i1", 1, llvm::dwarf::DW_ATE_boolean);
            break;
        case 8:
            this->ir_type = llvm::Type::getInt8Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i8", 1, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            break;
        case 16:
            this->ir_type = llvm::Type::getInt16Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i16", 1, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            break;
        case 32:
            this->ir_type = llvm::Type::getInt32Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i32", 1, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            break;
        case 64:
            this->ir_type = llvm::Type::getInt64Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i64", 1, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            break;
        case 128:
            this->ir_type = llvm::Type::getInt128Ty(codegen->context);
            this->di_type = codegen->dbuilder->createBasicType("i128", 1, this->is_signed ? llvm::dwarf::DW_ATE_signed : llvm::dwarf::DW_ATE_unsigned);
            break;
        default:
            throw std::runtime_error("Not supported number of bits");
        }

        LOGIA_ASSERT(this->ir_type);

        return (llvm::Value *)this->ir_type;
    }
    void Integer::post_attach()
    {
        // once guard
        if (!this->is_attached)
        {
            this->is_attached = true;
            this->__register_type(std::format("λ{}", this->get_repr()).c_str());
        }
    }

    //
    // Struct
    //
    StructAlias::StructAlias(antlr4::ParserRuleContext *rule, Identifier *from, Identifier *to, const char *_docstring) : docstring(_docstring), Type(rule, Primitives::NONE)
    {
        this->push_child(from);
        this->push_child(to);
    }

    Identifier *StructAlias::get_from()
    {
        return (Identifier *)this->children[0];
    }
    Identifier *StructAlias::get_to()
    {
        return (Identifier *)this->children[1];
    }
    std::string StructAlias::to_string()
    {
        return std::format("StructAlias ({:p})", static_cast<void *>(this->parent_node));
    }
    Type *StructAlias::get_type()
    {
        auto owner = (Struct *)this->parent_node;
        // TODO alias of methods ?
        return owner->get_field_type(this->get_to());
    }

    StructField::StructField(antlr4::ParserRuleContext *rule,
                             Identifier *name,
                             Type *type,
                             Expression *default_value,
                             const char *docstring) : docstring(docstring), Type(rule, Primitives::NONE)
    {
        this->push_child(name);
        this->push_child(type);
        if (default_value == nullptr)
        {
            // TODO type->get_default()
            this->push_child(new NoOp());
        }
        else
        {
            this->push_child(default_value);
        }
    }
    Identifier *StructField::get_name()
    {
        return this->get_child<Identifier>(0);
    }
    Type *StructField::get_type()
    {
        return ast_resolve_type(this->get_child<Type>(1));
    }
    Expression *StructField::get_default_value()
    {
        return this->get_child<Expression>(2);
    }
    std::string StructField::to_string()
    {
        return std::format("StructField ({:p})", static_cast<void *>(this->parent_node));
    }

    Struct::Struct(antlr4::ParserRuleContext *rule, Identifier *id) : Type(rule, Primitives::STRUCT_TY)
    {
        if (id != nullptr)
        {
            this->set_identifier(id);
        }
    }

    void Struct::set_identifier(Identifier *id)
    {
        LOGIA_ASSERT(id && "id parameters is required");

        if (this->has_name)
        {
            throw std::runtime_error("Struct already has a name");
        }

        this->has_name = true;
        this->unshift_child(id);
    }

    const char *Struct::get_name()
    {
        if (!this->has_name)
        {
            return "unkown"; // TODO REVIEW fail ?
        }
        return get_identifier()->identifier;
    }

    Identifier *Struct::get_identifier()
    {
        return this->get_child<Identifier>(0);
    }

    Identifier *Struct::get_alias_to(Identifier *from)
    {
        for (const auto &ptr : this->children)
        {
            if (auto alias = dynamic_cast<StructAlias *>(ptr))
            {
                if (strcmp(from->identifier, alias->get_from()->identifier) == 0)
                {
                    return alias->get_to();
                }
            }
        }

        return nullptr;
    }

    StructField *Struct::get_field(Identifier *id)
    {
        auto to = this->get_alias_to(id);
        if (to != nullptr)
        {
            id = to;
        }

        uint32_t count = 0;
        for (const auto &ptr : this->children)
        {
            if (auto field = dynamic_cast<StructField *>(ptr))
            {
                if (strcmp(id->identifier, field->get_name()->identifier) == 0)
                {
                    return field;
                }
            }
        }
        return nullptr;
    }

    uint32_t Struct::get_field_index(Identifier *id)
    {
        auto to = this->get_alias_to(id);
        if (to != nullptr)
        {
            id = to;
        }

        uint32_t count = 0;

        for (const auto &ptr : this->children)
        {
            if (auto field = dynamic_cast<StructField *>(ptr))
            {
                if (strcmp(id->identifier, field->get_name()->identifier) == 0)
                {
                    return count;
                }
                ++count;
            }
        }

        return -1;
    }

    Type *Struct::get_field_type(Identifier *id)
    {
        return this->get_field(id)->get_type();
    }

    std::string Struct::to_string()
    {
        return std::format("Type[struct {}] ({:p})", this->get_name(), static_cast<void *>(this->parent_node));
    }

    void Struct::post_attach()
    {
        // once guard
        if (!this->is_attached)
        {
            this->is_attached = true;
            // if the struct has a name -> attach it to body
            if (this->has_name)
            {
                this->__register_type(this->get_name());
            }
        }
    }

    llvm::Value *Struct::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir_type)
        {
            return (llvm::Value *)this->ir_type;
        }

        std::vector<llvm::Type *> elements;
        elements.reserve(this->field_count);
        for (auto &prop : this->children)
        {
            if (auto field = dynamic_cast<StructField *>(prop)) // review it works ?
            {
                elements.push_back((llvm::Type *)field->get_type()->codegen(codegen, builder));
            }
        }

        auto st = llvm::StructType::create(codegen->context, this->get_name());
        st->setBody(elements);

        this->ir_type = st;
        return (llvm::Value *)this->ir_type;
    }

    //
    // TypeDef
    //
    // REVIEW, it's a type but it's definition, need to  distinguish both ?
    TypeDef::TypeDef() : Type(nullptr, Primitives::NONE) {}
    TypeDef::~TypeDef() {}

    Type *TypeDef::get_type()
    {
        // TODO support more than one!?
        LOGIA_ASSERT(this->children.size() != 1, "TODO: single resolve atm");
        // search children!
        auto id = this->get_child<Identifier>(0);
        auto block = this->first_parent<Block>();
        auto node = block->lookup2<Node>(id->identifier);

        return node->get_type();
    }
    std::string TypeDef::to_string()
    {
        if (this->children.size())
        {
            auto id = (Identifier *)this->children[0];
            return std::format("TypeDef[{}]", id->identifier);
        }
        return std::format("TypeDef[?]");
    }

    llvm::Value *TypeDef::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        return this->get_type()->codegen(codegen, builder);
    }

    Node *TypeDef::resolve()
    {
        LOGIA_ASSERT(this->children.size() != 1, "TODO: single resolve atm");
        return this->children[0]->resolve();
    }

    //
    // FunctionParameters
    //

    FunctionParameter::FunctionParameter(
        Identifier *name,
        Type *type,
        Node *defaultValue) : alloca_inst(nullptr), Node(name->rule)
    {
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(type);

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
    Expression *FunctionParameter::get_default_value()
    {
        return this->get_child<Expression>(2);
    }
    std::string FunctionParameter::to_string()
    {
        return std::format("Parameter[{}][{}]", this->get_name()->identifier, this->get_type()->to_string());
    }
    llvm::Value *FunctionParameter::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        return this->alloca_inst = builder->CreateAlloca((llvm::Type *)this->get_type()->codegen(codegen, builder), nullptr, this->get_name()->identifier);
    }
    //
    // Function
    //

    Function::Function(antlr4::ParserRuleContext *rule, Identifier *id, Type *return_type, bool is_intrinsic) : Type(rule, Primitives::FUNCTION_TY)
    {
        LOGIA_ASSERT(id && "id parameters is required");

        if (return_type == nullptr)
        {
            return_type = new Type(nullptr, Primitives::VOID_TY);
        }

        this->push_child(id);          // get_name
        this->push_child(return_type); // get_return_type

        // auto block = new Block(nullptr, ast_create_identifier("function_param_alloca"));
        // this->push_child(block);

        // NOTE need to push something as arguments will be +3
        if (is_intrinsic)
        {
            this->push_child(new NoOp()); // get_body
        }
        else
        {
            auto block = new FunctionBlock(nullptr, ast_create_identifier("function_body"));
            this->push_child(block); // get_body
        }
        this->is_intrinsic = is_intrinsic;
    }

    Function::~Function()
    {
        // TODO
    }

    std::string Function::to_string()
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
            list += param->get_type()->to_string();
        }
        return std::format("Type[{} function {} ({})] ({:p})", this->get_return_type()->to_string(), this->get_name(), list, static_cast<void *>(this->parent_node));
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
        throw std::runtime_error(__FUNCTION__ "todo");
    }
    uint32_t get_optional_parameters_size()
    {
        throw std::runtime_error(__FUNCTION__ "todo");
    }

    // register myself into closest block
    void Function::post_attach()
    {
        // only once, when a type is used it will be attached many times as references
        if (!this->is_attached)
        {
            this->is_attached = true;

            this->__register_type(this->get_name());
        }
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

    void Function::codegen_parameters(logia::Backend *backend, llvm::IRBuilder<> *builder)
    {
        auto pcount = this->get_parameter_count();
        size_t i = 0;
        for (auto &Arg : this->cg_value->args())
        {
            auto param = this->get_parameter(i);
            param->codegen(backend, builder);
            builder->CreateStore(&Arg, param->alloca_inst);
            ++i;
        }
    }

    llvm::Value *Function::codegen(logia::Backend *backend, llvm::IRBuilder<> *builder)
    {
        // TODO FIXME this is the perfect generation order!
        // pre_codegen
        // generate return type
        // generate each parameter type
        // create the function type
        // create the function
        // attach the function entry block (pre_codegen)
        // alloca/store/meta of each parameter
        // post_codegen
        // generate body

        DEBUG() << this->to_string() << std::endl;
        // generate return type, as it's the first in metada
        auto rtype = this->get_return_type();
        rtype->codegen(backend, builder);

        // generate all parameters
        auto pcount = this->get_parameter_count();
        this->ir_parameters.reserve(pcount);
        llvm::SmallVector<llvm::Metadata *, 12> md_types;
        md_types.push_back(rtype->di_type);

        for (size_t i = 0; i < pcount; ++i)
        {
            // IR Type
            auto param_type = this->get_parameter(i)->get_final_type();
            param_type->codegen(backend, builder);
            this->ir_parameters.push_back(param_type->ir_type);

            // Metadata type
            auto RSO = llvm_type_to_string(param_type->ir_type);

            // llvm::MDString *TypeNameMD = llvm::MDString::get(backend->context, RSO.c_str());
            // Wrap it in an MDNode (could also include more info)
            // llvm::MDNode *TypeNode = llvm::MDNode::get(backend->context, {TypeNameMD});
            // md_types.push_back(TypeNode);
            md_types.push_back(param_type->di_type);

            DEBUG() << "parameter[" << i << "] is " << RSO << std::endl;
        }

        auto func = llvm::FunctionType::get(rtype->ir_type,
                                            this->ir_parameters, // parameter list
                                            false);              // not variadic
        this->ir_type = (llvm::Type *)func;

        this->cg_value = llvm::Function::Create((llvm::FunctionType *)this->ir_type, llvm::Function::ExternalLinkage, 0, this->get_name(), backend->module.get());

        // Create a basic block and insert a return

        if (!this->is_intrinsic)
        {
            llvm::DISubprogram *SP = nullptr;
            if (backend->debug)
            {
                // TODO STUDY only defined function can be coverage
                // intrinsics are defined elsewhere, we may required something to be able to notice call count...
                llvm::DISubroutineType *DISig = backend->dbuilder->createSubroutineType(backend->dbuilder->getOrCreateTypeArray(md_types));

                SP = backend->dbuilder->createFunction(
                    backend->dcompilation_unit->getFile(),
                    this->get_name(),
                    llvm::StringRef(),
                    backend->dfile,
                    this->rule->start->getLine(), // Line number
                    DISig,
                    0,                      // STUDY first line in the scope is "0" ?
                    llvm::DINode::FlagZero, // STUDY FlagPrototyped ??
                    llvm::DISubprogram::SPFlagDefinition);

                // assign after initialize parameters!
                this->cg_value->setSubprogram(SP);

                backend->dscopes.push_back(SP);
            }
            backend->set_debug_information(this->rule);
            this->get_body()->codegen(backend, builder);

            if (backend->debug)
            {
                // TODO FIXME this is out of order, we will fix it in the future!!!
                // strange behaviour but llvm_basicblock should be inserted into ir before calling this
                // so it should happens after codegen
                for (size_t i = 0; i < pcount; ++i)
                {
                    auto fparam = this->get_parameter(i);

                    // Create a debug descriptor for the variable.
                    llvm::DILocalVariable *D = backend->dbuilder->createParameterVariable(
                        SP, this->get_parameter_name(i)->identifier, i + 1, backend->dfile, 1, backend->di_double_ty,
                        true);
                    auto line = fparam->get_name()->rule->start->getLine();
                    auto column = fparam->get_name()->rule->start->getStartIndex();
                    backend->dbuilder->insertDeclare(fparam->alloca_inst, D, backend->dbuilder->createExpression(),
                                                     llvm::DILocation::get(SP->getContext(), line, column, SP),
                                                     this->get_body()->llvm_basicblock);
                }

                backend->dscopes.pop_back();
            }
        }

        return (llvm::Value *)this->ir_type;
    }

    LOGIA_API LOGIA_LEND Function *ast_create_function_type(Identifier *id, Type *return_type)
    {
        return new Function(nullptr, id, return_type);
    }

    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, Identifier *id, Type *return_type)
    {
        LOGIA_ASSERT(program);

        auto f = new Function(nullptr, id, return_type, true);

        // NOTE it will attach itself to program scope
        program->unshift_child(f);

        return f;
    }

    void Function::add_param(FunctionParameter *param)
    {

        LOGIA_ASSERT(!this->is_attached && "Function type should be created before attached");

        this->push_child(param);
        this->get_body()->set(param->get_name()->identifier, param);
    }

    void Function::check_call(CallExpression *callee)
    {
        // TODO
    }

    LOGIA_API LOGIA_LEND Struct *ast_create_struct_type(Identifier *id)
    {
        return new Struct(nullptr, id);
    }

    void Struct::add_field(
        antlr4::ParserRuleContext *rule,
        Identifier *name,
        Type *type,
        Expression *default_value,
        const char *docstring)
    {
        LOGIA_ASSERT(this);
        LOGIA_ASSERT(this->isStruct());

        LOGIA_ASSERT(type && "type is required for fields");
        LOGIA_ASSERT(name && "name is required for fields");

        this->push_child(new StructField(rule, name, type, default_value, docstring));
        ++this->field_count;
    }

    void Struct::add_alias(antlr4::ParserRuleContext *rule, Identifier *from, Identifier *to, const char *docstring)
    {
        LOGIA_ASSERT(from);
        LOGIA_ASSERT(to);

        // TODO exists to ?
        // TODO exists from ?

        this->push_child(new StructAlias(rule, from, to, docstring));
        ++this->alias_count;
    }

    LOGIA_API LOGIA_LEND Type *ast_resolve_type(Node *node)
    {
        int max = 100;
        do
        {
            node = node->get_type();
        } while (!node->is<TypeDef>() && --max);

        return node->as<Type>();
    }
}
