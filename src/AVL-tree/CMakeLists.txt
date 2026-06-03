cmake_minimum_required(VERSION 3.10)
project(AVL)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-O2 -Wall")
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(avl src/main.c)
