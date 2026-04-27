#include "ast/node.h"
#include "ast/traverse.h"

#include <format>

namespace logia::AST
{

    Node::Node(antlr4::ParserRuleContext *rule) : rule(rule)
    {
    }
    Node::~Node()
    {
        // TODO
    }

    std::string Node::to_string()
    {
        return std::format("| {} [@{}]", static_cast<void *>(this), static_cast<void *>(this->parent_node));
    }

    void Node::push_child(Node *child)
    {
        if (freezed)
        {
            throw std::exception("Node is freezed");
        }
        children.push_back(child);
        child->parent_node = this;

        this->_has_to_notify_attached(child);
    }

    void Node::unshift_child(Node *child)
    {
        if (freezed)
        {
            throw std::exception("Node is freezed");
        }
        children.insert(children.begin(), child);
        child->parent_node = this;

        this->_has_to_notify_attached(child);
    }

    void Node::replace_self(Node *new_node)
    {
        auto parent = this->parent_node;
        std::replace(parent->children.begin(), parent->children.end(), this, new_node);
    }

    void Node::_has_to_notify_attached(Node *child)
    {
        // an attached node is the one that can reach program
        Program *program = nullptr;
        if (this->is<Program>() || this->try_first_parent<Program>(&program))
        {
            child->__notify_attached();
        }
    }

    void Node::__notify_attached()
    {
        for (auto i = 0; i < this->children.size(); ++i)
        {
            this->children[i]->__notify_attached();
        }

        this->post_attach();
    }

    std::string Node::to_string_tree(std::string padding, bool last_child)
    {
        std::string out;
        if (padding.length() == 0 || last_child)
        {
            // root
            out += std::format("{}{} [{},{}]\n", padding, this->to_string(), this->is_pre_codegen ? "precg" : "", this->is_post_codegen ? "postcg" : "");
        }
        else
        {
            // children
            out += std::format("{}{} [{},{}]\n", padding, this->to_string(), this->is_pre_codegen ? "precg" : "", this->is_post_codegen ? "postcg" : "");
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
    void Node::set_type(Type *t)
    {
        LERROR() << this->to_string() << std::endl;
        throw std::runtime_error("set_type not supported for this type node");
    };

    void Node::post_attach()
    {
        this->is_attached = true;
    }

    void Node::type_inference()
    {
        // TODO how to handle inference can't detect the type -> pre_type_inference return false ??
        this->pre_type_inference();
        for (size_t i = 0; i < this->children.size(); i++)
        {
            this->children[i]->type_inference();
        }
        this->post_type_inference();
    }

    bool Node::pre_type_inference()
    {
        DEBUG() << this->to_string() << std::endl;
        return true;
    }

    void Node::post_type_inference()
    {
        DEBUG() << this->to_string() << std::endl;
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
        this->pre_codegen(backend);
        /*
        this->foreach_descendant([backend](auto node, auto deep)
                                 {
                                     if (node->is_codegen || node->skip_codegen)
                                     {
                                         DEBUG() << "SKIP codegen - " << node->to_string() << std::endl;
                                         return false; // stop
                                     }
                                     node->pre_codegen(backend);
                                     return true; // continue
                                 });
        */
        return (this->cg_value = this->post_codegen(backend));

        /*
        if (this->is_codegen || this->skip_codegen)
        {
            return this->cg_value;
        }

        this->pre_codegen(backend);
        for (size_t i = 0; i < this->children.size(); i++)
        {
            this->children[i]->codegen(backend);
        }
        // some node need to be created in a specific order, do not "double" post
        if (this->is_codegen) {
            return this->cg_value;
        }
        this->cg_value = this->post_codegen(backend);
        this->is_codegen = true;
        return this->cg_value;
        */
    }

    //
    // NoOp
    //
    NoOp::NoOp() : Node(nullptr) {}
    std::string NoOp::to_string() { return "NoOp"; };
    llvm::Value *NoOp::post_codegen(logia::Backend *backend) { return nullptr; }
    Type *NoOp::get_type() { return nullptr; };
}