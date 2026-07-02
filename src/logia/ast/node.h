#pragma once

#include "logia/logia.h"
#include "logia/ast/location.h"

#include <vector>
#include <string>
#include <functional>
#include <format>
#include "logia/compiler_error.h"

namespace llvm
{
    // #include "llvm/IR/Value.h"
    class Value;
}
namespace logia
{
    struct Backend;
}

namespace logia::AST
{
    struct Type;
    struct Block;
    struct Prorgam;

    /// @brief Base class for all AST nodes
    struct LOGIA_EXPORT Node
    {
    public:
        /// @brief is attached to a program
        unsigned char is_attached : 1 = false;

        /// @brief is current node validated
        unsigned char is_validated : 1 = false;

        /// @brief codegen pass done
        unsigned char skip_codegen : 1 = false;

        /// @brief marks node as pre_codegen done, used to avoid multiple calls
        unsigned char is_pre_codegen : 1 = false;

        /// @brief marks node as post_codegen done, used to avoid multiple calls
        unsigned char is_post_codegen : 1 = false;

        /// @brief current node require to be typed
        unsigned char has_type : 1 = true;

        /// @brief type inference pass done
        unsigned char is_typed : 1 = false;

        /// @brief marks node as constant so it can be used as constexpr at comptime
        unsigned char is_constant : 1 = false;

        /// @brief location in code, used for error reporting and debugging
        location loc = {nullptr, 0, 0, 0, 0, 0, 0, nullptr, ""};

        /// @brief backpointer to parent node to traverse to root(program)
        Node *parent_node = nullptr;

        /// @brief Avoids children modification (from API / direct access, you know :)
        bool freezed = false;

        Type *real_type = nullptr;

        /// @brief Last/Current type inference pass id
        size_t type_inference_pass_id = 0;

        /// @brief My beautiful children, and some not so beautiful.
        std::vector<Node *> children = {};

        /// @brief codegen result
        llvm::Value *cg_value = nullptr;

        Node(location loc);
        ~Node();

        // TODO
        // std::string getText() { return this->loc->getText(); }

        /// @brief Adds child at the end of the list
        void push_child(Node *child);

        /// @brief Adds child at the end of the list
        void set_child(Node *child, size_t position);

        /// @brief Adds child at the beginning of the list
        void unshift_child(Node *child);

        /// @brief Replace current node for the given one
        /// @remarks Node should be attached
        void replace_self(Node *new_node);

        /// @brief Replace current node for the given one
        /// @remarks Node should be attached
        void replace(Node *attached_node, Node *new_node);

        /// @brief returns a string representation of the tree starting from this node, with padding for each level
        /// @param padding
        /// @return
        std::string to_string_tree(std::string padding = "", bool last_child = false);

        /// @brief returns essential information nto debug
        virtual std::string to_string();

        /// @brief prepare node/children to generate LLVM IR
        virtual void pre_codegen(logia::Backend *backend);

        /// @brief generates LLVM IR for this node
        virtual llvm::Value *post_codegen(logia::Backend *backend);

        /// @brief retrieves/calculate the type of this node
        /// @remarks this may be available only after type inference pass
        /// @return
        virtual Type *get_type() { return this->real_type; }

        /// @brief override node type, it's only allowed in a few node. by default throws atm!
        /// @remarks this may be available only after type inference pass
        /// @return
        void set_type(Type *ty);

        /// @brief called after the node is attached to a program
        virtual void on_after_attach() = 0;

        /// @brief validate current node, mostly semantic check
        virtual void validate() = 0;

        /// @brief Calls _pre_type_inference if this node required to be typed
        /// @return
        void type_inference(size_t pass_id);

        virtual Type *get_final_type();

        std::vector<Node *> get_pre_descendant();

        std::vector<Node *> get_post_descendant();

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

        /// @brief traverse tree inorder while lambda returns true
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

        /// @brief traverse tree inorder while lambda returns true
        /// @tparam T
        /// @param cb
        template <typename T>
        void foreach_descendant(std::function<void(T *node, int deep)> cb, int deep = 0)
        {
            T *node;
            for (const auto &ptr : this->children)
            {

                if (ptr->try_cast<T>(&node))
                {
                    cb(node, deep);
                }
                ptr->foreach_descendant<T>(cb, deep + 1);
            }
        }

        /// @brief traverse tree Depth-First Traversal
        /// @tparam T
        /// @param cb
        void foreach_post_descendant(std::function<void(Node *node, int deep)> cb, int deep = 0)
        {
            for (const auto &ptr : this->children)
            {
                ptr->foreach_post_descendant(cb, deep + 1);
                cb(ptr, deep);
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
                T *out = dynamic_cast<T *>(ptr);
                if (out != nullptr)
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
                T *out = dynamic_cast<T *>(ptr);
                if (out != nullptr)
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
                T *maybe = dynamic_cast<T *>(ptr);
                if (maybe != nullptr)
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
            T *out = dynamic_cast<T *>(node);
            if (out != nullptr)
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
            T *out = dynamic_cast<T *>(node);
            if (out != nullptr)
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
            T *out = dynamic_cast<T *>(this);
            if (out != nullptr)
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
            T *out = dynamic_cast<T *>(this);
            return out != nullptr;
        }

        /// @brief reverse the tree and returns true if found a match, false otherwise
        /// @tparam T
        /// @param cb
        /// @return
        template <typename T>
        bool try_cast(T **out)
        {
            T *tmp = dynamic_cast<T *>(this);
            if (tmp != nullptr)
            {
                *out = tmp;
                return true;
            }
            return false;
        }

    protected:
        virtual void _early_type_inference();
        virtual void _pre_type_inference();
        virtual void _post_type_inference();
        virtual void _set_type(Type *ty) = 0;
    };

    /// @brief A node that does nothing
    // used for default values and intrinsics body
    struct NoOp : public Node
    {
        NoOp();
        std::string to_string() override;
        llvm::Value *post_codegen(logia::Backend *backend) override;
        Type *get_type() override;
        void on_after_attach() override;
        void validate() override;

    protected:
        void _set_type(Type *) override;
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

    template <class T>
    std::vector<T *> nodelist_cast(std::vector<Node *> input, bool remove_failures)
    {
        std::vector<T *> out;
        T *v;
        for (auto node : input)
        {
            if (node->try_cast<T>(&v))
            {
                out.push_back(v);
            }
            else
            {
                if (!remove_failures)
                {
                    throw_compiler_error(std::format("Unexpected node type: {} / {}", typeid(T).name(), node->to_string()));
                }
            }
        }
        return out;
    }
}
