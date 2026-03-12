# https://github.com/llvm/llvm-project/blob/main/llvm/docs/CMake.rst
cmake_minimum_required(VERSION 3.20)
project(logia LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)

# once downloaded no more!
set(FETCHCONTENT_FULLY_DISCONNECTED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(Python_EXECUTABLE "C:/Users/luis/.pyenv/pyenv-win/versions/3.10.11/python.exe" CACHE FILEPATH "Path to Python executable")
set(Python3_ROOT_DIR "C:/Users/luis/.pyenv/pyenv-win/versions/3.10.11/")

#
# start antlr4
#

# compiler requires C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  antlr_runtime
  GIT_REPOSITORY https://github.com/antlr/antlr4.git
  GIT_TAG 4.13.1
  GIT_SHALLOW    TRUE
  SOURCE_SUBDIR runtime/Cpp
)
FetchContent_MakeAvailable(antlr_runtime)

set(ANTLR_EXECUTABLE ${CMAKE_BINARY_DIR}/antlr-4.13.1-complete.jar)
if(NOT EXISTS ${ANTLR_EXECUTABLE})
  file(DOWNLOAD https://www.antlr.org/download/antlr-4.13.1-complete.jar ${ANTLR_EXECUTABLE})
endif()

include(${antlr_runtime_SOURCE_DIR}/runtime/Cpp/cmake/FindANTLR.cmake)

include_directories(${antlr_SOURCE_DIR}/runtime/Cpp/runtime/src)

#
# start llvm
# https://llvm.org/devmtg/2023-10/slides/tutorials/Hosek-UnderstandingtheLLVMbuild.pdf
# https://llvm.org/docs/CMake.html#llvm-related-variables

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  llvm_project
  GIT_REPOSITORY https://github.com/llvm/llvm-project.git
  GIT_TAG release/22.x
  GIT_SHALLOW    TRUE
  GIT_PROGRESS TRUE
  
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/llvm-project/src
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/llvm-project/build
  # we do not want FetchContent to configure the project automatically
  # so we point it to non existing subdir
  SOURCE_SUBDIR null

  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(llvm_project)

FetchContent_GetProperties(llvm_project SOURCE_DIR FETCHCONTENT_SOURCE_DIR_LLVMPROJECT)

# display all runtimes!
#set(LLVM_ENABLE_RUNTIMES notaruntime)
#libc;libunwind;libcxxabi;libcxx;compiler-rt;openmp;llvm-libgcc;offload;flang-rt;libclc;libsycl;orc-rt

set(LLVM_DIR "${llvm_project_SOURCE_DIR}/llvm" CACHE PATH "LLVM DIR" FORCE)
set(LLVM_USE_CRT_RELEASE MD)
set(LLVM_INCLUDE_DIRS ${llvm_project_SOURCE_DIR}/llvm/include)

set(LLVM_TARGETS_TO_BUILD Native)
set(LLVM_ENABLE_PROJECTS clang;lldb;libc)
set(LLVM_ENABLE_RUNTIMES libc;orc-rt)
set(LLVM_BUILD_LLVM_DYLIB ON)
set(LLVM_LINK_LLVM_DYLIB ON)
set(LLVM_ENABLE_RTTI ON)
set(LLVM_ENABLE_ASSERTIONS ON)

# MSVC-specific options
if(MSVC)
    # TODO: This assumes the installed LLVM was built in Release mode
    # TODO: this is not very friendly cmake, probably this should respect the cache and not override the user's choice
    set(CMAKE_C_FLAGS_DEBUG "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)

    if(${LLVM_USE_CRT_RELEASE} STREQUAL "MD")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
    elseif(${LLVM_USE_CRT_RELEASE} STREQUAL "MT")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    else()
        message(FATAL_ERROR "Unsupported LLVM_USE_CRT_RELEASE=${LLVM_USE_CRT_RELEASE}")
    endif()
endif()

message(STATUS "llvm_project_SOURCE_DIR ${llvm_project_SOURCE_DIR}")
message(STATUS "LLVM_DIR ${LLVM_DIR}")
message(STATUS "LLVM_USE_CRT_RELEASE ${llvm_project_SOURCE_DIR}")
message(STATUS "LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_CMAKE_ARGS ${LLVM_CMAKE_ARGS}")


add_subdirectory(
    ${llvm_project_SOURCE_DIR}/llvm llvm
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_library(llvm INTERFACE)

target_include_directories(llvm INTERFACE ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(llvm INTERFACE ${LLVM_DEFINITIONS_LIST})


add_executable(logia src/main.cpp
  ${ANTLR_LogiaLexer_CXX_OUTPUTS}
  ${ANTLR_LogiaParser_CXX_OUTPUTS})

target_link_libraries(logia
  antlr4_static
  LLVMOrcJIT
  LLVMExecutionEngine
  LLVMCore
  LLVMSupport)

#target_link_libraries(llvm INTERFACE ${LLVM_LIBS})

target_compile_features(logia PRIVATE
    cxx_std_20
)
