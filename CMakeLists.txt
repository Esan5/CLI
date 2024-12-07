cmake_minimum_required(VERSION 3.25)

project(
  cli 
  DESCRIPTION "cli[-inator]"
  LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_STANDARD 23)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Weffc++ -freflection-latest)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(e01 examples/e01.cc)
add_executable(e02 examples/e02.cc)
add_executable(e03 examples/e03.cc)
