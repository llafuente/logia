#pragma once

#define LOGIA_LEND
#define LOGIA_CLONE

// Define export/import macros for Windows
#ifdef _WIN32
#define LOGIA_EXPORT __declspec(dllexport)
#define LOGIA_API extern "C" LOGIA_EXPORT
#else
// On Linux/macOS, use GCC visibility attributes
#define LOGIA_EXPORT __attribute__((visibility("default")))
#define LOGIA_API extern "C" LOGIA_EXPORT
#endif