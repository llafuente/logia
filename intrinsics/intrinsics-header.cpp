// & 'C:\Program Files\LLVM\bin\clang++.exe' -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics.ll

#include <stdint.h>
#include <inttypes.h>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
// typedef __int128 i128;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
// typedef unsigned __int128 u128;

#include <float.h>

typedef _Float16 f16;
typedef float f32;
typedef double f64;
typedef long double f128;

// pointers
#include <stddef.h>
#include <cstdio>

extern "C" _iobuf *logia_get_stdout()
{
    return stdout;
}

extern "C" _iobuf *logia_get_stdin()
{
    return stdin;
}
extern "C" _iobuf *logia_get_stderr()
{
    return stderr;
}

extern "C" void logia_print_stdout(char *text)
{
    fputs(text, stdout);
}

// REVIEW what happen when size_t != i64 and we send something bigger!
// naybe we should not allow to compile logia in that target, or assert
extern "C" void logia_fwrite(void *buffer, i64 size_in_bytes, _iobuf *stream)
{
    fwrite(buffer, 1, (size_t)size_in_bytes, stream);
}