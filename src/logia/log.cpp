#include "logia/log.h"

#include <fstream>
#include <iostream>

namespace logia
{
    std::ofstream logia_log_file;
    size_t logia_log_level = SILLY;

    bool logia_init_log(char *file_name, bool append)
    {
        if (append)
        {
            logia_log_file.open(file_name, std::ios::out | std::ios::app);
        }
        else
        {
            logia_log_file.open(file_name, std::ios::out | std::ios::trunc);
        }

        if (!logia_log_file.is_open())
        {
            std::cerr << "Error: Could not open log file: " << file_name << "\n";
            return false;
        }
        return true;
    }

    void logia_deinit_log()
    {
        if (logia_log_file.is_open())
        {
            logia_log_file.close();
        }
    }
}