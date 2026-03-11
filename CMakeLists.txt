cmake_minimum_required(VERSION 3.10)
project(logia)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  antlr_runtime
  GIT_REPOSITORY https://github.com/antlr/antlr4.git
  GIT_TAG 4.13.1
  SOURCE_SUBDIR runtime/Cpp
)
FetchContent_MakeAvailable(antlr_runtime)

set(ANTLR_EXECUTABLE ${CMAKE_BINARY_DIR}/antlr-4.13.1-complete.jar)
if(NOT EXISTS ${ANTLR_EXECUTABLE})
  file(DOWNLOAD https://www.antlr.org/download/antlr-4.13.1-complete.jar ${ANTLR_EXECUTABLE})
endif()

include(${antlr_runtime_SOURCE_DIR}/runtime/Cpp/cmake/FindANTLR.cmake)

add_executable(logia src/main.cpp)
target_link_libraries(logia PRIVATE antlr4_shared)
