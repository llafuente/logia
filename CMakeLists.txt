# https://github.com/llvm/llvm-project/blob/main/llvm/docs/CMake.rst
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")


if(CMAKE_GENERATOR MATCHES "Ninja")
  # fix clang no support CXX compiler feature
  set(CMAKE_POLICY_DEFAULT_CMP0025 NEW)
  # enable compile_commands.json to use clang-check -> requires ninja
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
# once downloaded no more!
#set(FETCHCONTENT_FULLY_DISCONNECTED ON)


#
# start antlr4
#

include(antlr4)
include(llvm)

#
# logia
#
# fix: Error 69 error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MT_StaticRelease' doesn't match value 'MD_DynamicRelease'
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Static
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL") # Dinamic
include(gtest)


project(logia LANGUAGES CXX)

# compiler binary
message(STATUS "ANTLR_LogiaGrammarLexer_CXX_OUTPUTS ${ANTLR_LogiaGrammarLexer_CXX_OUTPUTS}")
message(STATUS "ANTLR_LogiaGrammarParser_CXX_OUTPUTS ${ANTLR_LogiaGrammarParser_CXX_OUTPUTS}")

set(LOGIA_SOURCE_FILES
  src/logia/log.cpp

  src/logia/help.cpp
  src/logia/run.cpp
  src/logia/cst2ast.cpp
  src/logia/compiler_error.cpp
  src/logia/config.cpp
  src/logia/frontend.cpp
  src/logia/backend.cpp
  src/logia/type_inference.cpp
  src/logia/type_system.cpp
  src/logia/maybe_error.cpp
  src/logia/multiple_dispatch.cpp

  src/logia/ast/location.cpp
  src/logia/ast/node.cpp
  src/logia/ast/semantic_error.cpp
  src/logia/ast/type.cpp
  src/logia/ast/struct.cpp
  src/logia/ast/function.cpp

  src/logia/ast/cast.cpp

  src/logia/ast/scope.cpp
  src/logia/ast/block.cpp
  src/logia/ast/functionblock.cpp
  src/logia/ast/program.cpp
  src/logia/ast/package.cpp

  src/logia/ast/stmt.cpp
  src/logia/ast/returnstmt.cpp
  src/logia/ast/vardeclstmt.cpp
  src/logia/ast/if_stmt.cpp
  src/logia/ast/import.cpp

  src/logia/ast/operators.cpp
  src/logia/ast/expr.cpp
  src/logia/ast/callexpr.cpp
  src/logia/ast/memberaccessexpr.cpp
  src/logia/ast/unaryexpr.cpp
  src/logia/ast/structinitexpr.cpp
  src/logia/ast/identifier.cpp
  src/logia/ast/binaryexpr.cpp
  src/logia/ast/constexpr.cpp

  src/logia/ast/llvm.cpp

  src/utils.cpp
)

add_executable(logia
  src/main.cpp
  ${LOGIA_SOURCE_FILES}
  ${ANTLR_LogiaGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_LogiaGrammarParser_CXX_OUTPUTS}
)




# compiler unit tests
include_directories(src)

add_executable(logia_test_suite
  tests/main.cpp
  tests/test_utils.cpp
  tests/test_node.cpp
  tests/test_ast.cpp
  tests/test_logia.cpp
  tests/test_logia_err.cpp
  tests/test_type_system.cpp
  tests/test_multiple_dispatch.cpp
  ${LOGIA_SOURCE_FILES}
  ${ANTLR_LogiaGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_LogiaGrammarParser_CXX_OUTPUTS}
)
# Register the test with CTest
add_test(NAME logia_test_ast COMMAND logia_test_suite)
target_link_libraries(logia_test_suite PRIVATE gtest_main gmock_main)

set(LLVM_LIBS ${LLVM_LIBS}
  LLVMX86Info
  LLVMX86TargetMCA
  LLVMX86Disassembler
  LLVMX86Desc
  LLVMX86CodeGen
  )

foreach(project_name IN ITEMS "logia" "logia_test_suite")
  message(STATUS "Apply common config to: ${project_name}")
  message(STATUS "LLVM_LIBS = ${LLVM_LIBS}")

  target_link_libraries(${project_name} PRIVATE antlr4_static)
  target_link_libraries(${project_name} PRIVATE ${LLVM_LIBS})

  set_target_properties(${project_name} PROPERTIES
      CXX_STANDARD 23 # std::stacktrace
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

  if (WIN32)
      target_compile_definitions(${project_name} PRIVATE
        # Prevents Windows.h from adding unnecessary includes
        WIN32_LEAN_AND_MEAN
        # Prevents Windows.h from defining min/max as macros
        NOMINMAX
      )
  endif()

  message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")

  if (MSVC)
      target_compile_options(${project_name} PRIVATE
          /Zc:preprocessor # needed for __VA_OPT__
          /W4            # High warning level
          /permissive-   # Stricter C++
      )
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_compile_options(${project_name} PRIVATE
          -Wall -Wextra -Wpedantic
      )
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      target_compile_options(${project_name} PUBLIC
          -Wall
          -Wextra
          -Wpedantic
      )
  endif()



  if (ASAN_ENABLED)
    message(STATUS "Enable ASAN")
    include(sanitizer)
    enable_sanitizer(${project_name})
  endif()
endforeach()


