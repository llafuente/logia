#include "ast/node.h"
#include "ast/traverse.h"

#include <format>

namespace logia::AST
{

    Node::Node(antlr4::ParserRuleContext *rule, ast_types type)
    {
        this->rule = rule;
        this->type = type;
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
        auto program = ast_find_closest_parent(this, ast_types::PROGRAM);
        if (!program)
        {
            return;
        }

        child->__notify_attached();
    }

    void Node::__notify_attached()
    {
        for (int i = 0; i < this->children.size(); ++i)
        {
            this->children[i]->__notify_attached();
        }
        /*
        // types are allowed to have parent for the rest -> NO!
        if (this->parent_node != nullptr)
        {
            if ((this->type & ast_types::TYPE)
                != 0)
            {
                return;
            }
                std::cout << "empty parent node not allowed! " << this->to_string() << std::endl;
                LOGIA_ASSERT(false);
        }
        */

        this->on_after_attach();
    }

    std::string Node::to_string_tree(std::string padding)
    {
        // std::string out = std::vformat(std::string("{}({:p}) {}\r\n"), std::make_format_args(padding, static_cast<void*>(this), this->to_string()));
        std::string out = std::format("{} {} ({:p})\n", padding, this->to_string(), static_cast<void *>(this));

        padding += "  ";

        for (int i = 0; i < this->children.size(); i++)
        {
            out += this->children[i]->to_string_tree(padding);
        }

        return out;
    }

    void Node::on_after_attach()
    {
        this->is_attached = true;
    }
}