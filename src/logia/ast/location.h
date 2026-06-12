#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace logia::AST
{
    struct Node;
    /// @brief source code location info
    struct location
    {
        const char *file = nullptr;
        uint64_t start_line = 0;
        uint64_t start_column = 0;
        uint64_t stop_line = 0;
        uint64_t stop_column = 0;
        const char *text = nullptr;

        /// @brief Retrieves debug information to dump to user so it can locate the (failing) code
        std::string get_debug_location(uint32_t prev_lines = 3, uint32_t post_lines = 3);
    };

    std::vector<std::string> nodelist_get_debug(std::vector<Node *> list);
}