#include "ast/node.h"
#include "ast/traverse.h"

#include <format>

namespace logia::AST
{
    ParserRuleContextException::ParserRuleContextException(antlr4::ParserRuleContext *rule, const std::string &message)
        : std::runtime_error(format_message(rule, message))
    {
    }

    std::string ParserRuleContextException::format_message(antlr4::ParserRuleContext *rule, const std::string &message)
    {
        if (!rule)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        auto start = rule->getStart();
        if (!start)
        {
            return std::format("{} [at <unknown>:0:0]", message);
        }

        std::string source = "<unknown>";
        if (auto tokenSource = start->getTokenSource())
        {
            if (auto input = tokenSource->getInputStream())
            {
                source = input->getSourceName();
            }
        }

        return std::format("{} [at {}:{}:{}]", message, source, start->getLine(), start->getCharPositionInLine());
    }

    [[noreturn]] void throw_parser_context_exception(antlr4::ParserRuleContext *rule, const std::string &message)
    {
        throw ParserRuleContextException(rule, message);
    }

    [[noreturn]] void throw_node_exception(const Node *node, const std::string &message)
    {
        throw_parser_context_exception(node ? node->rule : nullptr, message);
    }

    Node::Node(antlr4::ParserRuleContext *rule, ast_types type)
    {
        this->rule = rule;
        this->type = type;
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

        this->post_attach();
    }

    std::string Node::to_string_tree(std::string padding)
    {
        std::string out = std::format("{} {} (parent {:p})\n", padding, this->to_string(), static_cast<void *>(this->parent_node));

        padding += "  ";

        for (int i = 0; i < this->children.size(); i++)
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
        for (int i = 0; i < this->children.size(); i++)
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
    NoOp::NoOp() : Node(nullptr, ast_types::EXPRESSION) {}
    std::string NoOp::to_string() { return "NoOp"; };
    llvm::Value *NoOp::codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) { return nullptr; }
    Type *NoOp::get_type() { return nullptr; };
}