cmake_minimum_required(VERSION 3.16)

project(calculator_demo LANGUAGES CXX)

add_executable(calculator_demo
    src/main.cpp
    src/calculator.cpp
)

target_include_directories(calculator_demo PRIVATE include)