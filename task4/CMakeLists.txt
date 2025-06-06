cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang++")

set(SOURCES 
  main.cpp
)

add_executable(task4 ${SOURCES})

set_target_properties(
  task4 PROPERTIES
  CXX_STANDART 20
  CXX_STANDART_REQUIRED ON
)

project(task4 VERSION 0.0.1 LANGUAGES C CXX)
