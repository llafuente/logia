#include "logia/ast/node.h"

#include "logia/log.h"
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
            node->is_attached = true;
            node->post_attach();
        }
        else
        {
            LOG(WRN, "invalid post_attach, node already attached ? {}", node->to_string());
        }
        node->foreach_descendant([](auto descendant, auto i) -> bool
                                 {
                                    if(!descendant->is_attached) {
                                    descendant->is_attached = true;
            descendant->post_attach();
                                    } else {
                                        LOG(WRN, "invalid post_attach, node already attached ? {}", descendant->to_string());
                                    }
            return true; });
    }

    // impl

    Node::Node(location loc) : loc(loc)
    {
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
        if (this->is_pre_type_inference)
        {
            flags += (flags.length() ? "," : "");
            flags += "prety";
        }
        if (this->is_post_type_inference)
        {
            flags += (flags.length() ? "," : "");
            flags += "postty";
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
        else
        {
            ty = is_typed && is_attached ? this->get_final_type()->get_repr() : "??";
        }

        return std::format("| {} [@{}] ty={} [{}]", static_cast<void *>(this), static_cast<void *>(this->parent_node), ty, flags);
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
    void Node::set_type(Type *ty)
    {
        if (!this->has_type)
        {
            // REVIEW can we do anything ?!
        }

        auto my_ty = this->get_type();
        if (my_ty == nullptr)
        {
            this->_set_type(ty);
            this->is_typed = true;
        }
        else if (my_ty != ty)
        {
            // TODO we should check this ? we may not
            // throw_compiler_error("Type already set");
            LOG(WRN, "Type already set");
            this->_set_type(ty);
            this->is_typed = true;
        }
        if (my_ty == ty)
        {
            this->is_typed = true;
        }
    }
    /*
        void Node::_set_type(Type *ty)
        {
            LERROR() << this->to_string() << std::endl;
            throw std::runtime_error("set_type not supported for this type node");
        };
    */

    void Node::pre_type_inference()
    {
        LOG(DBG, "{}", this->to_string());

        if (!this->skip_type_inference || this->is_pre_type_inference)
        {
            this->_pre_type_inference();
        }
    }
    void Node::_pre_type_inference()
    {
        this->is_pre_type_inference = true;
        /* remove this test as we are going to retry on type_inference!
        #if _DEBUG
                // everyone should have is_pre_type_inference!
                for (size_t i = 0; i < this->children.size(); ++i)
                {
                    if (!this->children[i]->has_type)
                        continue;

                    if (this->children[i]->skip_type_inference)
                        continue;

                    if (!this->children[i]->is_pre_type_inference)
                    {
                        std::cerr << this->to_string_tree() << std::endl
                                  << "-------------------" << std::endl
                                  << this->children[i]->to_string() << std::endl;

                        throw_compiler_error(std::format("Invalid children[{}] state, should have is_pre_type_inference", i));
                    }
                }
        #endif
        */
    }

    void Node::post_type_inference()
    {
        LOG(DBG, "{}", this->to_string());

        if (!this->skip_type_inference || this->is_post_type_inference)
        {
            this->_post_type_inference();
        }
    }

    void Node::_post_type_inference()
    {
        this->is_post_type_inference = true;
#if _DEBUG
        // everyone should have is_post_type_inference!
        for (size_t i = 0; i < this->children.size(); ++i)
        {
            if (!this->children[i]->has_type)
                continue;

            if (this->children[i]->skip_type_inference)
                continue;

            if (!this->children[i]->is_post_type_inference)
            {
                std::cerr << this->to_string_tree() << std::endl;
                throw_compiler_error(std::format("Invalid children[{}] state, should have is_post_type_inference", i));
            }
        }
#endif
    }

    Type *Node::get_final_type()
    {
        // this resolve types in the following manner
        // TypeDef -> get referenced type
        // Struct
        // * has one field with "λ" as name -> return the "λ" type
        // * return the struct
        // AnyOther -> return

        auto type = this->get_type();
        int MAX = 10;
        for (; MAX > 0; --MAX)
        {
            if (type->is<TypeDef>())
            {
                type = type->get_type();
            }
            else if (type->is<Struct>())
            {
                auto s = type->as<Struct>();
                if (s->field_count == 1)
                {
                    auto field = s->get_field("λ");
                    if (field != nullptr)
                    {
                        return field->get_final_type();
                    }
                }
                return type;
            }
            else
            {
                return type;
            }
        }

        throw std::runtime_error(std::format("exceeded MAX iteration"));
    }

    Node *Node::resolve()
    {
        return nullptr;
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
    llvm::Value *Node::post_codegen(logia::Backend *backend)
    {
        this->is_post_codegen = true;
        return this->cg_value;
    }

    llvm::Value *Node::codegen(logia::Backend *backend)
    {
        // exist fast, if we have the value we seek, just return!
        if (this->cg_value)
        {
            LOG(DBG, "{} before pre_codegen return cg_value is ready!", this->to_string());
            return this->cg_value;
        }
        // pre_codegen, as optimization is pre_codegen can generate the code we need we just return!
        this->pre_codegen(backend);
        if (this->cg_value)
        {
            LOG(DBG, "{} after pre_codegen return cg_value is ready!", this->to_string());
            // TODO set is_post_codegen ? seems like proper way to handle it not in each children
            return this->cg_value;
        }

        return (this->cg_value = this->post_codegen(backend));
    }

    //
    // NoOp
    //
    NoOp::NoOp() : Node({})
    {
        this->has_type = false;
        this->skip_codegen = true;
        this->skip_type_inference = true;
    }
    std::string NoOp::to_string() { return "NoOp"; };
    llvm::Value *NoOp::post_codegen(logia::Backend *backend) { return nullptr; }
    Type *NoOp::get_type() { return nullptr; };
    void NoOp::post_attach() {}
    void NoOp::validate() {}
    void NoOp::_set_type(Type *) {};
}