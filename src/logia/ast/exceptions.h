#pragma once

#include <stdexcept>
#include <string>

class terminal_error : std::runtime_error
{
    terminal_error(const std::string &_Message) : std::runtime_error(_Message) {}
};