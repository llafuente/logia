#include "utils.h"

// debug
std::ofstream logia_log_file;

bool logia_init_log(char* file_name)
{
    logia_log_file.open(file_name, std::ios::out | std::ios::app);
    if (!logia_log_file.is_open())
    {
        std::cerr << "Error: Could not open log file: " << file_name << "\n";
        return false;
    }
    return true;
}

void logia_deinit_log() {
    if (logia_log_file.is_open()) {
        logia_log_file.close();
    }
}


#ifdef _WIN32

#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <string>
#include <cstdlib>

// Link with DbgHelp.lib
#pragma comment(lib, "dbghelp.lib")

// Function to print a stack trace
void print_stack_trace() {
    unsigned int maxFrames = 62;
    // Allocate symbol table
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void* stack[62];
    USHORT frames = CaptureStackBackTrace(0, maxFrames, stack, NULL);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    std::cerr << "Stack trace (" << frames << " frames):\n";
    for (USHORT i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        std::cerr << "  [" << i << "] " << symbol->Name
                  << " - 0x" << std::hex << symbol->Address << std::dec << "\n";
    }

    free(symbol);
}

#else

#include <execinfo.h> // for backtrace()
#include <unistd.h>

// Function to print a stack trace
void print_stack_trace() {
    constexpr int MAX_FRAMES = 64;
    void* addrlist[MAX_FRAMES];

    // Get void*'s for all entries on the stack
    int addrlen = backtrace(addrlist, MAX_FRAMES);

    if (addrlen == 0) {
        std::cerr << "  <empty stack trace>\n";
        return;
    }

    // Print the stack trace to stderr
    char** symbollist = backtrace_symbols(addrlist, addrlen);
    if (symbollist == nullptr) {
        perror("backtrace_symbols");
        return;
    }

    std::cerr << "Stack trace (" << addrlen << " frames):\n";
    for (int i = 0; i < addrlen; i++) {
        std::cerr << "  " << symbollist[i] << "\n";
    }

    free(symbollist);
}

#endif



