#pragma once

#include <vector>
#include <format>
#include <stdexcept>

#include "ast/types.h"
#include "utils.h"
#include "logia/backend.h"

#include "antlr4-runtime.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

struct ::logia::Backend;

namespace logia::AST
{
    struct Type;

    /// @brief Base class for all AST nodes
    struct LOGIA_EXPORT Node
    {
    public:
        /// @brief is attached to a program
        unsigned char is_attached : 1 = false;
        /// @brief codegen pass done
        unsigned char is_codegen : 1 = false;
        /// @brief type inference pass done
        unsigned char is_typed : 1 = false;
        /// @brief internal, check if name is set, used to throw if double set
        unsigned char has_name : 1 = false;
        /// @brief marks node as constant so it can be used as constexpr at comptime
        unsigned char is_constant : 1 = false;

        /// @brief antlr rule, used for error reporting and debugging
        antlr4::ParserRuleContext *rule = nullptr;
        /**
         * Node type to be able to cast from node back to the real type
         */
        // TODO remove this, not needed as we can dynamic_cast
        ast_types type = (ast_types)0;

        /// @brief backpointer to parent node to traverse to root(program)
        Node *parent_node = nullptr;

        /// @brief Avoids children modification (from API / direct access, you know :)
        bool freezed = false;

        /// @brief My beautiful children, and some not so beautiful.
        std::vector<Node *> children = {};

        Node(antlr4::ParserRuleContext *rule, ast_types type);
        ~Node();

        // TODO
        // std::string getText() { return this->rule->getText(); }

        /// @brief Adds child at the end
        void push_child(Node *child);

        /// @brief Adds child at the beginning
        void unshift_child(Node *child);

        void _has_to_notify_attached(Node *child);

        void __notify_attached();

        /// @brief returns a string representation of the tree starting from this node, with padding for each level
        /// @param padding
        /// @return
        std::string to_string_tree(std::string padding = "");

        /// @brief returns essential information nto debug
        virtual std::string to_string() = 0;

        /// @brief generates LLVM IR for this node
        virtual llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) = 0;

        /// @brief retrieves/calculate the type of this node
        /// @remarks this may be available only after type inference pass
        /// @return
        virtual Type *get_type() = 0;

        /// @brief called after the node is attached to a program
        virtual void post_attach();

        /// @brief start type inference pass
        void type_inference();

        /// @brief notify node type inferece pass start
        /// @return
        virtual bool pre_type_inference();

        /// @brief notify node type inferece pass end, all children type should be already inferred
        virtual void post_type_inference();

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

            throw std::runtime_error(std::format("not found {} above {}", typeid(T).name(), this->to_string()));
        }

        /// @brief reverse the tree and returns the first node that match given type or throws
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        T *first_parent()
        {
            Node *ptr = this;
            do
            {
                if (auto out = dynamic_cast<T *>(ptr))
                {
                    return out;
                }
                ptr = ptr->parent_node;
            } while (ptr != nullptr);

            throw std::runtime_error(std::format("not found {} above {}", typeid(T).name(), this->to_string()));
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
            throw std::runtime_error(std::format("unexpected type {} expected {}", typeid(node).name(), typeid(T).name()));
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
                throw std::runtime_error(message);
            }
            throw std::runtime_error(std::format("unexpected type {} expected {}", typeid(this).name(), typeid(T).name()));
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
    };

    /// @brief A node that does nothing
    // used for default values and intrinsics body
    struct NoOp : public Node
    {
        NoOp();
        std::string to_string() override;
        llvm::Value *codegen(logia::Backend *codegen, llvm::IRBuilder<> *builder) override;
        Type *get_type() override;
    };

// TODO remove and reimplement with templates
/// @brief Check node contains at least one bit of ty
/// @param node
/// @param ty
#define NODE_TYPE_ASSERT(node, ty)                                                                         \
    do                                                                                                     \
    {                                                                                                      \
        LOGIA_ASSERT(((node->type & ((ast_types)(ty))) != 0) && __FUNCTION__ && "Invalid node type sent"); \
    } while (false)

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
        throw std::runtime_error(std::format("{}\nExpected type: {}\n{}", message, typeid(T).name(), node->to_string()));
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
        throw std::runtime_error(std::format("{}\nExpected type: {} or {}\n{}", message, typeid(T).name(), typeid(T2).name(), node->to_string()));
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
        throw std::runtime_error(std::format("{}\nExpected type: {} or {} or {}\n{}", message, typeid(T).name(), typeid(T2).name(), typeid(T3).name(), node->to_string()));
    }
}
