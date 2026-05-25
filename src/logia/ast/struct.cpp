#include "logia/ast/struct.h"
#include "logia/ast/identifier.h"
#include "logia/ast/expr.h"

namespace logia::AST
{

    //
    // Struct
    //
    StructAlias::StructAlias(antlr4::ParserRuleContext *rule, Identifier *from, Identifier *to, const char *_docstring) : docstring(_docstring), Type(rule, Primitives::NONE)
    {
        this->is_typed = true;

        this->push_child(from);
        from->has_type = false;
        from->skip_codegen = true;
        from->skip_type_inference = true;

        this->push_child(to);
        to->has_type = false;
        to->skip_codegen = true;
        to->skip_type_inference = true;
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
        return std::format("StructAlias{}", Node::to_string());
    }
    Type *StructAlias::get_type()
    {
        auto owner = this->parent_node->as<Struct>();
        // TODO alias of methods ?
        return owner->get_field_type(this->get_to());
    }

    StructField::StructField(antlr4::ParserRuleContext *rule,
                             uint32_t index,
                             Identifier *name,
                             Type *type,
                             Expression *default_value,
                             const char *docstring) : docstring(docstring), Type(rule, Primitives::NONE), index(index)
    {
        this->is_typed = true;

        this->push_child(name); // 0
        name->skip_codegen = true;
        name->skip_type_inference = true;
        name->set_type(type);

        this->push_child(type); // 1
        if (default_value != nullptr)
        {
            this->push_child(default_value); // 2
        }
    }
    Identifier *StructField::get_name()
    {
        return this->get_child<Identifier>(0);
    }
    Type *StructField::get_type()
    {
        return this->get_child<Type>(1)->get_final_type();
    }
    Expression *StructField::get_default_value()
    {
        return this->children.size() == 2 ? nullptr : this->get_child<Expression>(2);
    }
    std::string StructField::to_string()
    {
        return std::format("StructField{}", Node::to_string());
    }

    void StructField::_pre_type_inference()
    {
        auto ty = this->get_type();
        auto default_value = this->get_default_value();
        if (default_value != nullptr)
        {
            default_value->set_type(ty);
        }
        Type::_pre_type_inference();
    }

    //
    // Struct
    //

