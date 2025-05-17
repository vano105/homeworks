cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@19/bin/clang++")
set(CMAKE_CXX_STANDART 23)

project(task2 VERSION 0.0.1 LANGUAGES C CXX)

set(SOURCES 
  main.cpp
)

add_executable(task2 ${SOURCES} ${HEADERS})
