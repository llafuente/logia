# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR_BUILD_STATIC ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
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
  message(STATUS "Download: ${ANTLR_EXECUTABLE}")
  file(DOWNLOAD https://www.antlr.org/download/antlr-4.13.1-complete.jar ${ANTLR_EXECUTABLE})
else()
  message(STATUS "Downloaded: ${ANTLR_EXECUTABLE}")
endif()

include(${antlr_runtime_SOURCE_DIR}/runtime/Cpp/cmake/FindANTLR.cmake)
#include(${antlr_runtime_SOURCE_DIR}/runtime/Cpp/cmake/ExternalAntlr4Cpp.cmake)

# add antlr4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(LogiaGrammarLexer src/LogiaLexer.g4 LEXER
             PACKAGE logia)
antlr_target(LogiaGrammarParser src/LogiaParser.g4 PARSER
             PACKAGE logia
             DEPENDS_ANTLR LogiaGrammarLexer
             COMPILE_FLAGS -lib ${ANTLR_LogiaGrammarLexer_OUTPUT_DIR} -visitor)

# include generated files in project environment
include_directories(${ANTLR_LogiaGrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_LogiaGrammarParser_OUTPUT_DIR})

include_directories(${antlr_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src)