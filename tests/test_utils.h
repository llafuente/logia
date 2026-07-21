#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "logia/frontend.h"
#include "logia/backend.h"
#include "logia/ast/program.h"
#include "logia/ast/types.h"
#include "logia/ast/block.h"
#include "logia/run.h"

bool start_stdout_capture();
char *end_stdout_capture();

#define LOGIA_UNIT_TEST()                                                                  \
    auto _ = logia::logia_parse_program(".\\tests\\unit-test.logia");                      \
    auto program = _.get();                                                                \
    auto backend = program->backend.get();                                                 \
    program->load_intrinsics();                                                            \
    logia::AST::Function *main_fn = scope_look_one<logia::AST::Function>(program, "main"); \
    logia::AST::Block *main_body = main_fn->get_body();                                    \
    auto i1 = scope_look_one<logia::AST::Integer>(program, "λi1");                         \
    auto i18 = scope_look_one<logia::AST::Integer>(program, "λi8");                        \
    auto i16 = scope_look_one<logia::AST::Integer>(program, "λi16");                       \
    auto i32 = scope_look_one<logia::AST::Integer>(program, "λi32");                       \
    auto i64 = scope_look_one<logia::AST::Integer>(program, "λi64");                       \
    logia::AST::location loc = {"", 0, 0, 0, 0, 0, 0, ""};

logia::logia_run_result test_single_file(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file);
const char *test_file_with_semantic_error(const char *logia_folder, const char *ir_folder, const char *obj_folder, const char *file);

logia::AST::TypeDef *test_make_typdef(const char *type);
