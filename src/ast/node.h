#pragma once

#include <vector>
#include <format>
#include <stdexcept>

#include "utils.h"
#include "logia/backend.h"
#include "logia/compiler_error.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

struct ::logia::Backend;

namespace logia::AST
{
    struct Type;
    struct Block;

    /// @brief Base class for all AST nodes
    struct LOGIA_EXPORT Node
    {
    public:
        /// @brief is attached to a program
        unsigned char is_attached : 1 = false;

        /// @brief codegen pass done
        unsigned char is_codegen : 1 = false;

        /// @brief codegen pass done
        unsigned char skip_codegen : 1 = false;

        /// @brief type inference pass done
        unsigned char is_typed : 1 = false;

        /// @brief internal, check if name is set, used to throw if double set
        unsigned char has_name : 1 = false;

        /// @brief marks node as constant so it can be used as constexpr at comptime
        unsigned char is_constant : 1 = false;

        /// @brief marks node as pre_codegen done, used to avoid multiple calls
        unsigned char is_pre_codegen : 1 = false;

        /// @brief marks node as post_codegen done, used to avoid multiple calls
        unsigned char is_post_codegen : 1 = false;

        /// @brief antlr rule, used for error reporting and debugging
        antlr4::ParserRuleContext *rule = nullptr;

        /// @brief backpointer to parent node to traverse to root(program)
        Node *parent_node = nullptr;

        /// @brief Avoids children modification (from API / direct access, you know :)
        bool freezed = false;

        /// @brief My beautiful children, and some not so beautiful.
        std::vector<Node *> children = {};

        /// @brief codegen result
        llvm::Value *cg_value = nullptr;

        Node(antlr4::ParserRuleContext *rule);
        ~Node();

        // TODO
        // std::string getText() { return this->rule->getText(); }

        /// @brief Adds child at the end
        void push_child(Node *child);

        /// @brief Adds child at the beginning
        void unshift_child(Node *child);

        /// @brief Replace current node for the given one
        void replace_self(Node *new_node);

        void _has_to_notify_attached(Node *child);

        void __notify_attached();

        /// @brief returns a string representation of the tree starting from this node, with padding for each level
        /// @param padding
        /// @return
        std::string to_string_tree(std::string padding = "", bool last_child = false);

        /// @brief returns essential information nto debug
        virtual std::string to_string();

        /// @brief traverse the tree and if is_codegen is false, pre_codegen and post_codegen!
        llvm::Value *codegen(logia::Backend *backend);

        /// @brief prepare node/children to generate LLVM IR
        virtual void pre_codegen(logia::Backend *backend);

        /// @brief generates LLVM IR for this node
        virtual llvm::Value *post_codegen(logia::Backend *backend);

        /// @brief retrieves/calculate the type of this node
        /// @remarks this may be available only after type inference pass
        /// @return
        virtual Type *get_type() = 0;

        /// @brief override node type, it's only allowed in a few node. by default throws atm!
        /// @remarks this may be available only after type inference pass
        /// @return
        virtual void set_type(Type *t);

        /// @brief called after the node is attached to a program
        virtual void post_attach();

        /// @brief start type inference pass
        void type_inference();

        /// @brief notify node type inferece pass start
        /// @return
        virtual bool pre_type_inference();

        /// @brief notify node type inferece pass end, all children type should be already inferred
        virtual void post_type_inference();

        virtual Type *get_final_type();

        /// @brief resolve node references
        /// @return
        virtual Node *resolve();

        /// @brief loops all children with given type (cdel)
        /// @tparam T
        /// @param cb
        template <typename T>
        void foreach_child(void (*cb)(const T *))
        {
            for (const auto &ptr : this->children)
            {
                if (auto out = dynamic_cast<T *>(ptr))
                {
                    cb(out);
                }
            }
        }

        /// @brief loops all children with given type (lambda capture)
        /// @tparam T
        /// @param cb
        template <typename T>
        void foreach_child(std::function<void(const T *)> cb)
        {
            for (const auto &ptr : this->children)
            {
                if (auto out = dynamic_cast<T *>(ptr))
                {
                    cb(out);
                }
            }
        }

        /// @brief traverse tree while lambda returns true
        /// @tparam T
        /// @param cb
        void foreach_descendant(std::function<bool(Node *node, int deep)> cb, int deep = 0)
        {
            for (const auto &ptr : this->children)
            {
                if (cb(ptr, deep))
                {
                    ptr->foreach_descendant(cb, deep + 1);
                }
            }
        }

