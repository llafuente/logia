#include "ast/node.h"
#include "ast/traverse.h"

#include <format>

namespace logia::AST
{

    Node::Node(antlr4::ParserRuleContext *rule)
    {
        this->rule = rule;
    }
    Node::~Node()
    {
        // TODO
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
        for (int i = 0; i < this->children.size(); ++i)
        {
            this->children[i]->__notify_attached();
        }

        this->post_attach();
    }

    std::string Node::to_string_tree(std::string padding)
    {
        std::string out = std::format("{} {} (parent {:p})\n", padding, this->to_string(), static_cast<void *>(this->parent_node));

        padding += "  ";

        for (size_t i = 0; i < this->children.size(); i++)
        {
            out += this->children[i]->to_string_tree(padding);
        }

        return out;
    }

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
    Node *Node::resolve()
    {
        return nullptr;
    }

    //
    // NoOp
    //
    NoOp::NoOp() : Node(nullptr) {}
    std::string NoOp::to_string() { return "NoOp"; };
    llvm::Value *NoOp::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) { return nullptr; }
    Type *NoOp::get_type() { return nullptr; };
}