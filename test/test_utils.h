#pragma once

#include <stdio.h>
#include <stdlib.h>

bool start_stdout_capture();
char *end_stdout_capture();

#define LOGIA_BACKEND_START()                                                                                                                                         \
    logia::Backend *back;                                                                                                                                             \
    logia::AST::Program *program;                                                                                                                                     \
    logia::AST::Function *main_fn;                                                                                                                                    \
    logia::AST::Block *main_body;                                                                                                                                     \
    do                                                                                                                                                                \
    {                                                                                                                                                                 \
        back = new logia::Backend();                                                                                                                                  \
        program = back->program;                                                                                                                                      \
        back->load_intrinsics();                                                                                                                                      \
        main_fn = logia::AST::ast_create_function_type(logia::AST::ast_create_identifier(strdup("main")), logia::AST::ast_get_type_by_name(program, strdup("λi64"))); \
        EXPECT_TRUE(main_fn);                                                                                                                                         \
        program->push_child(main_fn);                                                                                                                                 \
        main_body = main_fn->get_body();                                                                                                                              \
    } while (0)

#define LOGIA_BACKEND_END() \
    do                      \
    {                       \
        delete back;        \
    } while (false)