    Struct::Struct(antlr4::ParserRuleContext *rule, Identifier *id) : Type(rule, Primitives::STRUCT_TY)
    {
        this->is_typed = true;
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
        id->skip_codegen = true;
        id->set_type(this);
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

    Identifier *Struct::get_alias_to(const char *from)
    {
        for (const auto &ptr : this->children)
        {
            if (auto alias = dynamic_cast<StructAlias *>(ptr))
            {
                if (*alias->get_from() == from)
                {
                    return alias->get_to();
                }
            }
        }

        return nullptr;
    }

    StructField *Struct::get_field(Identifier *id)
    {
        auto field = this->get_field(id->identifier);
        if (field == nullptr)
        {
            throw_semantic_error(id, "Field not found: '{}' at '{}'", id->identifier, this->get_repr());
        }
        return field;
    }

    StructField *Struct::get_field(const char *id)
    {
        auto to = this->get_alias_to(id);
        if (to != nullptr)
        {
            id = to->identifier;
        }

        uint32_t count = 0;
        for (const auto &ptr : this->children)
        {
            if (auto field = dynamic_cast<StructField *>(ptr))
            {
                if (*field->get_name() == id)
                {
                    return field;
                }
            }
        }
        return nullptr;
    }

    Type *Struct::get_field_type(Identifier *id)
    {
        return this->get_field(id->identifier)->get_type();
    }

    StructField *Struct::get_field_by_index(uint32_t index)
    {
        StructField *field;
        for (const auto &ptr : this->children)
        {
            if (ptr->try_cast<StructField>(&field))
            {
                if (field->index == index)
                {
                    return field;
                }
            }
        }

        throw_compiler_error(std::format("index {} out of bounds", index));
    }

    Type *Struct::get_type()
    {
        return this;
    }

    std::string Struct::to_string()
    {
        return std::format("Type.Struct {}", Node::to_string());
    }
    std::string Struct::get_repr()
    {
        std::string list = "";

        StructField *field;
        for (const auto &ptr : this->children)
        {
            if (ptr->try_cast<StructField>(&field))
            {
                if (!list.empty())
                {
                    list += ", ";
                }
                list += std::format("{} {}", field->get_name()->identifier, field->get_final_type()->get_repr());
            }
        }

        return std::format("struct {} {{{}}}", this->get_name(), list);
    }

    void Struct::post_attach()
    {
        // once guard
        if (!this->is_attached)
        {
            // if the struct has a name -> attach it to body
            if (this->has_name)
            {
                this->__register_type(this->get_name());
            }

            this->semantic_validate();

            Type::post_attach();
        }
    }

    llvm::Value *Struct::post_codegen(logia::Backend *backend)
    {
        DEBUG() << this->to_string() << std::endl;
        // cache, because type are unique and we will be visiting this a lot
        if (this->ir_type)
        {
            return (llvm::Value *)this->ir_type;
        }

        std::vector<llvm::Type *> elements;
        elements.reserve(this->field_count);
        StructField *field;
        for (auto &prop : this->children)
        {
            if (prop->try_cast(&field))
            {
                elements.push_back((llvm::Type *)field->get_final_type()->codegen(backend));
            }
        }

        auto st = llvm::StructType::create(backend->context, this->get_name());
        st->setBody(elements);

        this->ir_type = st;
        return Type::post_codegen(backend);
    }

    void Struct::_pre_type_inference()
    {
        return Type::_pre_type_inference();
    }

    void Struct::add_field(
        antlr4::ParserRuleContext *rule,
        Identifier *name,
        Type *type,
        Expression *default_value,
        const char *docstring)
    {
        if (!type)
        {
            throw_compiler_error("type is required for fields");
        }
        if (!name)
        {
            throw_compiler_error("name is required for fields");
        }

        this->push_child(new StructField(rule, this->field_count++, name, type, default_value, docstring));
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

    void Struct::semantic_validate()
    {
        // check properties are unique!
        // check alias point to something!
        // check ailas do not collide with fields!
        StructAlias *first_alias, *second_alias;
        StructField *first_field, *second_field;
        for (const auto &first : this->children)
        {
            if (first->try_cast<StructField>(&first_field))
            {
                for (const auto &second : this->children)
                {
                    if (first == second)
                        continue; // skip self!

                    if (second->try_cast<StructField>(&second_field))
                    {
                        if (first_field->get_name()->operator==(second_field->get_name()))
                        {
                            throw_semantic_error(second_field, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_field->get_name()->identifier, first_field->get_debug_location(), second_field->get_debug_location()));
                        }
                    }
                    else if (second->try_cast<StructAlias>(&second_alias))
                    {
                        if (first_field->get_name()->operator==(second_alias->get_from()))
                        {
                            throw_semantic_error(second_alias, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_field->get_name()->identifier, first_field->get_debug_location(), second_alias->get_debug_location()));
                        }
                    }
                }
            }
            else if (first->try_cast<StructAlias>(&first_alias))
            {
                // duplication?
                for (const auto &second : this->children)
                {
                    if (first == second)
                        continue; // skip self!
                    if (second->try_cast<StructField>(&second_field))
                    {
                        if (first_alias->get_from()->operator==(second_field->get_name()))
                        {
                            throw_semantic_error(second_field, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_alias->get_from()->identifier, first_alias->get_debug_location(), second_field->get_debug_location()));
                        }
                    }
                    else if (second->try_cast<StructAlias>(&second_alias))
                    {
                        if (first_alias->get_from()->operator==(second_alias->get_from()))
                        {
                            throw_semantic_error(second_alias, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_alias->get_from()->identifier, first_alias->get_debug_location(), second_alias->get_debug_location()));
                        }
                    }
                }
                // target is valid
                auto found = false;
                for (const auto &second : this->children)
                {
                    if (second->try_cast<StructField>(&second_field))
                    {
                        if (first_alias->get_to()->operator==(second_field->get_name()))
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                {
                    throw_semantic_error(first_alias, std::format("LGERR_ST002 Alias target '{}' not found.\nDeclared {}", first_alias->get_to()->identifier, first_alias->get_debug_location()));
                }
            }
            else
            {
                // std::cerr << first->to_string_tree() << std::endl;
                // throw_compiler_error("what is this ???");
            }
            /*
                        if (auto alias = dynamic_cast<StructAlias *>(ptr))
                        {
                            if (*alias->get_from() == from)
                            {
                                return alias->get_to();
                            }
                        }
                        */
        }
    }

    LOGIA_API LOGIA_LEND Struct *ast_create_struct_type(Identifier *id)
    {
        return new Struct(nullptr, id);
    }

} // namespace logia::AST
