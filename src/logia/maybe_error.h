#pragma once
#include <string>
#include "ast/node.h"
#include "ast/semantic_error.h"

namespace logia
{
    // This namespace is to make an alias with your types and use directly make*
    namespace utils
    {
        /// @brief Represents a possible error that need to be handle properly. success_the error can be accessed in a way that requires handling :)
        /// @tparam success_t
        /// @example
        /// auto x = maybe_error()
        template <typename success_t, typename error_t>
        struct maybe_error
        {
        protected:
            // protected so it needs to be unwrap(ed)!
            success_t success_data;
            error_t error_data;

        public:
            std::string message;

            maybe_error(success_t sdata, std::string message, error_t edata)
            {
                this->success_data = sdata;
                this->message = message;
                this->error_data = edata;
            }

            /// @brief Contains an error ?
            bool is_error()
            {
                return message.size() > 0;
            }

            /// @brief Unwrap value, should be no error
            success_t unwrap_success()
            {
                if (is_error())
                {
                    this->throw_semantic(nullptr);
                }
                return this->success_data;
            }

            error_t unwrap_error()
            {
                if (!is_error())
                {
                    throw std::runtime_error("try to unwrap an error that is a success! check is_error first!");
                }
                return this->error_data;
            }

            /// @brief throw shortcut
            void throw_semantic(AST::Node *node = nullptr)
            {
                throw_semantic_error(node, this->message);
            }
        };

        template <typename success_t, typename error_t>
        maybe_error<success_t, error_t> make_error(std::string message, error_t data)
        {
            return maybe_error<success_t, error_t>({}, message, data);
        }

        template <typename success_t, typename error_t>
        maybe_error<success_t, error_t> make_chained_error(std::string message, maybe_error<success_t, error_t> previous_error)
        {
            message += std::format("\n{}", previous_error.message);
            return maybe_error<success_t, error_t>({}, message, previous_error.unwrap_error());
        }

        template <typename success_t, typename error_t>
        maybe_error<success_t, error_t> make_success(success_t data)
        {
            return maybe_error<success_t, error_t>(data, "", {});
        }
    }
}