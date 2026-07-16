#include "logia/ast/types/struct.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/identifier.h"
#include "logia/ast/expr.h"
#include "logia/ast/types/function.h"
#include "logia/ast/types/ref.h"

#include "llvm/IR/DerivedTypes.h" // StructType

namespace logia::AST
{

    //
    // StructAlias
    //
    StructAlias::StructAlias(location loc, Identifier *from, Identifier *to, const char *_docstring) : docstring(_docstring), Node(loc)
    {
        this->push_child(from);
        from->has_type = false;
        from->skip_codegen = true;
        from->type_inference_pass_id = TYPE_INFERENCE_MAX;

        this->push_child(to);
        to->has_type = false;
        to->skip_codegen = true;
        to->type_inference_pass_id = TYPE_INFERENCE_MAX;
    }

    Identifier *StructAlias::get_from()
    {
        return this->get_child<Identifier>(0);
    }

    Identifier *StructAlias::get_to()
    {
        return this->get_child<Identifier>(1);
    }
    void StructAlias::on_after_attach() {}

    void StructAlias::validate()
    {
        // check target exists!
        auto st = this->parent_node->as<Struct>();
        auto ident = this->get_to();

        // TODO we may want to support method aliasing too... maybe another search here is enough!
        this->target = st->get_field(ident->identifier);
        if (this->target == nullptr)
        {
            throw_semantic_error(this, std::format("LGERR_ST002 Alias target '{}' not found.\nDeclared {}", ident->identifier, this->loc.get_debug_location()));
        }
        // if found we have the same type!
        this->is_typed = true;
    }

    std::string StructAlias::to_string()
    {
        return std::format("StructAlias{}", Node::to_string());
    }

    Type *StructAlias::get_type()
    {
        return this->target == nullptr ? nullptr : this->target->get_type();
    }

    void StructAlias::_on_set_type(TypeDecl *ty)
    {
    }

    //
    // StructField
    //

    StructField::StructField(location loc,
                             uint32_t index,
                             Identifier *name,
                             Type *type,
                             Expression *default_value,
                             const char *docstring) : docstring(docstring), Node(loc), index(index)
    {
        this->is_typed = true;

        this->push_child(name); // 0
        name->skip_codegen = true;
        name->type_inference_pass_id = TYPE_INFERENCE_MAX;

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
        return this->get_child<Type>(1);
    }

    Expression *StructField::get_default_value()
    {
        return this->children.size() == 2 ? nullptr : this->get_child<Expression>(2);
    }

    void StructField::on_after_attach() {}

    void StructField::validate() {}

    std::string StructField::to_string()
    {
        return std::format("StructField{}", Node::to_string());
    }

    bool StructField::type_inference(size_t pass_id)
    {
        switch (pass_id)
        {
        case TYPE_INFERENCE_PRE:
        {
            auto ty = this->get_type();
            auto tyd = ty->get_type_decl();
            if (tyd == nullptr)
            {
                return false;
            }

            this->set_type(tyd);
        }
        break;
        }
        return true;
    }

    void StructField::_on_set_type(TypeDecl *tyd)
    {
        this->get_name()->set_type(tyd);
        auto default_value = this->get_default_value();
        if (default_value != nullptr)
        {
            default_value->set_type(tyd);
        }
    }

    //
    // Struct
    //

    Struct::Struct(location loc, Identifier *name) : TypeDecl(loc, Primitives::STRUCT_TY)
    {
        // TODO REVIEW type-system do not use: set_type atm
        this->real_type = this;
        this->is_typed = true;

        LOGIA_VERIFY(name != nullptr);

        name->skip_codegen = true;
        name->type_inference_pass_id = TYPE_INFERENCE_MAX;
        // NOTE the following stmt order is important :)
        this->push_child(name);
        name->set_type(this);
    }

