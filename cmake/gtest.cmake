if(false)
cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  logia_googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(logia_googletest)
endif()

set(gtest_force_shared_crt on)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  logia_googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.x
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(logia_googletest)

include_directories(${logia_googletest_SOURCE_DIR}/googletest/include)
include_directories(${logia_googletest_SOURCE_DIR}/googlemock/include)