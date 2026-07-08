#include "logia/ast/node.h"

#include "logia/log.h"
#include "logia/type_inference.h"
#include "logia/ast/type.h"
#include "logia/ast/import.h"
#include "logia/ast/struct.h"
#include "logia/ast/program.h"

#include <format>
#include <iostream> // std::cerr

namespace logia::AST
{
    // utils
    bool __is_attached_to_program(Node *node)
    {
        // an attached node is the one that can reach program
        Program *program = nullptr;
        if (node->is<Program>() || node->try_first_parent<Program>(&program))
        {
            return true;
        }
        return false;
    }

    void __notify_attached_descendants(Node *node)
    {
        LOG(SILLY, "{}", node->to_string());
        if (!node->is_attached)
        {
            // node->on_before_attach();
            node->is_attached = true;
        }
        else
        {
            LOG(WRN, "invalid post_attach, node already attached ? {}", node->to_string());
        }

        node->foreach_descendant([](auto descendant, auto i) -> bool
                                 {
                                    if(!descendant->is_attached) {
                                        //descendant->on_before_attach();
                                        descendant->is_attached = true;
                                    } else {
                                        LOG(WRN, "invalid post_attach, node already attached ? {}", descendant->to_string());
                                    }
            return true; });

        node->on_after_attach();
        node->foreach_descendant([](auto descendant, auto i) -> bool
                                 {
                                    descendant->on_after_attach();
            return true; });
    }

    // impl
    // constructor
    Node::Node(location loc) : loc(loc)
    {
    }
    // copy constructor
    Node::Node(const Node &other)
    {
        // this->is_attached = other.is_attached;
        // this->is_validated = other.is_validated;
        this->skip_codegen = other.skip_codegen;
        // this->is_pre_codegen = other.is_pre_codegen;
        // this->is_post_codegen = other.is_post_codegen;
        this->has_type = other.has_type;
        this->is_typed = other.is_typed;
        this->is_constant = other.is_constant;
        this->loc = other.loc;

        // skip: parent_node, freezed, real_type, type_inference_pass_id, cg_value
        // TODO children ???
    }

    Node::~Node()
    {
        // TODO
    }

    std::string Node::to_string()
    {
        std::string flags = "";
        if (this->is_pre_codegen)
        {
            flags += (flags.length() ? "," : "");
            flags += "precg";
        }
        if (this->is_post_codegen)
        {
            flags += (flags.length() ? "," : "");
            flags += "poscg";
        }
        if (this->has_type)
        {
            flags += (flags.length() ? "," : "");
            flags += "wty";
        }
        if (this->is_typed)
        {
            flags += (flags.length() ? "," : "");
            flags += "typed";
        }
        if (this->skip_codegen)
        {
            flags += (flags.length() ? "," : "");
            flags += "skipcg";
        }
        if (this->is_attached)
        {
            flags += (flags.length() ? "," : "");
            flags += "attached";
        }

        std::string ty = "";
        if (!has_type)
        {
            ty = "no";
        }
        else if (is_typed && is_attached)
        {
            auto fty = this->get_final_type();
            if (fty != nullptr)
            {
                ty = fty->get_repr();
            }
            else
            {
                ty = "??";
            }
        }

        // show source code, but only if is a reasonable amount of code!
        auto d = this->loc.stop_index - this->loc.start_index;
        if (d > 0 && d < 64)
        {
            return std::format("| {} [@{}] ty={} [{},ti_pass_id={}] {}", static_cast<void *>(this), static_cast<void *>(this->parent_node), ty, flags, this->type_inference_pass_id, this->loc.get_source_code());
        }

        return std::format("| {} [@{}] ty={} [{},ti_pass_id={}]", static_cast<void *>(this), static_cast<void *>(this->parent_node), ty, flags, this->type_inference_pass_id);
    }

    void Node::push_child(Node *child)
    {
        if (freezed)
        {
            throw std::exception("Node is freezed");
        }
        children.push_back(child);
        child->parent_node = this;

        if (__is_attached_to_program(this))
        {
            __notify_attached_descendants(child);
        }
    }

    void Node::set_child(Node *child, size_t position)
    {
        LOG(DBG, "{}, {}", (void *)child, position);

        if (freezed)
        {
            throw std::exception("Node is freezed");
        }
        children[position] = child;
        child->parent_node = this;

        if (__is_attached_to_program(this))
        {
            __notify_attached_descendants(child);
        }
    }

    void Node::unshift_child(Node *child)
    {
        if (freezed)
        {
            throw std::exception("Node is freezed");
        }
        children.insert(children.begin(), child);
        child->parent_node = this;

        if (__is_attached_to_program(this))
        {
            __notify_attached_descendants(child);
        }
    }

    void Node::replace_self(Node *new_node)
    {
        auto parent = this->parent_node;
        std::replace(parent->children.begin(), parent->children.end(), this, new_node);
        new_node->parent_node = this;
        if (__is_attached_to_program(this))
        {
            __notify_attached_descendants(new_node);
        }
    }

    void Node::replace(Node *attached_node, Node *new_node)
    {
        std::replace(children.begin(), children.end(), attached_node, new_node);
        new_node->parent_node = this;
        if (__is_attached_to_program(this))
        {
            __notify_attached_descendants(new_node);
        }
    }

