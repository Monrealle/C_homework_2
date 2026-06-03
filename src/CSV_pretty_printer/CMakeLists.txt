cmake_minimum_required(VERSION 3.10)
project(CSV)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Добавляем include директорию
add_library(csv_lib STATIC src/csv.c)
target_include_directories(csv_lib PUBLIC include)

add_executable(csv src/main.c)
target_link_libraries(csv csv_lib)

add_executable(test_csv tests/test_csv.c)
target_link_libraries(test_csv csv_lib)
target_include_directories(test_csv PRIVATE include)

enable_testing()
add_test(NAME test_csv COMMAND test_csv)
