cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang++")

set(SOURCES 
  src/main.cpp
  src/hash_table/hash_table.cpp
  src/list/list.cpp
)

add_executable(task3 ${SOURCES})

set_target_properties(
  task3 PROPERTIES
  CXX_STANDART 20
  CXX_STANDART_REQUIRED ON
)

project(task3 VERSION 0.0.1 LANGUAGES C CXX)
