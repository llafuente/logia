#include "logia/log.h"

#include <fstream>
#include <iostream>

namespace logia
{
    std::ofstream logia_log_file;
    size_t logia_log_level = SILLY;
    size_t logia_previous_log_level = SILLY;

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

    void logia_push_log_level(size_t new_level)
    {
        LOG(0, "change log level to {}", new_level);
        logia_previous_log_level = logia_log_level;
        logia_log_level = new_level;
    }

    void logia_pop_log_level()
    {
        LOG(0, "restore log level {}", logia_previous_log_level);
        logia_log_level = logia_previous_log_level;
    }
}