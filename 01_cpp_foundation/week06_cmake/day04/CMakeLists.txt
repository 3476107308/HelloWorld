cmake_minimum_required(VERSION 3.16)

project(day04_demo LANGUAGES CXX)

add_library(score_lib STATIC
    src/score_lib.cpp
)

target_include_directories(score_lib PUBLIC include)

add_executable(day04_example
    src/example_main.cpp
)

target_link_libraries(day04_example PRIVATE score_lib)

add_executable(day04_exercise
    src/exercise_main.cpp
)

target_link_libraries(day04_exercise PRIVATE score_lib)
