cmake_minimum_required(VERSION 3.16)

project(day05_gdb_debug LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(score_debug STATIC
    src/score_debug.cpp
)

target_include_directories(score_debug PUBLIC include)

add_executable(day05_example
    src/example_main.cpp
)

target_link_libraries(day05_example PRIVATE score_debug)

add_executable(day05_exercise
    src/exercise_main.cpp
)

target_link_libraries(day05_exercise PRIVATE score_debug)
