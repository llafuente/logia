# https://github.com/llvm/llvm-project/blob/main/llvm/docs/CMake.rst
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_VERBOSE_MAKEFILE ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

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
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
include(gtest)


project(logia LANGUAGES CXX)

# compiler binary
message(STATUS "ANTLR_LogiaGrammarLexer_CXX_OUTPUTS ${ANTLR_LogiaGrammarLexer_CXX_OUTPUTS}")
message(STATUS "ANTLR_LogiaGrammarParser_CXX_OUTPUTS ${ANTLR_LogiaGrammarParser_CXX_OUTPUTS}")

add_executable(logia
  src/main.cpp
  src/compiler.cpp
  src/llvmvisitor.cpp
  ${ANTLR_LogiaGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_LogiaGrammarParser_CXX_OUTPUTS}
)

target_link_libraries(logia PRIVATE antlr4_static)
target_link_libraries(logia PRIVATE ${LLVM_LIBS})


# compiler unit tests
include_directories(src)

add_executable(logia_test_suite
  src/ast.cpp
  test/test_ast.cpp
)
# Register the test with CTest
add_test(NAME logia_test_ast COMMAND logia_test_suite)
target_link_libraries(logia_test_suite PRIVATE gtest_main gmock_main)

# target_compile_features(logia PRIVATE cxx_std_20)


if (ASAN_ENABLED)
  message(STATUS "Enable ASAN")
  include(sanitizer)
  enable_sanitizer(logia)
  enable_sanitizer(logia_test_suite)
endif()
