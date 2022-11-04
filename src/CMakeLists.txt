cmake_minimum_required(VERSION 3.14)

set(EXECUTABLE_NAME main)

add_executable(${EXECUTABLE_NAME} main.cpp)

find_package(SFML REQUIRED graphics window system)

target_link_libraries(${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system)
target_include_directories(${EXECUTABLE_NAME} PUBLIC ../header)
