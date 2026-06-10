#pragma once

#define PROFILING_START() \
    std::chrono::high_resolution_clock::time_point __previous_time = std::chrono::high_resolution_clock::now();

#define PROFILING_END()                                                                                        \
    std::chrono::high_resolution_clock::time_point __current_time = std::chrono::high_resolution_clock::now(); \
    long long new_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - previous_time).count()