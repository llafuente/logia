#include "ast/node.h"
#include "ast/type.h"
#include "ast/import.h"

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
        return std::format("| {} [@{}] ty={}", static_cast<void *>(this), static_cast<void *>(this->parent_node), !has_type ? "no" : (skip_codegen ? "skip" : (is_typed && is_attached ? this->get_final_type()->get_repr() : "??")));
    }
    std::string Node::get_debug_location(uint32_t prev_lines, uint32_t post_lines)
    {
        if (!is_attached)
        {
            throw_compiler_error("Cannot generate debug information of detached nodes");
        }

        auto program = this->first_parent<Program>();

        auto err_line = this->rule->start->getLine();
        auto err_start_column = this->rule->start->getCharPositionInLine();
        auto err_stop_column = std::max<int>(err_start_column + 1, this->rule->stop->getCharPositionInLine());

        auto start_line = std::max<size_t>(0, err_line - prev_lines);
        auto end_line = err_line + post_lines;

        if (prev_lines == 0 && post_lines == 0)
        {
            return std::format("at {}:{}:{}", program->entry_point_file, err_line + 1, err_start_column);
        }

        std::string snippet;
        const char *text = program->file_contents;
        size_t src = 0;
        size_t line = 1;
        char c;
        while ((c = text[src++]) != '\0')
        {
            if (line >= start_line && line <= end_line)
            {
                snippet += c;
            }

            if (c == '\n')
            {
                if (line == err_line)
                {
                    for (size_t j = 0; j < err_start_column; ++j)
                    {
                        snippet += ' ';
                    }
                    for (size_t j = err_start_column; j < err_stop_column; ++j)
                    {
                        snippet += '^';
                    }
                    snippet += '\n';
                }
                ++line;
            }
        }

        return std::format("at {}:{}:{}\n----\n{}\n----\n", program->entry_point_file, err_line + 1, err_start_column, snippet);
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
            flags += "isty";
        }
        if (this->skip_codegen)
        {
            flags += (flags.length() ? "," : "");
            flags += "skipcg";
        }

        if (padding.length() == 0 || last_child)
        {
            // root
            out += std::format("{}{} [{}]\n", padding, this->to_string(), flags);
        }
        else
        {
            // children
            out += std::format("{}{} [{}]\n", padding, this->to_string(), flags);
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
            throw_compiler_error("Type already set");
        }
    }
    /*
        void Node::_set_type(Type *ty)
        {
            LERROR() << this->to_string() << std::endl;
            throw std::runtime_error("set_type not supported for this type node");
        };
    */
    void Node::post_attach()
    {
        this->is_attached = true;
    }

    void Node::pre_type_inference()
    {
        DEBUG() << this->to_string() << std::endl;

        if (!this->skip_type_inference || this->is_pre_type_inference)
        {
            this->_pre_type_inference();
        }
    }
    void Node::_pre_type_inference()
    {
        this->is_pre_type_inference = true;

#if _DEBUG
        // my children should have is_pre_type_inference!
        for (size_t i = 1; i < this->children.size(); ++i)
        {
            if (this->children[i]->skip_type_inference)
                continue;

            if (!this->children[i]->is_pre_type_inference)
            {
                std::cerr << this->to_string_tree() << std::endl;
                throw_compiler_error(std::format("Invalid children[{}] state, should have is_pre_type_inference", i));
            }
        }
#endif
    }

    void Node::post_type_inference()
    {
        DEBUG() << this->to_string() << std::endl;

        if (!this->skip_type_inference || this->is_post_type_inference)
        {
            this->_post_type_inference();
        }
    }

    void Node::_post_type_inference()
    {
        this->is_post_type_inference = true;
#if _DEBUG
        // all arguments should have is_post_type_inference!
        for (size_t i = 1; i < this->children.size(); ++i)
        {
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
        if (this->cg_value)
        {
            return this->cg_value;
        }

        return (this->cg_value = this->post_codegen(backend));
    }

    //
    // NoOp
    //
    NoOp::NoOp() : Node(nullptr) { this->has_type = false; }
    std::string NoOp::to_string() { return "NoOp"; };
    llvm::Value *NoOp::post_codegen(logia::Backend *backend) { return nullptr; }
    Type *NoOp::get_type() { return nullptr; };
    void NoOp::_set_type(Type *) {};
}