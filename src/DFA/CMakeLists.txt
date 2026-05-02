cmake_minimum_required(VERSION 3.10)
project(CSV)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Добавляем include директорию
add_library(dfa_lib STATIC src/dfa.c)
target_include_directories(dfa_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(dfa src/main.c)
target_link_libraries(dfa dfa_lib)

add_executable(test_dfa tests/test_dfa.c)
target_link_libraries(test_dfa dfa_lib)
target_include_directories(test_dfa PRIVATE include)

enable_testing()
add_test(NAME test_dfa COMMAND test_dfa)
