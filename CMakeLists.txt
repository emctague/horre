cmake_minimum_required(VERSION 3.14)
project(horre)

set(CMAKE_CXX_STANDARD 17)

add_executable(horre src/main.cpp glad/src/glad.c src/glinc.h)
target_link_libraries(horre glfw)
target_include_directories(horre PRIVATE glad/include)
