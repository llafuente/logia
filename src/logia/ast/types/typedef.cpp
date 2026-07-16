
#include "logia/ast/types/typedef.h"
#include "logia/ast/types/struct.h"

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
    // REVIEW, it's a type but it's definition, need to  distinguish both ?
    TypeDef::TypeDef() : Type({})
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

    std::string TypeDef::get_repr()
    {
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
            TypeDecl *current_type = nullptr;
            for (auto node : this->children)
            {
                if (node->try_cast(&ident))
                {
                    if (current_type == nullptr)
                    {
                        auto list = ident->scope_search(false, false);
                        current_type = list[0]->get_type_decl();
                        ident->set_type(current_type);
                    }
                    else
                    {
                        // go deeper in the type!
                        Struct *st;
                        if (current_type->try_cast(&st))
                        {
                            auto field = st->get_field(ident->identifier);
                            if (field == nullptr)
                            {
                                throw_semantic_error(this, std::format("unexpected type '{}' do not have a property named: '{}'", current_type->get_repr(), ident->identifier));
                            }
                            if (field->real_type == nullptr)
                            {
                                field->type_inference(TYPE_INFERENCE_EARLY);
                            }
                            current_type = field->get_type_decl();
                            if (current_type == nullptr)
                            {
                                return false;
                            }
                            ident->set_type(current_type);
                        }
                        else
                        {
                            throw_semantic_error(this, std::format("unexpected type '{}' do not have a property named: '{}'", current_type->get_repr(), ident->identifier));
                        }
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
                throw_compiler_error("Unpextected only child is not an identifier!");
            }
        }
        break;
        case 2:
        {
            // xxx.yyy -> ok!
            if (children[0]->is<Identifier>() && children[1]->is<Identifier>())
            {
            }
            else if (children[0]->is<Identifier>() && children[1]->is<TypeDef>())
            {
            }
            else if (children[0]->is<Identifier>() && children[1]->is<TemplateDef>())
            {
            }
            else
            {
                throw_compiler_error("Unpextected children types!");
            }
        }
        break;
        default:
            throw_compiler_error("Unpected children count");
        }
    }
}