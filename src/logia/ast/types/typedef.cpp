
#include "logia/ast/types/typedef.h"

#include "logia/ast/types/templatedef.h"
#include "logia/ast/types/struct.h"
#include "logia/ast/types/ref.h"
#include "logia/ast/types/vec.h"

#include "utils.h"
#include "logia/type_inference.h"
#include "logia/backend.h"
#include "logia/ast/constexpr.h"
#include "logia/ast/callexpr.h"
#include "logia/ast/identifier.h"
#include "logia/ast/functionblock.h"
#include "logia/ast/returnstmt.h"
#include "logia/ast/scope.h"

#include "llvm/IR/Type.h"              // Type
#include "llvm/IR/DebugInfoMetadata.h" // dwarf

namespace logia::AST
{
    TypeDef::TypeDef(location loc) : Type(loc)
    {
        this->has_type = true;
        this->is_typed = false;
    }

    TypeDef::~TypeDef() {}

    void TypeDef::add_identifier(Identifier *name)
    {
        name->skip_codegen = true;
        name->resolve = false;

        this->push_child(name);
    }

    void TypeDef::add_type_def(TypeDef *td)
    {
        this->push_child(td);
    }

    void TypeDef::add_template_def(TemplateDef *tdef)
    {
        if (this->children.size() == 0)
        {
            throw_compiler_error("template def cannot be the first children!");
        }

        this->push_child(tdef);
    }

    Type *TypeDef::get_type()
    {
        // this could happens while parsing, not a real issue but avoid some null-access
        if (!this->is_attached)
        {
            return nullptr;
        }

        return this->real_type;
    }

    std::string TypeDef::to_string()
    {
        Identifier *ident;
        TypeDef *tydef;
        TemplateDef *tdef;

        std::string dot_locatots;
        for (auto node : this->children)
        {
            dot_locatots += dot_locatots.size() ? "," : "";
            if (node->try_cast(&ident))
            {
                dot_locatots += ident->identifier;
            }
            else if (node->try_cast(&tydef))
            {
                dot_locatots += tydef->get_repr();
            }
            else if (node->try_cast(&tdef))
            {
                dot_locatots += tdef->get_repr();
            }
        }

        return std::format("TypeDef[{}]{}", dot_locatots, Node::to_string());
    }

    std::string TypeDef::to_code(size_t ident)
    {
        std::string dot_locatots;
        for (auto node : this->children)
        {
            dot_locatots += dot_locatots.size() ? "." : "";
            dot_locatots += node->to_code();
        }

        return std::format("{}", dot_locatots);
    }

    std::string TypeDef::get_repr()
    {

        auto left = this->children[0]->to_code();
        if (this->children.size() > 1)
        {
            auto right = this->children[1];
            if (right->is<TemplateDef>())
            {
                return std::format("{}<{}>", left, right->to_code());
            }
            if (right->is<Identifier>())
            {
                return std::format("{}.{}", left, right->to_code());
            }
        }

        std::string t;
        Identifier *ident;
        for (auto node : this->children)
        {
            if (node->try_cast(&ident))
            {
                t += (t.size() ? "." : "");
                t += ident->identifier;
            }
        }

        return t;
    }

    void TypeDef::post_codegen(logia::Backend *backend)
    {
        this->get_type()->post_codegen(backend);
        return Node::post_codegen(backend);
    }

    void TypeDef::on_after_attach()
    {
    }

    TypeDecl *TypeDef::resolve_identifier(Identifier *ident)
    {
        // TODO REVIEW TEMPORARY ref/vec
        // we need to hook ref and vec here by hand, because we cannot use struct+tpl atm
        // maybe in the future this code can be removed and use the same "hack" and integer/float struct -> primitive

        if (ident->operator==("ref"))
        {
            LOGIA_VERIFY(this->parent_node->is<TypeDef>() == true);
            return new Ref(); // pointee will be set later!
        }
        if (ident->operator==("vec"))
        {
            LOGIA_VERIFY(this->parent_node->is<TypeDef>() == true);
            return new Vec(); // pointee will be set later!
        }

        auto list = ident->scope_search(false, false);
        return list[0]->get_type_decl();
    }

