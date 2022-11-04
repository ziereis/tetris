cmake_minimum_required(VERSION 3.14)

project(SFML_GRAPHICS)

set(CMAKE_CXX_STANDARD 20)


add_compile_options(-Wall -Wextra -Wconversion -pedantic -Wfatal-errors)

add_subdirectory("src")
