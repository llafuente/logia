#include "test_utils.h"

#if _WIN32

#include "logia/run.h"
#include "logia/ast/semantic_error.h"

#include "gtest/gtest.h"

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>

static HANDLE g_readPipe = NULL;
static HANDLE g_writePipe = NULL;
static HANDLE g_oldStdout = NULL;
static int g_oldCrtStdout = -1;

void winapi_pint_err(const char *context)
{
    DWORD err = GetLastError();

    if (err == 0)
    {
        printf("%s: No error\n", context);
        return;
    }

    LPSTR msgBuf = NULL;

    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuf,
        0,
        NULL);

    if (size && msgBuf)
    {
        fprintf(stderr, "%s failed:\n", context);
        fprintf(stderr, "  Error code: %lu\n", err);
        fprintf(stderr, "  Message   : %s\n", msgBuf);
        LocalFree(msgBuf);
    }
    else
    {
        fprintf(stderr, "%s failed:\n", context);
        fprintf(stderr, "  Error code: %lu\n", err);
        fprintf(stderr, "  (FormatMessage failed)\n");
    }
}

bool start_stdout_capture()
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    // Create pipe
    if (!CreatePipe(&g_readPipe, &g_writePipe, &sa, 0))
    {
        winapi_pint_err("CreatePipe");
        return false;
    }

    // Save original stdout handle
    g_oldStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (g_oldStdout == INVALID_HANDLE_VALUE)
    {
        winapi_pint_err("GetStdHandle");
        return false;
    }

    // Duplicate CRT stdout fd
    g_oldCrtStdout = _dup(_fileno(stdout));
    if (g_oldCrtStdout == -1)
    {
        fprintf(stderr, "_dup failed\n");
        return false;
    }

    // Set new stdout handle (WinAPI)
    if (!SetStdHandle(STD_OUTPUT_HANDLE, g_writePipe))
    {
        winapi_pint_err("SetStdHandle");
        return false;
    }

    // Convert HANDLE -> CRT fd
    int pipeFd = _open_osfhandle((intptr_t)g_writePipe, _O_TEXT);
    if (pipeFd == -1)
    {
        fprintf(stderr, "_open_osfhandle failed\n");
        return false;
    }

    // Redirect CRT stdout to pipe
    if (_dup2(pipeFd, _fileno(stdout)) == -1)
    {
        fprintf(stderr, "_dup2 failed\n");
        return false;
    }

    // Disable buffering for immediate capture
    setvbuf(stdout, NULL, _IONBF, 0);

    return true;
}

char *end_stdout_capture()
{
    if (!g_readPipe || !g_writePipe)
    {
        return NULL;
    }

    fflush(stdout);

    // Restore CRT stdout

    _dup2(g_oldCrtStdout, _fileno(stdout));
    _close(g_oldCrtStdout);
    g_oldCrtStdout = -1;

    // Restore WinAPI stdout

    SetStdHandle(STD_OUTPUT_HANDLE, g_oldStdout);
    g_oldStdout = NULL;

    // Close write end so ReadFile gets EOF
    CloseHandle(g_writePipe);
    g_writePipe = NULL;

    // Read all data from pipe
    DWORD bytesRead;
    size_t total = 0;
    size_t capacity = 4096;
    char *buffer = (char *)malloc(capacity);

    if (!buffer)
    {
        CloseHandle(g_readPipe);
        g_readPipe = NULL;
        return NULL;
    }

    while (1)
    {
        if (total + 4096 > capacity)
        {
            capacity *= 2;
            char *newBuf = (char *)realloc(buffer, capacity);
            if (!newBuf)
            {
                free(buffer);
                CloseHandle(g_readPipe);
                g_readPipe = NULL;
                return NULL;
            }
            buffer = newBuf;
        }

        if (!ReadFile(g_readPipe, buffer + total, 4096, &bytesRead, NULL) || bytesRead == 0)
        {
            break;
        }

        total += bytesRead;
    }

    CloseHandle(g_readPipe);
    g_readPipe = NULL;

    // Null-terminate
    buffer[total] = '\0';

    return buffer;
}

#else
// not implemented!
void start_stdout_capture() {}
char *get_captured_stdout() {}
#endif

int test_single_file(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file)
{
    std::cout << std::endl
              << "logia file      : " << logia_folder << file << ".logia" << std::endl
              << "current  ll file: " << ir_folder << file << ".ll" << std::endl
              << "expected ll file: " << ".\\tests\\expected-ir\\" << file << ".ll" << std::endl;

    bool debug = true;

    int argc = debug ? 7 : 5;
    //++argc;
    const char **argv = (const char **)malloc(sizeof(char *) * argc);
    int arg = 0;
    auto logia_file = std::format("{}{}{}", logia_folder, file, ".logia");
    argv[arg++] = logia_file.c_str();

    argv[arg++] = "--emit-llvm";
    auto llfile = std::format("{}{}{}", ir_folder, file, ".ll");
    argv[arg++] = llfile.c_str();

    argv[arg++] = "--emit-obj";
    auto objfile = std::format("{}{}{}", obj_folder, file, ".o");
    argv[arg++] = objfile.c_str();

    if (debug)
    {
        argv[arg++] = "--debug";
        argv[arg++] = "--coverage";
    }
    // argv[arg++] = "--verbose";

    auto ret = logia::logia_run(argc, argv);
    free(argv);
    return ret;
}

const char *test_file_with_semantic_error(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file)
{
    try
    {
        test_single_file(logia_folder, ir_folder, obj_folder, file);
        EXPECT_TRUE(false && "unreachable, file run but no error found!");
    }
    catch (logia::AST::semantic_error const &e)
    {
        std::cerr << "catch logia::AST::semantic_error" << std::endl;
        return strdup(e.what());
    }
    catch (std::exception const &e)
    {
        std::cerr << "catch std::exception" << std::endl;
        throw std::runtime_error(std::format("file: {} throw an unexpected exception:\n{}", file, e.what()));
    }

    throw std::runtime_error(std::format("file: {} don't throw", file));
}
