#pragma once

#include <fstream> // fix: 'std::basic_ostream<_Elem,_Traits> &std::operator <<(std::basic_ostream<_Elem,_Traits> &,const std::error_code &)': could not deduce template argument for 'std::basic_ostream<_Elem,_Traits> &' from 'std::ofstream'

// Step 1: Helper macro to convert a value to a string
#define LOG_STRINGIFY(x) #x

// Step 2: Ensure macro arguments are expanded before stringizing
#define LOG_TOSTRING(x) LOG_STRINGIFY(x)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define LOG_ERR(FORMAT, ...) LOG(1, FORMAT, __VA_ARGS__)
#define LOG_INF(FORMAT, ...) LOG(2, FORMAT, __VA_ARGS__)
#define LOG_WRN(FORMAT, ...) LOG(3, FORMAT, __VA_ARGS__)
#define LOG_DBG(FORMAT, ...) LOG(4, FORMAT, __VA_ARGS__)
#define LOG_VRB(FORMAT, ...) LOG(5, FORMAT, __VA_ARGS__)
#define LOG_SILLY(FORMAT, ...) LOG(6, FORMAT, __VA_ARGS__)

#define INF 2
#define WRN 3
#define DBG 4
#define VRB 5
#define SILLY 6
#define LOG(LEVEL, FORMAT, ...)                                                                                                                                                                \
    do                                                                                                                                                                                         \
    {                                                                                                                                                                                          \
        if (::logia::logia_log_level >= LEVEL)                                                                                                                                                 \
        {                                                                                                                                                                                      \
            ::logia::logia_log_file << LOG_TOSTRING(LEVEL) << "|" << __FILENAME__ << ":" << __LINE__ << "|" << __func__ << " " << std::format(FORMAT __VA_OPT__(, ) __VA_ARGS__) << std::endl; \
        }                                                                                                                                                                                      \
    } while (false)

namespace logia
{
    extern std::ofstream logia_log_file;
    extern size_t logia_log_level;

    /**
     * Initialize log
     */
    bool logia_init_log(char *file_name, bool append = false);
    void logia_deinit_log();
}