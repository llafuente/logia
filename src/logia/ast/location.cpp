#include "logia/ast/location.h"

#include "logia/log.h"
#include "logia/ast/node.h"

#include <string>
#include <format>

namespace logia::AST
{
    std::string location::get_debug_location(uint32_t prev_lines, uint32_t post_lines)
    {

        if (this->file == nullptr)
        {
            return std::format("at <unknown>:0:0");
        }
        auto err_start_line = this->start_line;
        auto err_start_column = this->start_column;
        auto err_stop_line = std::max<size_t>(this->start_line, this->stop_line);
        auto err_stop_column = std::max<size_t>(err_start_column + 1, this->stop_column);
        auto snippet_start_line = err_start_line > prev_lines ? std::max<size_t>(0, err_start_line - prev_lines) : 0; // mind not to underflow!
        auto snippet_stop_line = err_start_line + post_lines;                                                         // don't mind to"overflow"

        // LOG(DBG, "start = {}:{} end = {}:{}", err_start_line, err_start_column, err_stop_line, err_stop_column);

        char snippet[1024];
        const char *text = this->text;
        size_t src = 0;
        size_t dst = 0;
        size_t line = 0;
        char c;
        while ((c = text[src++]) != '\0')
        {
            if (line >= snippet_start_line && line <= snippet_stop_line)
            {
                snippet[dst++] = c;
            }

            if (c == '\n')
            {
                if (line == err_start_line)
                {
                    for (size_t j = 0; j < err_start_column; ++j)
                    {
                        snippet[dst++] = ' ';
                    }
                    for (size_t j = err_start_column; j < err_stop_column; ++j)
                    {
                        snippet[dst++] = '^';
                    }
                    snippet[dst++] = '\n';
                }
                ++line;
            }
        }
        snippet[dst++] = '\0';

        return std::format("at {}:{}:{}\n{}", this->file, err_start_line + 1, err_start_column + 1, snippet);
    }

    std::string location::get_source_code()
    {
        if (this->preview.size())
        {
            return this->preview;
        }
        if (this->stop_index == 0 && this->start_index == 0)
        {
            return this->preview;
        }

        // newlines -> "\r\n"
        this->preview.reserve(this->stop_index - this->start_index + 2 * (this->stop_line - this->start_line) + 1);

        for (size_t i = this->start_index; i <= this->stop_index; ++i)
        {
            if (this->text[i] == '\r')
            {
                this->preview += "\\r";
            }
            else if (this->text[i] == '\n')
            {
                this->preview += "\\n";
            }
            else
            {
                this->preview += this->text[i];
            }
        }

        return this->preview;
    }

    std::vector<std::string> nodelist_get_debug(std::vector<Node *> list)
    {
        auto out = std::vector<std::string>(list.size(), "");
        for (auto node : list)
        {
            out.push_back(node->loc.get_debug_location());
        }
        return out;
    }
}