        /// @brief Retrieves the first child that match given type or throws
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        T *first_child()
        {
            for (const auto &ptr : this->children)
            {
                if (auto out = dynamic_cast<T *>(ptr))
                {
                    return out;
                }
            }

            throw_compiler_error(std::format("not found {} above {}", typeid(T).name(), this->to_string()));
        }

        /// @brief reverse the tree and returns the first node that match given type or throws
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        T *first_parent()
        {
            Node *ptr = this->parent_node;
            // DEBUG() << ptr->to_string() << std::endl;
            do
            {
                if (auto out = dynamic_cast<T *>(ptr))
                {
                    return out;
                }
                ptr = ptr->parent_node;
                // DEBUG() << (ptr != nullptr ? ptr->to_string() : "nullptr") << std::endl;
            } while (ptr != nullptr);

            throw_compiler_error(std::format("not found {} above {}", typeid(T).name(), this->to_string()));
        }

        /// @brief reverse the tree and returns true if found a match, false otherwise
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        bool try_first_parent(T **out)
        {
            Node *ptr = this->parent_node;
            while (ptr != nullptr)
            {
                // DEBUG() << ptr->to_string() << std::endl;

                if (auto maybe = dynamic_cast<T *>(ptr))
                {
                    *out = maybe;
                    return true;
                }
                ptr = ptr->parent_node;
                // DEBUG() << (ptr != nullptr ? ptr->to_string() : "nullptr") << std::endl;
            }

            return false;
        }

        /// @brief Retrieve children at given position as given type. If fail throws.
        /// @tparam T
        /// @param index
        /// @return
        template <class T>
        T *get_child(uint32_t index)
        {
            auto node = this->children[index];
            if (auto out = dynamic_cast<T *>(node))
            {
                return out;
            }
            throw_compiler_error(std::format("unexpected type {} expected {}", typeid(node).name(), typeid(T).name()));
        }

        /// @brief Retrieve children at given position as given type. If fail throws.
        /// @tparam T
        /// @param index
        /// @return
        template <class T>
        bool is_child(uint32_t index)
        {
            auto node = this->children[index];
            if (auto out = dynamic_cast<T *>(node))
            {
                return true;
            }
            return false;
        }
        /// @brief Casts the node to the given type or throws if the cast fails
        /// @tparam T
        /// @param message
        /// @return
        template <class T>
        T *as(const char *message = nullptr)
        {
            if (auto out = dynamic_cast<T *>(this))
            {
                return out;
            }
            if (message)
            {
                throw_compiler_error(message);
            }
            throw_compiler_error(std::format("unexpected type {} expected {}`n{}", typeid(this).name(), typeid(T).name(), this->to_string()));
        }

        /// @brief Checks if the node is of the given type
        /// @tparam T
        /// @return
        template <class T>
        bool is()
        {
            if (auto out = dynamic_cast<T *>(this))
            {
                return true;
            }
            return false;
        }

        /// @brief reverse the tree and returns true if found a match, false otherwise
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        bool try_cast(T **out)
        {
            if (auto tmp = dynamic_cast<T *>(this))
            {
                *out = tmp;
                return true;
            }
            return false;
        }
    };

    /// @brief A node that does nothing
    // used for default values and intrinsics body
    struct NoOp : public Node
    {
        NoOp();
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
    };

    /// @brief Throws if node is not of given type
    /// @tparam T
    /// @param node
    /// @param message
    template <class T>
    void node_assert(Node *node, std::string message)
    {
        if (dynamic_cast<T *>(node))
        {
            return;
        }
        throw_compiler_error(std::format("{}\nExpected type: {}\n{}", message, typeid(T).name(), node->to_string()));
    }

    /// @brief Throws if node is not of given type
    /// @tparam T
    /// @tparam T2
    /// @param node
    /// @param message
    template <class T, class T2>
    void node_assert(Node *node, std::string message)
    {
        if (dynamic_cast<T *>(node) || dynamic_cast<T2 *>(node))
        {
            return;
        }
        throw_compiler_error(std::format("{}\nExpected type: {} or {}\n{}", message, typeid(T).name(), typeid(T2).name(), node->to_string()));
    }

    /// @brief Throws if node is not of given type
    /// @tparam T
    /// @tparam T2
    /// @param node
    /// @param message
    template <class T, class T2, class T3>
    void node_assert(Node *node, std::string message)
    {
        if (dynamic_cast<T *>(node) || dynamic_cast<T2 *>(node) || dynamic_cast<T3 *>(node))
        {
            return;
        }
        throw_compiler_error(std::format("{}\nExpected type: {} or {} or {}\n{}", message, typeid(T).name(), typeid(T2).name(), typeid(T3).name(), node->to_string()));
    }
}
