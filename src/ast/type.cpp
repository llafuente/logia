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
    // Struct
    //

    Struct::Struct(antlr4::ParserRuleContext *rule, Identifier *id) : Type(rule, Primitives::STRUCT_TY)
    {
        LOGIA_ASSERT(id && "id parameters is required");
        NODE_TYPE_ASSERT(id, ast_types::IDENTIFIER);

        this->push_child(id); // get_name
    }

    char *Struct::get_name()
    {
        return get_identifier()->identifier;
    }

    Identifier *Struct::get_identifier()
    {
        return (Identifier *)this->children[0];
    }

    Identifier *Struct::get_alias_to(Identifier *from)
    {
        for (auto &prop : this->aliases)
        {
            if (strcmp(from->identifier, prop.from->identifier) == 0)
            {
                return prop.to;
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
        for (auto &prop : this->fields)
        {
            if (strcmp(id->identifier, prop.name->identifier) == 0)
            {
                return count;
            }
            ++count;
        }
        return -1;
    }

    std::string Struct::to_string()
    {
        std::string list;
        // concat all properties with their type
        for (auto &prop : this->fields)
        {
            if (!list.empty())
            {
                list += ", ";
            }
            list += prop.type->to_string();
        }

        return std::format("Type[struct {}] {} ({:p})", this->get_name(), list, static_cast<void *>(this->parent_node));
    }

    void Struct::on_after_attach()
    {
        // once guard
        if (!this->is_attached)
        {
            this->is_attached = true;

            auto parentBody = (Block *)ast_find_closest_parent(this, ast_types::BODY);
            LOGIA_ASSERT(parentBody);

            parentBody->set(this->get_name(), this);
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

        auto max = this->fields.size();
        std::vector<llvm::Type *> elements;
        elements.reserve(max);
        for (int i = 0; i < max; ++i)
        {
            auto t = this->fields[i].type;
            elements.push_back((llvm::Type *)t->codegen(codegen, builder));
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

    char *Function::get_name()
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

            auto parentBody = (Block *)ast_find_closest_parent(this, ast_types::BODY);
            parentBody->set(this->get_name(), this);

            // register params
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

    void Struct::add_field(Type *prop_type, Identifier *prop_name, Expression *prop_default_value)
    {
        LOGIA_ASSERT(this);
        LOGIA_ASSERT(this->isStruct());

        LOGIA_ASSERT(prop_type && "type is required for fields");
        NODE_TYPE_ASSERT(prop_type, ast_types::TYPE);
        LOGIA_ASSERT(prop_name && "name is required for fields");
        NODE_TYPE_ASSERT(prop_name, ast_types::IDENTIFIER);

        auto prop = this->fields.emplace_back(prop_name, prop_type, prop_default_value, nullptr);
        // TODO REVIEW default values are not push ?
        if (prop_default_value)
        {
            prop_default_value->parent_node = this;
        }
    }

    void Struct::add_alias(Identifier *from, Identifier *to)
    {
        LOGIA_ASSERT(from);
        LOGIA_ASSERT(to);

        // TODO exists to ?
        // TODO exists from ?

        auto prop = this->aliases.emplace_back(from, to);
    }

}
