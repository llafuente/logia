#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "logia/frontend.h"
#include "logia/backend.h"

bool start_stdout_capture();
char *end_stdout_capture();

#define LOGIA_UNIT_TEST()                                                                       \
    logia::ParseResult *parse_result = logia::logia_parse_program(".\\tests\\unit-test.logia"); \
    auto backend = new logia::Backend(parse_result);                                            \
    backend->load_intrinsics();                                                                 \
    logia::AST::Program *program = backend->program;                                            \
    logia::AST::Function *main_fn = program->look<logia::AST::Function>("main");                \
    logia::AST::Block *main_body = main_fn->get_body();                                         \
    auto i1 = program->look<logia::AST::Integer>("λi1");                                        \
    auto i18 = program->look<logia::AST::Integer>("λi8");                                       \
    auto i16 = program->look<logia::AST::Integer>("λi16");                                      \
    auto i32 = program->look<logia::AST::Integer>("λi32");                                      \
    auto i64 = program->look<logia::AST::Integer>("λi64");

#define LOGIA_UNIT_TEST_END() \
    do                        \
    {                         \
        delete backend;       \
        delete parse_result;  \
    } while (false)

int test_single_file(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file);
const char *test_file_with_semantic_error(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file);