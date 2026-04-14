#include "ast/type.h"
#include "ast/traverse.h"
#include "utils.h"

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

    Type::Type(antlr4::ParserRuleContext *rule, Primitives prim) : Node(rule, ast_types::TYPE)
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

    void Type::on_after_attach()
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
        auto parentBody = (Block *)ast_find_closest_parent(this, ast_types::BODY);
        LOGIA_ASSERT(parentBody);

        parentBody->set(name, this);
    }

    llvm::Value *Type::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->llvm_type)
        {
            return (llvm::Value *)this->llvm_type;
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
        case 8:
            this->llvm_type = llvm::Type::getInt8Ty(codegen->context);
            break;
        case 16:
            this->llvm_type = llvm::Type::getInt16Ty(codegen->context);
            break;
        case 32:
            this->llvm_type = llvm::Type::getInt32Ty(codegen->context);
            break;
        case 64:
            this->llvm_type = llvm::Type::getInt64Ty(codegen->context);
            break;
        case 128:
            this->llvm_type = llvm::Type::getInt128Ty(codegen->context);
            break;
        default:
            throw std::runtime_error("Not supported number og bits");
        }

        LOGIA_ASSERT(this->llvm_type);

        return (llvm::Value *)this->llvm_type;
    }
    void Integer::on_after_attach()
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
        return (Identifier *)this->children[0];
    }
    Type *StructField::get_type()
    {
        return (Type *)this->children[1];
    }
    Expression *StructField::get_default_value()
    {
        return (Expression *)this->children[2];
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
        NODE_TYPE_ASSERT(id, ast_types::IDENTIFIER);

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
        return (Identifier *)this->children[0];
    }

    Identifier *Struct::get_alias_to(Identifier *from)
    {
        for (auto &prop : this->children)
        {
            if (typeid(prop) == typeid(StructAlias)) // review it works ?
            {
                auto alias = (StructAlias *)prop;
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
        for (auto &prop : this->children)
        {
            if (typeid(prop) == typeid(StructField)) // review it works ?
            {
                auto field = (StructField *)prop;
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
        for (auto &prop : this->children)
        {
            if (typeid(prop) == typeid(StructField)) // review it works ?
            {
                auto field = (StructField *)prop;
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
        // TODO nullptr
        return this->get_field(id)->get_type();
    }

    std::string Struct::to_string()
    {
        return std::format("Type[struct {}] ({:p})", this->get_name(), static_cast<void *>(this->parent_node));
    }

    void Struct::on_after_attach()
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
        if (this->llvm_type)
        {
            return (llvm::Value *)this->llvm_type;
        }

        std::vector<llvm::Type *> elements;
        elements.reserve(this->field_count);
        for (auto &prop : this->children)
        {
            if (typeid(prop) == typeid(StructField)) // review it works ?
            {
                auto field = (StructField *)prop;
                elements.push_back((llvm::Type *)field->get_type()->codegen(codegen, builder));
            }
        }

        auto st = llvm::StructType::create(codegen->context, this->get_name());
        st->setBody(elements);

        this->llvm_type = st;
        return (llvm::Value *)this->llvm_type;
    }

    //
    // Function
    //

    Function::Function(antlr4::ParserRuleContext *rule, Identifier *id, Type *return_type, bool is_intrinsic) : Type(rule, Primitives::FUNCTION_TY)
    {
        LOGIA_ASSERT(id && "id parameters is required");
        NODE_TYPE_ASSERT(id, ast_types::IDENTIFIER);

        if (return_type == nullptr)
        {
            return_type = new Type(nullptr, Primitives::VOID_TY);
        }
        NODE_TYPE_ASSERT(return_type, ast_types::TYPE);

        this->push_child(id);          // get_name
        this->push_child(return_type); // get_return_type

        // NOTE need to push something as arguments will be +3
        if (is_intrinsic)
        {
            this->push_child(new NoOp()); // get_body
        }
        else
        {
            auto block = ast_create_block();
            block->type = (ast_types)(ast_types::FUNCTION | ast_types::BODY);
            this->push_child(block); // get_body
        }
        this->intrinsic = is_intrinsic;
    }

    Function::~Function()
    {
        // TODO
    }

    std::string Function::to_string()
    {
        std::string list;
        // concat each parameter type
        for (auto &param : this->parameters)
        {
            if (!list.empty())
            {
                list += ", ";
            }
            list += param.type->to_string();
        }
        return std::format("Type[{} function {} ({})] ({:p})", this->get_return_type()->to_string(), this->get_name(), list, static_cast<void *>(this->parent_node));
    }

    const char *Function::get_name()
    {
        return get_identifier()->identifier;
    }

    Identifier *Function::get_identifier()
    {
        return (Identifier *)this->children[0];
    }

    Type *Function::get_return_type()
    {
        return (Type *)this->children[1];
    }

    Block *Function::get_body()
    {
        return (Block *)this->children[2];
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
    void Function::on_after_attach()
    {
        // only once, when a type is used it will be attached many times as references
        if (!this->is_attached)
        {
            this->is_attached = true;

            this->__register_type(this->get_name());

            // register parameters
            for (auto param : this->parameters)
            {
                this->get_body()->set(param.name->identifier, param.type);
            }
        }
    }

    llvm::Value *Function::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        int pcount = this->parameters.size();
        this->parametersIR.reserve(pcount);
        for (int i = 0; i < pcount; ++i)
        {
            this->parametersIR.push_back((llvm::Type *)this->parameters[i].type->codegen(codegen, builder));
        }
        auto rtype = this->get_return_type();
        rtype->codegen(codegen, builder);
        this->llvm_type = (llvm::Type *)llvm::FunctionType::get(rtype->llvm_type,
                                                                this->parametersIR, // parameter list
                                                                false);             // not variadic

        this->functionIR = llvm::Function::Create((llvm::FunctionType *)this->llvm_type, llvm::Function::ExternalLinkage, 0, this->get_name(), codegen->module.get());

        // Create a basic block and insert a return

        if (!this->intrinsic)
        {
            auto block = this->get_body();
            LOGIA_ASSERT(typeid(*block) == typeid(Block) && "Invalid function body type");

            auto BB = (llvm::BasicBlock *)block->create_llvm_block(codegen, (char *)"entry");
            BB->insertInto(this->functionIR);
            block->codegen(codegen, builder);
        }

        return (llvm::Value *)this->llvm_type;
    }

    LOGIA_API LOGIA_LEND Function *ast_create_function_type(Identifier *id, Type *return_type)
    {
        return new Function(nullptr, id, return_type);
    }

    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, Identifier *id, Type *return_type)
    {
        LOGIA_ASSERT(program);
        NODE_TYPE_ASSERT(program, ast_types::PROGRAM);

        auto f = new Function(nullptr, id, return_type, true);

        // NOTE it will attach itself to program scope
        program->unshift_child(f);

        return f;
    }

    void Function::add_param(Type *param_type, Identifier *param_name, Expression *param_default_value)
    {
        LOGIA_ASSERT(this->isFunction());
        LOGIA_ASSERT(!this->is_attached && "Function type should be created before attached");

        NODE_TYPE_ASSERT(param_type, ast_types::TYPE);
        NODE_TYPE_ASSERT(param_name, ast_types::IDENTIFIER);

        // TODO REVIEW default values are not push ?
        if (param_default_value)
        {
            NODE_TYPE_ASSERT(param_default_value, ast_types::EXPRESSION | ast_types::CONST);
            param_default_value->parent_node = this;
        }
        auto param = this->parameters.emplace_back(param_name, param_type, param_default_value);
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
        NODE_TYPE_ASSERT(type, ast_types::TYPE);
        LOGIA_ASSERT(name && "name is required for fields");
        NODE_TYPE_ASSERT(name, ast_types::IDENTIFIER);

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
}