    std::string Node::to_string_tree(std::string padding, bool last_child)
    {
        std::string out;

        if (padding.length() == 0 || last_child)
        {
            // root
            out += std::format("{}{}\n", padding, this->to_string());
        }
        else
        {
            // children
            out += std::format("{}{}\n", padding, this->to_string());
        }

        // std::string out = std::format("{} {} (parent {:p}){},{}\n", padding, this->to_string(), static_cast<void *>(this->parent_node), this->is_pre_codegen ? "precg" : "", this->is_post_codegen ? "postcg" : "");

        padding += "  ";
        auto max = this->children.size();
        auto last = max - 1;
        for (auto i = 0; i < max; i++)
        {
            out += this->children[i]->to_string_tree(padding, last == i);
        }

        return out;
    }
    void Node::set_type(TypeDecl *tyd)
    {
        LOG(DBG, "{} <- {}", (void *)this, (void *)tyd);

        if (!this->has_type)
        {
            // REVIEW can we do anything ?!
        }

        if (this->real_type != tyd)
        {
            // TODO we should check this ? we may not
            // throw_compiler_error("Type already set");
            LOG(WRN, "{} Type already set / Type change!", (void *)this);
        }

        this->real_type = tyd;
        this->is_typed = true;

        this->_on_set_type(tyd);
    }
    /*
        void Node::_set_type(Type *ty)
        {
            LERROR() << this->to_string() << std::endl;
            throw std::runtime_error("set_type not supported for this type node");
        };
    */

    void Node::type_inference(size_t pass_id)
    {
        // skip ?
        if (this->type_inference_pass_id >= pass_id)
        {
            return;
        }

        switch (pass_id)
        {
        case TYPE_INFERENCE_EARLY:
            return this->_early_type_inference();
        case TYPE_INFERENCE_PRE:
            return this->_pre_type_inference();
        case TYPE_INFERENCE_POST:
            return this->_post_type_inference();
        }

        throw_compiler_error("unreachable!");
    }
    void Node::_early_type_inference()
    {
        this->type_inference_pass_id = TYPE_INFERENCE_EARLY;
    }

    void Node::_pre_type_inference()
    {
        this->type_inference_pass_id = TYPE_INFERENCE_PRE;
    }

    void Node::_post_type_inference()
    {
        this->type_inference_pass_id = TYPE_INFERENCE_POST;
    }

    TypeDecl *Node::get_final_type()
    {

        // this resolve types in the following manner
        // TypeDef -> get referenced type
        // Struct
        // * has one field with "λ" as name -> return the "λ" type
        // * return the struct
        // AnyOther -> return
        TypeDecl *tyd;
        Struct *s;

        auto type = this->get_type();
        if (this->real_type != nullptr)
        {
            type = this->real_type;
        }

        int MAX = 10;
        for (; MAX > 0; --MAX)
        {
            if (type->is<TypeDef>())
            {
                type = type->get_final_type();
            }
            else if (type->try_cast(&s))
            {
                if (s->field_count == 1)
                {
                    auto field = s->get_field("λ");
                    if (field != nullptr)
                    {
                        if (field->is_typed)
                        {
                            return field->get_final_type();
                        }
                        else
                        {
                            return nullptr;
                        }
                    }
                }
                return s;
            }
            else if (type->try_cast(&tyd))
            {
                return tyd;
            }
            else
            {
                // throw_compiler_error("wtf!");
                return nullptr;
            }
        }

        throw std::runtime_error(std::format("exceeded MAX iteration"));
    }

    void _get_pre_descendant(Node *node, std::vector<Node *> *out)
    {
        out->push_back(node);
        for (auto child : node->children)
        {
            _get_pre_descendant(child, out);
        }
    }

    std::vector<Node *> Node::get_pre_descendant()
    {
        std::vector<Node *> out;
        _get_pre_descendant(this, &out);
        return out;
    }

    void _get_post_descendant(Node *node, std::vector<Node *> *out)
    {
        for (auto child : node->children)
        {
            _get_post_descendant(child, out);
        }
        out->push_back(node);
    }

    std::vector<Node *> Node::get_post_descendant()
    {
        std::vector<Node *> out;
        _get_post_descendant(this, &out);
        return out;
    }

    void Node::pre_codegen(logia::Backend *backend)
    {
        this->is_pre_codegen = true;
    }
    void Node::post_codegen(logia::Backend *backend)
    {
        this->is_post_codegen = true;
    }

    //
    // NoOp
    //
    NoOp::NoOp() : Node(location{nullptr, 0, 0, 0, 0, 0, 0, nullptr})
    {
        this->has_type = false;
        this->skip_codegen = true;
        this->type_inference_pass_id = TYPE_INFERENCE_MAX;
    }
    std::string NoOp::to_string() { return "NoOp"; };
    void NoOp::post_codegen(logia::Backend *backend) {}
    Type *NoOp::get_type() { return nullptr; };
    void NoOp::on_after_attach() {}
    void NoOp::validate() {}
    void NoOp::_on_set_type(TypeDecl *) {};
}