    const char *Struct::get_name()
    {
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
            throw_semantic_error(id, std::format("Field not found: '{}' at '{}'", id->identifier, this->get_repr()));
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

    Node *Struct::get_property(const char *id)
    {
        // this are the types we can return
        StructAlias *sa;
        StructField *sf;
        Function *fn;

        uint32_t count = 0;
        for (const auto &ptr : this->children)
        {
            if (ptr->try_cast<StructAlias>(&sa))
            {
                if (sa->get_from()->operator==(id))
                {
                    return this->get_field(id);
                }
            }
            else if (ptr->try_cast<StructField>(&sf))
            {
                if (sf->get_name()->operator==(id))
                {
                    return sf;
                }
            }
            else if (ptr->try_cast<Function>(&fn))
            {
                if (fn->get_identifier()->operator==(id))
                {
                    return fn;
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
                auto ty = field->get_type_decl();
                list += std::format("{} {}", ty == nullptr ? "??" : ty->get_repr(), field->get_name()->identifier);
            }
        }

        return std::format("struct {} {{{}}}", this->get_name(), list);
    }

    TypeDecl *Struct::get_effective_type_decl()
    {
        if (this->field_count == 1)
        {
            auto field = this->get_field("λ");
            if (field != nullptr)
            {
                return field->get_type_decl()->get_effective_type_decl(); // recurse!
            }
        }
        return this;
    }

    void Struct::on_after_attach()
    {
        this->__register_type(this->get_name());
    }

    void Struct::pre_codegen(logia::Backend *backend)
    {
        // like functions we should generate the type asap
        // we can fill it later!
        this->ir_type = this->ir_struct = llvm::StructType::create(backend->context, this->get_name());

        std::vector<llvm::Type *> elements;
        elements.reserve(this->field_count);
        StructField *field;
        for (auto &prop : this->children)
        {
            if (prop->try_cast(&field))
            {
                auto tyd = field->get_type_decl()->get_effective_type_decl();
                tyd->pre_codegen(backend);
                LOGIA_VERIFY(tyd->ir_type);

                elements.push_back(tyd->ir_type);
            }
        }

        this->ir_struct->setBody(elements);

        Type::pre_codegen(backend);
    }

    void Struct::post_codegen(logia::Backend *backend)
    {
        LOG(DBG, "{}", this->to_string());
        // cache, because type are unique and we will be visiting this a lot
        if (this->is_post_codegen)
        {
            return;
        }

        // To avoid possible infinite recursions we should have the type defined before use
        // that's why we generate methods after the struct has llvm type
        Type::post_codegen(backend);

        Function *fn;
        for (auto &prop : this->children)
        {
            if (prop->try_cast(&fn))
            {
                fn->post_codegen(backend);
            }
        }
    }

    void Struct::add_field(
        location loc,
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

        this->push_child(new StructField(loc, this->field_count++, name, type, default_value, docstring));
    }

    void Struct::add_alias(location loc, Identifier *from, Identifier *to, const char *docstring)
    {
        LOGIA_VERIFY(from != nullptr);
        LOGIA_VERIFY(to != nullptr);

        // TODO exists to ?
        // TODO exists from ?

        this->push_child(new StructAlias(loc, from, to, docstring));
        ++this->alias_count;
    }

    void Struct::add_method(Function *fn)
    {
        fn->insert_parameter(0, new FunctionParameter(new Identifier({}, "this"), new Ref(this)));
        fn->is_method = true;
        this->push_child(fn);
        ++this->method_count;
    }

    void Struct::validate()
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
                            throw_semantic_error(second_field, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_field->get_name()->identifier, first_field->loc.get_debug_location(), second_field->loc.get_debug_location()));
                        }
                    }
                    else if (second->try_cast<StructAlias>(&second_alias))
                    {
                        if (first_field->get_name()->operator==(second_alias->get_from()))
                        {
                            throw_semantic_error(second_alias, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_field->get_name()->identifier, first_field->loc.get_debug_location(), second_alias->loc.get_debug_location()));
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
                            throw_semantic_error(second_field, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_alias->get_from()->identifier, first_alias->loc.get_debug_location(), second_field->loc.get_debug_location()));
                        }
                    }
                    else if (second->try_cast<StructAlias>(&second_alias))
                    {
                        if (first_alias->get_from()->operator==(second_alias->get_from()))
                        {
                            throw_semantic_error(second_alias, std::format("LGERR_ST001 Redeclaration of field name '{}'.\nFirst declaration {}\nSecond declaration {}", first_alias->get_from()->identifier, first_alias->loc.get_debug_location(), second_alias->loc.get_debug_location()));
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
                    throw_semantic_error(first_alias, std::format("LGERR_ST002 Alias target '{}' not found.\nDeclared {}", first_alias->get_to()->identifier, first_alias->loc.get_debug_location()));
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
} // namespace logia::AST
