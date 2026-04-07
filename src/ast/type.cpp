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

    std::string Type::to_string()
    {
        std::string list;
        switch (this->primitive)
        {
        case Primitives::FUNCTION_TY:
            // concat each parameter type
            for (auto &param : this->Function.parameters)
            {
                if (!list.empty())
                {
                    list += ", ";
                }
                list += param.type->to_string();
            }
            return std::string("Type[") + this->Function.return_type->to_string() + " function " + this->Function.name + "(" + list + ")" + "]";
        case Primitives::STRUCT_TY:
            // concat all properties with their type
            for (auto &prop : this->Struct.properties)
            {
                if (prop.isField())
                {
                    if (!list.empty())
                    {
                        list += ", ";
                    }
                    list += prop.type->to_string();
                }
            }
            return std::string("Type[struct ") + this->Struct.name + " {" + list + "}";
        }

        return std::string("Type[") + ast_primitives_to_string(this->primitive) + "]";
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

            auto parentBody = (Block *)ast_find_closest_parent(this, ast_types::BODY);
            LOGIA_ASSERT(parentBody);

            if (this->isFunction())
            {
                parentBody->set(this->Function.name, this);
                // aka not intrinsic
                if (this->Function.body != nullptr)
                {
                    this->unshift_child(this->Function.body);
                }
            }
            else if (this->isStruct())
            {
                parentBody->set(this->Struct.name, this);
            }
            // TODO enum, if needed :)

            // the rest of types has no name, because primitives are handled at ast_create_program
        }
    }

    llvm::Value *Type::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder)
    {
        DEBUG() << this->to_string() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir)
        {
            return (llvm::Value *)this->ir;
        }

        switch (this->primitive)
        {
        case Primitives::FUNCTION_TY:
        {

            int pcount = this->Function.parameters.size();
            this->Function.parametersIR.reserve(pcount);
            for (int i = 0; i < pcount; ++i)
            {
                this->Function.parametersIR.push_back((llvm::Type *)this->Function.parameters[i].type->codegen(codegen, builder));
            }
            this->Function.return_type->codegen(codegen, builder);
            this->ir = (llvm::Type *)llvm::FunctionType::get(this->Function.return_type->ir,
                                                             this->Function.parametersIR, // parameter list
                                                             false);                      // not variadic

            this->Function.functionIR = llvm::Function::Create((llvm::FunctionType *)this->ir, llvm::Function::ExternalLinkage, 0, this->Function.name, codegen->module.get());

            // Create a basic block and insert a return

            if (!this->Function.intrinsic)
            {
                LOGIA_ASSERT(this->Function.body);

                auto BB = (llvm::BasicBlock *)this->Function.body->create_llvm_block(codegen, (char *)"entry");
                BB->insertInto(this->Function.functionIR);
                this->Function.body->codegen(codegen, builder);
            }

            return (llvm::Value *)this->ir;
        }
        break;
        case Primitives::STRUCT_TY:
        {
            auto max = this->Struct.properties.size();
            std::vector<llvm::Type *> elements;
            elements.reserve(max);
            for (int i = 0; i < max; ++i)
            {
                auto p = &this->Struct.properties[i];
                if (p->isField())
                {
                    elements.push_back((llvm::Type *)p->type->codegen(codegen, builder));
                }
            }
            auto st = llvm::StructType::create(codegen->context, this->Struct.name);
            st->setBody(elements);

            this->ir = st;
            return (llvm::Value *)this->ir;
        }
        break;
        }

        // TODO
        throw std::exception("TODO!");
    }

    LOGIA_API LOGIA_LEND Type *ast_create_function_type(char *name, Type *return_type)
    {
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, Primitives::FUNCTION_TY);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.body = ast_create_block();
        t->Function.body->type = (ast_types)(ast_types::FUNCTION | ast_types::BODY);

        return t;
    }

    LOGIA_API LOGIA_LEND Type *ast_create_instrinsic(Program *program, char *name, Type *return_type)
    {
        LOGIA_ASSERT(program);
        LOGIA_ASSERT(name);
        LOGIA_ASSERT(return_type);

        Type *t = new Type(nullptr, Primitives::FUNCTION_TY);
        new (&t->Function) FunctionType();
        t->Function.name = name;
        t->Function.return_type = return_type;
        t->Function.intrinsic = true;
        t->Function.body = nullptr;

        program->set(name, t);
        program->unshift_child(t);

        return t;
    }

    LOGIA_API void ast_function_add_param(Type *s, Type *param_type, char *param_name, Expression *param_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isFunction());
        // LOGIA_ASSERT(*prop_name);

        auto param = s->Function.parameters.emplace_back(param_name, param_type, param_default_value);
        // TODO REVIEW default values are not push ?
        if (param_default_value)
        {
            param_default_value->parent_node = s;
        }
        s->Function.body->set(param_name, param_type);
    }

    LOGIA_API LOGIA_LEND Type *ast_create_struct_type(char *name)
    {
        LOGIA_ASSERT(name);

        Type *t = new Type(nullptr, Primitives::STRUCT_TY);
        new (&t->Struct) StructType();
        t->Struct.name = name;

        return t;
    }

    LOGIA_API void ast_struct_add_field(Type *s, Type *prop_type, char *prop_name, Expression *prop_default_value)
    {
        LOGIA_ASSERT(s);
        LOGIA_ASSERT(s->isStruct());
        // LOGIA_ASSERT(*prop_name);

        auto prop = s->Struct.properties.emplace_back(prop_name, nullptr, StructPropertyType::STRUCT_PROPERTY_TYPE_FIELD, prop_type, prop_default_value);
        // TODO REVIEW default values are not push ?
        if (prop_default_value)
        {
            prop_default_value->parent_node = s;
        }
    }

}