    TypeDecl *TypeDef::resolve_property(TypeDecl *tyd, Identifier *ident)
    {
        // TODO function!
        // go deeper in the type!
        Struct *st;
        if (tyd->try_cast(&st))
        {
            auto field = st->get_field(ident->identifier);
            if (field == nullptr)
            {
                throw_semantic_error(this, std::format("unexpected type '{}' do not have a property named: '{}'", tyd->get_repr(), ident->identifier));
            }
            if (field->real_type == nullptr)
            {
                field->type_inference(TYPE_INFERENCE_EARLY);
            }
            return field->get_type_decl();
        }
        else
        {
            throw_semantic_error(this, std::format("unexpected type '{}' do not have a property named: '{}'", tyd->get_repr(), ident->identifier));
        }
    }

    bool TypeDef::type_inference(size_t pass_id)
    {
        // struct/ref/vec + template
        switch (pass_id)
        {
        case TYPE_INFERENCE_EARLY:
        {
            LOG(DBG, "resolve({})", this->children.size());
            LOG(DBG, "{}", this->to_string_tree());
            Identifier *ident;
            TemplateDef *tpldef;
            TypeDef *tydef;
            TypeDecl *current_type = nullptr;
            for (auto node : this->children)
            {
                // NOTE TemplateDef should be above TypeDef because it's its children!
                if (node->try_cast(&tpldef))
                {
                    Ref *r;
                    Vec *v;
                    if (current_type->try_cast(&r))
                    {
                        LOGIA_VERIFY(tpldef->children.size() == 1);
                        r->pointee = tpldef->children[0]->get_type_decl(); // set Ref type
                        if (r->pointee == nullptr)
                        {
                            return false;
                        }
                    }
                    else if (current_type->try_cast(&v))
                    {
                        LOGIA_VERIFY(tpldef->children.size() == 1);
                        v->pointee = tpldef->children[0]->get_type_decl(); // set Vec type
                        if (v->pointee == nullptr)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        // TODO implement templates
                        throw_compiler_error("to-do - implement template!");
                    }
                }
                else if (node->try_cast(&tydef))
                {
                    // forward typedef
                    current_type = tydef->get_type_decl();
                    if (current_type == nullptr)
                    {
                        return false;
                    }
                }
                else if (node->try_cast(&ident))
                {
                    if (current_type == nullptr)
                    {
                        current_type = this->resolve_identifier(ident);
                        if (current_type == nullptr)
                        {
                            return false;
                        }
                        ident->set_type(current_type);
                    }
                    else
                    {
                        current_type = this->resolve_property(current_type, ident);
                        if (current_type == nullptr)
                        {
                            return false;
                        }
                        ident->set_type(current_type);
                    }
                }
                else
                {
                    LOG_ERR("{}", node->to_string());
                    throw_compiler_error("unsupported child node type found");
                }
            }
            LOGIA_VERIFY(current_type != nullptr);
            // TODO REVIEW type-system do not use: set_type atm
            this->real_type = current_type;
            this->is_typed = true;
        }
        break;
        }
        return true;
    }

    void TypeDef::validate()
    {
        // ident
        // ident + typedef
        // ident.template
        switch (children.size())
        {
        case 0:
        {
            throw_compiler_error("Empty typedef");
        }
        break;
        case 1:
        {
            if (!children[0]->is<Identifier>())
            {
                throw_compiler_error("Typedef with only one child is expected to be and identifier!");
            }
        }
        break;
        case 2:
        {

            if (children[0]->is<Identifier>())
            {
                // with Identifier left, typedef cannot be right (because we are left recursing)
                // ref<i8>
                // point.x

                if (children[1]->is<Identifier>() || children[1]->is<TemplateDef>())
                {
                    return;
                }
            }

            if (children[0]->is<TypeDef>())
            {
                // with a typedef left, identifier / typedef
                // point<float>.x
                // point.operator+<float>    // <-- this makes little sense ? point<float>.operator+

                // struct point<$t> { $t x, $t y}
                // struct rectangle<$t> { point<$t> start, point<$t> end}
                // rectangle<float>.point.x

                // struct v2<$t> { $t x, $t y}
                // struct v3<$t> extends v2<$t> { $t z }
                // rectangle<float>.point.x

                // struct a<$t> { $t x, $t y}
                // struct b { $t z }
                // rectangle<float>.point.x

                if (children[1]->is<Identifier>() || children[1]->is<TemplateDef>())
                {
                    return;
                }
            }

            if (children[1]->is<TypeDef>())
            {
                throw_compiler_error("Unexpected second child cannot be TypeDef");
            }

            throw_compiler_error("Unpextected children types!");
        }
        break;
        default:
            throw_compiler_error("Unpected children count");
        }
    }
}