#pragma once
#include <string>
#include "ast/node.h"
#include "ast/semantic_error.h"

namespace logia
{

    template <typename T>
    struct maybe_error
    {
    protected:
        // protected so it needs to be unwrap(ed)!
        T data;

    public:
        std::string message;
        /// @brief Not an error, but also don't need to return
        maybe_error();
        /// @brief Not an error and need to return something!
        /// @param data
        maybe_error(T data);
        /// @brief An error with message and extra information
        /// @param msg
        /// @param d
        maybe_error(std::string msg, T d);

        /// @brief More specific errror
        /// @param msg
        /// @param d
        maybe_error(maybe_error<T> previous_error, std::string msg);

        /// @brief Contains an error ?
        bool is_error();

        /// @brief Unwrap value, should be no error
        T unwrap();

        /// @brief throw shortcut
        void throw_semantic(AST::Node *node);
    };

    template <typename T>
    maybe_error<T>::maybe_error()
    {
        this->message = "";
    }
    template <typename T>
    maybe_error<T>::maybe_error(T data)
    {
        message = "";
        this->data = data;
    }

    template <typename T>
    maybe_error<T>::maybe_error(std::string msg, T d)
    {
        this->message = msg;
        data = d;
    }

    template <typename T>
    maybe_error<T>::maybe_error(maybe_error<T> previous_error, std::string msg)
    {
        this->message = std::format("{}\n{}", msg, previous_error.message);
        data = previous_error.data;
    }

    template <typename T>
    bool maybe_error<T>::is_error()
    {
        return message.size() > 0;
    }

    template <typename T>
    T maybe_error<T>::unwrap()
    {
        if (is_error())
        {
            this->throw_semantic(nullptr);
        }
        return this->data;
    }

    template <typename T>
    void maybe_error<T>::throw_semantic(AST::Node *node)
    {
        throw_semantic_error(node, this->message);
    }

}